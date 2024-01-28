#include "escenas.h"

extern TriTextura triTxt;
vector<pLuz *> luces;

Textura cargarTextura(string ficheroEntrada, float achoTextura)
{
  cout << "🎨 CARGANDO TEXTURA " << ficheroEntrada << " --- ";
  string formato = "";
  int width, height, resolucion;
  double maximo;
  vector<Pixel> pixeles;

  leerFichero(ficheroEntrada, formato, width, height, maximo, resolucion, pixeles);

  vector<Pixel> pixeles_rgb = conversion_HDR_RGB(pixeles, maximo, resolucion);
  cout << "TEXTURA CARGADA 🎨" << endl;

  return Textura(width, height, pixeles, achoTextura / width);
}

void leerPLY(string ficheroEntrada, Color color, bool textura, BRDF brdf, int tipoLuz, Color potencia)
{
  ifstream plyFile(ficheroEntrada + ".ply"); // Cambia "archivo.ply" por el nombre de tu archivo PLY
  string line, carpeta, strTextura;
  vector<Vertex> vertices;
  int numVertices = 0;
  int numFaces = 0;
  int currentVertex = 0;
  int currentFace = 0;
  bool readingVertices = false;
  bool readingFaces = false;
  int nTexts = triTxt.txt.size();

  bool archivoConColores = false;

  cout << "📂 Abriendo " << ficheroEntrada + ".ply 📂\n";

  // Buscamos la última aparición del carácter '/'
  size_t found = ficheroEntrada.find_last_of('/');

  // Si encontramos el carácter '/' en el path
  if (found != string::npos)
  {
    // Usamos substr para obtener la parte de la cadena antes del último '/'
    carpeta = ficheroEntrada.substr(0, found);
  }
  else
  {
    // Si no se encuentra '/', el archivo está en el directorio raíz
    carpeta = "";
  }

  if (plyFile.is_open())
  {
    while (getline(plyFile, line))
    {
      if (line.find("comment TextureFile") != string::npos)
      {
        char s[100];
        sscanf(line.c_str(), "comment TextureFile %s", &s);
        strTextura = s;
        bool encontrado = false; // Variable para almacenar si se encontró "caca"

        if (textura)
        {

          // Recorremos el vector para buscar "caca"
          for (const std::string &elemento : triTxt.ficheros)
          {
            if (elemento == strTextura)
            {
              encontrado = true;
              break; // Si se encuentra, salimos del bucle
            }
          }

          if (!encontrado)
          {
            Textura tx = cargarTextura(carpeta + '/' + strTextura, 1);
            triTxt.txt.push_back(tx);
            triTxt.ficheros.push_back(strTextura);
          }
        }
      }

      if (line.find("element vertex") != string::npos)
      {
        sscanf(line.c_str(), "element vertex %d", &numVertices);
      }

      if (line.find("property uchar r") != string::npos)
      {
        archivoConColores = true;
      }

      if (line.find("element face") != string::npos)
      {
        sscanf(line.c_str(), "element face %d", &numFaces);
      }

      if (line == "end_header")
      {
        // Después de "end_header", los datos de los vértices y las caras comienzan
        readingVertices = true;
        // cout << "NVertices: " << numVertices << endl;
        // cout << "NFaces: " << numFaces << endl;
        continue;
      }

      if (readingVertices && currentVertex < numVertices)
      {
        Vertex vertex;
        sscanf(line.c_str(), "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
        vertices.push_back(vertex);
        currentVertex++;
      }

      if (readingFaces && currentFace < numFaces)
      {
        int v1 = 0, v2 = 0, v3 = 0;

        if (archivoConColores)
        {
          int r, g, b;
          sscanf(line.c_str(), "3 %d %d %d %d %d %d", &v1, &v2, &v3, &r, &g, &b);
          color.r = r;
          color.g = g;
          color.b = b;
          TriTxt txt = TriTxt(0, 0, 0, 0, 0, 0, -1);
          triTxt.tri.push_back(txt);
        }
        else
        {
          if (textura)
          {
            float Tv1x, Tv1y, Tv2x, Tv2y, Tv3x, Tv3y;
            int n;
            int numCamposLeidos = sscanf(line.c_str(), "3 %d %d %d 6 %f %f %f %f %f %f %i", &v1, &v2, &v3, &Tv1x, &Tv1y, &Tv2x, &Tv2y, &Tv3x, &Tv3y, &n);
            n = numCamposLeidos == 10 ? 0 : n;
            if (n > triTxt.txt.size() - 1)
            {
              n = 0;
            }

            TriTxt txt = TriTxt(Tv1x, Tv1y, Tv2x, Tv2y, Tv3x, Tv3y, n + nTexts);
            triTxt.tri.push_back(txt);
          }
          else
          {
            sscanf(line.c_str(), "3 %d %d %d", &v1, &v2, &v3);
            TriTxt txt = TriTxt(0, 0, 0, 0, 0, 0, -1);
            triTxt.tri.push_back(txt);
          }
        }

        // Crea un triángulo usando los índices de los vértices
        Triangulo t = Triangulo(G_O(vertices[v1].x, vertices[v1].y, vertices[v1].z, 1),
                                G_O(vertices[v2].x, vertices[v2].y, vertices[v2].z, 1),
                                G_O(vertices[v3].x, vertices[v3].y, vertices[v3].z, 1),
                                color, brdf, tipoLuz, potencia);
        tri.push_back(t);
        currentFace++;
      }

      if (readingVertices && currentVertex >= numVertices)
      {
        readingVertices = false;
        readingFaces = true;
      }
    }
    plyFile.close();
    cout << "🧊 Modelo 3D cargado: " << currentFace << " triangulos cargados 🧊" << endl;
  }
  else
  {
    cout << "🧊 No se pudo abrir el archivo PLY. 🧊" << endl;
  }

  // Ahora, tienes los triángulos almacenados en el vector 'triangulos'.
  // Puedes acceder a cada triángulo y sus vértices como desees.
}

void cornellBox(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;
  Esfera leftSphere = {-0.5, -0.7, 0.25, 0.3, Color(227, 172, 250)};
  Esfera rightSphere = {0.5, -0.7, -0.25, 0.3, Color(140, 251, 251)};

  Plano leftPlane = {-1.0, 0.0, 0.0, 1.0, Color(255, 0, 0)};
  Plano rightPlane = {1.0, 0.0, 0.0, 1.0, Color(0, 255, 0)};
  Plano floorPlane = {0.0, -1.0, 0.0, 1.0, Color(196, 196, 196)};
  Plano ceilingPlane = {0.0, 1.0, 0.0, 1.0, Color(196, 196, 196)};
  Plano backPlane = {0.0, 0.0, 1.0, 1.0, Color(196, 196, 196)};

  geometrias.push_back(&ceilingPlane);
  geometrias.push_back(&leftPlane);
  geometrias.push_back(&rightPlane);
  geometrias.push_back(&leftSphere);
  geometrias.push_back(&rightSphere);
  geometrias.push_back(&floorPlane);
  geometrias.push_back(&backPlane);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};

  camara.crearImagenPhotonMapping("./renders/cornellBox", geometrias, tipoColor, NO_MOSTRAR_PROGRESO, DL_USING_PM);
}

void cornellBoxLUZ(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  pLuz p1 = pLuz(G_O(0, 0.5, 0, 1), Color(10, 10, 10));
  luces.push_back(&p1);

  Esfera leftSphere = {-0.5, -0.7, 0.25, 0.3, Color(140, 251, 251), Textura(), BRDF(BRDF_PLASTICO)};
  Esfera rightSphere = {0.5, -0.7, -0.25, 0.3, Color(227, 172, 250), Textura(), BRDF(BRDF_REFRACTIVO)};

  Plano leftPlane = {-1.0, 0.0, 0.0, 1.0, Color(255, 0, 0)};
  Plano rightPlane = {1.0, 0.0, 0.0, 1.0, Color(0, 255, 0)};
  Plano floorPlane = {0.0, -1.0, 0.0, 1.0, Color(200, 200, 200)};
  Plano ceilingPlane = {0.0, 1.0, 0.0, 1.0, Color(200, 200, 200), Textura(), BRDF(BRDF_DIFUSO), SIN_LUZ, Color(10, 10, 10)};
  Plano backPlane = {0.0, 0.0, 1.0, 1.0, Color(200, 200, 200)};
  Plano backPlane2 = {0.0, 0.0, -1.0, 1.0, Color(200, 200, 200)};

  geometrias.push_back(&rightSphere);
  geometrias.push_back(&leftSphere);

  geometrias.push_back(&ceilingPlane);
  geometrias.push_back(&leftPlane);
  geometrias.push_back(&rightPlane);
  geometrias.push_back(&floorPlane);
  geometrias.push_back(&backPlane);

  Camara camara = Camara{G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                         G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};
  // camara.rotarCamara(EJE_Z, -8);

  camara.crearImagenPhotonMapping("./renders/cornellBox", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
  // camara.crearImagenPathTracer("./renders/cornellBox", geometrias, tipoColor, SI_MOSTRAR_PROGRESO);
}

void cornellBoxCamaraOptions(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  pLuz p1 = pLuz(G_O(0, 0.5, 0, 1), Color(1e5, 1e5, 1e5));
  luces.push_back(&p1);
  pLuz p2 = pLuz(G_O(0, 0.5, -1, 1), Color(1e5, 1e5, 1e5));
  luces.push_back(&p2);
  pLuz p3 = pLuz(G_O(0, 0.5, -2, 1), Color(1e5, 1e5, 1e5));
  luces.push_back(&p3);
  pLuz p4 = pLuz(G_O(0, 0.5, -3, 1), Color(1e5, 1e5, 1e5));
  luces.push_back(&p4);
  pLuz p5 = pLuz(G_O(0, 0.5, -4, 1), Color(1e5, 1e5, 1e5));
  luces.push_back(&p5);

  Esfera sphere1 = {-0.7, -0.7, 0.5, 0.3, Color(255, 0, 0), Textura(), BRDF(BRDF_DIFUSO)};
  Esfera sphere2 = {-0.4, -0.7, -0.5, 0.3, Color(0, 255, 0), Textura(), BRDF(BRDF_DIFUSO)};
  Esfera sphere3 = {-0.1, -0.7, -1.5, 0.3, Color(0, 0, 255), Textura(), BRDF(BRDF_DIFUSO)};
  Esfera sphere4 = {0.25, -0.7, -2.5, 0.3, Color(255, 0, 0), Textura(), BRDF(BRDF_DIFUSO)};
  Esfera sphere5 = {0.5, -0.7, -3.5, 0.3, Color(0, 255, 0), Textura(), BRDF(BRDF_DIFUSO)};
  Esfera sphere6 = {.75, -0.7, -4.5, 0.3, Color(0, 0, 255), Textura(), BRDF(BRDF_DIFUSO)};

  Plano leftPlane = {-1.0, 0.0, 0.0, 1.0, Color(255, 0, 0)};
  Plano rightPlane = {1.0, 0.0, 0.0, 1.0, Color(0, 255, 0)};
  Plano floorPlane = {0.0, -1.0, 0.0, 1.0, Color(200, 200, 200)};
  Plano ceilingPlane = {0.0, 1.0, 0.0, 1.0, Color(200, 200, 200), Textura(), BRDF(BRDF_DIFUSO), SIN_LUZ, Color(10, 10, 10)};
  Plano backPlane = {0.0, 0.0, 1.0, 1.0, Color(200, 200, 200)};

  geometrias.push_back(&sphere1);
  geometrias.push_back(&sphere2);
  geometrias.push_back(&sphere3);
  geometrias.push_back(&sphere4);
  geometrias.push_back(&sphere5);
  geometrias.push_back(&sphere6);

  geometrias.push_back(&ceilingPlane);
  geometrias.push_back(&leftPlane);
  geometrias.push_back(&rightPlane);
  geometrias.push_back(&floorPlane);
  geometrias.push_back(&backPlane);

  float fov = 90;
  float apertura = 0.02;
  float dFocal = 1; // Convertir FOV de grados a radianes

  Camara camara = Camara{G_O(0, -0.5, -7, 1), G_O(-1, 0, 0, 0),
                         G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel, fov, dFocal, apertura};
  // camara.rotarCamara(EJE_Z, -8);

  // camara.crearImagenPhotonMapping("./renders/cornellBox", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
  camara.crearImagenPathTracer("./renders/cornellBox", geometrias, tipoColor, SI_MOSTRAR_PROGRESO);
}

void cornellBoxTexturasLUZ(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;
  Textura txLadrillos = cargarTextura("./texturas/txLadrillos.ppm", 1);
  Textura txAgua = cargarTextura("./texturas/txAgua.ppm", 1);
  Textura txMadera = cargarTextura("./texturas/txMadera.ppm", 1);
  Textura txSofa = cargarTextura("./texturas/txSofa.ppm", 1);
  Textura txMapamundi = cargarTextura("./texturas/txMapamundi.ppm", 1);
  Textura txMapaLuna = cargarTextura("./texturas/txMapaLuna.ppm", 1);

  pLuz p1 = pLuz(G_O(0, 0.5, 0, 1), Color(99999, 99999, 99999));
  luces.push_back(&p1);

  Esfera leftSphere = {-0.5, -0.7, 0.25, 0.3, Color(140, 251, 251), txMapaLuna, BRDF(Color(1, 1, 1), Color(), Color(), 0.01, 1)};
  Esfera rightSphere = {0.5, -0.7, -0.25, 0.3, Color(227, 172, 250), Textura(), BRDF(BRDF_REFRACTIVO)};

  Plano leftPlane = {-1.0, 0.0, 0.0, 1.0, Color(255, 0, 0), txLadrillos, BRDF(BRDF_DIFUSO)};
  Plano rightPlane = {1.0, 0.0, 0.0, 1.0, Color(0, 255, 0), txLadrillos, BRDF(BRDF_DIFUSO)};
  Plano floorPlane = {0.0, -1.0, 0.0, 1.0, Color(150, 150, 150), txMadera, BRDF(BRDF_DIFUSO)};
  Plano ceilingPlane = {0.0, 1.0, 0.0, 1.0, Color(150, 150, 150), txAgua, BRDF(BRDF_PLASTICO)};
  Plano backPlane = {0.0, 0.0, 1.0, 1.0, Color(150, 150, 150), Textura(), BRDF(BRDF_ESPECULAR)};

  geometrias.push_back(&rightSphere);
  geometrias.push_back(&leftSphere);

  geometrias.push_back(&ceilingPlane);
  geometrias.push_back(&leftPlane);
  geometrias.push_back(&rightPlane);
  geometrias.push_back(&floorPlane);
  geometrias.push_back(&backPlane);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};

  // camara.crearImagenPhotonMapping("./renders/cornellBox", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
  camara.crearImagenPathTracer("./renders/cornellBoxTextures", geometrias, tipoColor, SI_MOSTRAR_PROGRESO);
}

void cornellBoxModelo3D(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  pLuz p1 = pLuz(G_O(0, 0.5, 0, 1), Color(100, 100, 100));
  luces.push_back(&p1);

  leerPLY("../objetos3D/bugatti_125K", Color(255, 0, 0), false, BRDF(BRDF_DIFUSO), SIN_LUZ, Color());

  Plano leftPlane = {-1.0, 0.0, 0.0, 1.0, Color(255, 0, 0)};
  Plano rightPlane = {1.0, 0.0, 0.0, 1.0, Color(0, 255, 0)};
  Plano floorPlane = {0.0, -1.0, 0.0, 1.0, Color(200, 200, 200)};
  Plano ceilingPlane = {0.0, 1.0, 0.0, 1.0, Color(200, 200, 200), Textura(), BRDF(BRDF_DIFUSO), CON_LUZ_HACIA_FUERA, Color(10, 10, 10)};
  Plano backPlane = {0.0, 0.0, 1.0, 1.0, Color(200, 200, 200)};
  Plano backPlane2 = {0.0, 0.0, -1.0, 1.0, Color(200, 200, 200)};

  geometrias.push_back(&ceilingPlane);
  geometrias.push_back(&leftPlane);
  geometrias.push_back(&rightPlane);
  geometrias.push_back(&floorPlane);
  geometrias.push_back(&backPlane);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};
  camara.crearImagenPathTracer("./renders/cornellBoxBugatti", geometrias, tipoColor, SI_MOSTRAR_PROGRESO);
}

void cornellBoxFiguras(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  pLuz p1 = pLuz(G_O(0, 0.5, 0, 1), Color(10, 10, 10));
  luces.push_back(&p1);

  Cono cono = Cono(G_O(-0.5, -1, 0.5, 1), 0.5, 0.3, Color(55, 255, 155), Textura());
  Toro toro = Toro{G_O(0.0, 1.0, 0.0, 0), G_O(-0.1, -0.95, -0.2, 1), 0.2, 0.05, Color(255, 255, 0)};
  Disco disco = Disco{G_O(-1.0, -1.0, -0.6, 0), G_O(0.5, 0.1, -0.5, 1), 0.4, Color(255, 0, 255), BRDF(BRDF_ESPECULAR)};
  Cilindro cilindro = Cilindro(G_O(-0.8, -1, 0, 1), 0.8, 0.1, Color(0, 255, 255), Textura());
  Elipsoide elipsoide = Elipsoide(G_O(0, -0.5, 0.5, 1), G_O(0.2, 0.4, 0.15, 0), Color(0, 0, 255), Textura());

  geometrias.push_back(&cono);
  geometrias.push_back(&toro);
  geometrias.push_back(&disco);
  geometrias.push_back(&cilindro);
  geometrias.push_back(&elipsoide);

  Esfera rightSphere = {0.5, -0.7, -0.25, 0.3, Color(227, 172, 250), Textura(), BRDF(BRDF_REFRACTIVO)};

  Plano leftPlane = {-1.0, 0.0, 0.0, 1.0, Color(255, 0, 0)};
  Plano rightPlane = {1.0, 0.0, 0.0, 1.0, Color(0, 255, 0)};
  Plano floorPlane = {0.0, -1.0, 0.0, 1.0, Color(200, 200, 200)};
  Plano ceilingPlane = {0.0, 1.0, 0.0, 1.0, Color(200, 200, 200), Textura(), BRDF(BRDF_DIFUSO), SIN_LUZ, Color(10, 10, 10)};
  Plano backPlane = {0.0, 0.0, 1.0, 1.0, Color(200, 200, 200)};
  Plano backPlane2 = {0.0, 0.0, -1.0, 1.0, Color(200, 200, 200)};

  geometrias.push_back(&rightSphere);

  geometrias.push_back(&ceilingPlane);
  geometrias.push_back(&leftPlane);
  geometrias.push_back(&rightPlane);
  geometrias.push_back(&floorPlane);
  geometrias.push_back(&backPlane);
  // geometrias.push_back(&backPlane2);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};
  // Camara camara = {G_O(0, -0.7, -2.5, 1), G_O(-1, 0, 0, 0),
  //                  G_O(0, 1, 0, 0), G_O(0, 0, 1, 0), height, width, rayosPorPixel, 45, 1, 0};
  // camara.rotarCamara(EJE_Z, -8);

  // camara.crearImagenPhotonMapping("./renders/cornellBox", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_NE);
  camara.crearImagenPathTracer("./renders/cornellBox", geometrias, tipoColor, SI_MOSTRAR_PROGRESO);
}

void cornellBoxSombraDura(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  pLuz p1 = pLuz(G_O(0, 0.5, 0, 1), Color(100, 100, 100));
  luces.push_back(&p1);

  Esfera leftSphere = {-0.5, -0.7, 0.25, 0.3, Color(140, 251, 251), Textura(), BRDF(BRDF_DIFUSO)};
  Esfera rightSphere = {0.5, -0.7, -0.25, 0.3, Color(227, 172, 250), Textura(), BRDF(BRDF_DIFUSO)};

  Plano floorPlane = {0.0, -1.0, 0.0, 1.0, Color(200, 200, 200)};

  geometrias.push_back(&rightSphere);
  geometrias.push_back(&leftSphere);

  geometrias.push_back(&floorPlane);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};
  // Camara camara = {G_O(0, -0.7, -2.5, 1), G_O(-1, 0, 0, 0),
  //                  G_O(0, 1, 0, 0), G_O(0, 0, 1, 0), height, width, rayosPorPixel, 45, 1, 0};
  // camara.rotarCamara(EJE_Z, -8);

  // camara.crearImagenPhotonMapping("./renders/cornellBoxShadow", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_NE);
  camara.crearImagenPathTracer("./renders/cornellBoxShadow", geometrias, tipoColor, SI_MOSTRAR_PROGRESO);
}

void verObjeto(string nombre, Camara camara, vector<Geometria *> geometrias, bool normal)
{
  for (int i = 0; i < tri.size(); i++)
  {
    geometrias.push_back(&tri.at(i));
  }

  camara.crearImagenPathTracer(nombre, geometrias, normal, SI_MOSTRAR_PROGRESO);
}

Camara camaraOpt(vector<Triangulo> &triangulos, int height, int width, int rayosPorPixel)
{
  // Supongamos que tienes un vector de triángulos llamado 'triangulos'.
  // Calcula los valores mínimos y máximos de coordenadas.
  G_O minCoord(1e10, 1e10, 1e10, 1);
  G_O maxCoord(-1e10, -1e10, -1e10, 1);

  for (const Triangulo &tri : triangulos)
  {
    // Actualiza los valores mínimos y máximos.
    minCoord.x = min(minCoord.x, min(tri.v0.x, min(tri.v1.x, tri.v2.x)));
    minCoord.y = min(minCoord.y, min(tri.v0.y, min(tri.v1.y, tri.v2.y)));
    minCoord.z = min(minCoord.z, min(tri.v0.z, min(tri.v1.z, tri.v2.z)));

    maxCoord.x = max(maxCoord.x, max(tri.v0.x, max(tri.v1.x, tri.v2.x)));
    maxCoord.y = max(maxCoord.y, max(tri.v0.y, max(tri.v1.y, tri.v2.y)));
    maxCoord.z = max(maxCoord.z, max(tri.v0.z, max(tri.v1.z, tri.v2.z)));
  }
  G_O posicionCamara((maxCoord.x + minCoord.x) / 2, (maxCoord.y + minCoord.y) / 2, minCoord.x * 1.0, 1);
  G_O left(-abs(minCoord.x), 0, 0, 0);
  G_O up(0, abs(minCoord.x), 0, 0);
  G_O forward(0, 0, abs(minCoord.x), 0);

  return {posicionCamara, left, up, forward, height, width, rayosPorPixel};
}

void triangulo(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  Triangulo tg = {G_O(-0.5, -1, 0.0, 1),
                  G_O(0, 1, 0.0, 1),
                  G_O(0.5, 0, 0.0, 1), Color(255, 255, 255)};

  Plano backPlane = {0.0, 0.0, 5.0, 1.0, Color(196, 196, 196)};

  geometrias.push_back(&tg);
  // geometrias.push_back(&backPlane);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 2, 0), height, width, rayosPorPixel};

  camara.crearImagenPhotonMapping("./renders/pruebaTriangulo", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
}

void cono(int tipoColor, int height, int width, int rayosPorPixel)
{
  Textura txMapamundi = cargarTextura("./texturas/txMapamundi.ppm", 1);

  vector<Geometria *> geometrias;

  Cono cono = Cono(G_O(0, -0.5, 0.25, 1), 1.5, 0.8, Color(255, 0, 0), txMapamundi);

  geometrias.push_back(&cono);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};

  camara.crearImagenPhotonMapping("./renders/cono", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
}

void cilindro(int tipoColor, int height, int width, int rayosPorPixel)
{
  Textura txMapamundi = cargarTextura("./texturas/txMapamundi.ppm", 1);

  vector<Geometria *> geometrias;

  Cilindro cilindro = Cilindro(G_O(0, -0.75, 0.25, 1), 1.5, 0.8, Color(255, 0, 0), txMapamundi);

  geometrias.push_back(&cilindro);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};

  camara.crearImagenPhotonMapping("./renders/cilindro", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
}

void elipsoide(int tipoColor, int height, int width, int rayosPorPixel)
{
  Textura txMapamundi = cargarTextura("./texturas/txMapamundi.ppm", 1);

  vector<Geometria *> geometrias;

  Elipsoide elipsoide = Elipsoide(G_O(0, 0, 1, 1), G_O(0.9, 0.4, 0.4, 0), Color(255, 0, 0), txMapamundi);

  geometrias.push_back(&elipsoide);

  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};

  camara.crearImagenPhotonMapping("./renders/elipsoide", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
}

void disco(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  Disco disco = {G_O(-1.0, -1.0, -0.6, 0), G_O(0.0, 0.0, 0.0, 1), 1, Color(255, 0, 0)};

  geometrias.push_back(&disco);
  Camara camara = {G_O(0, 0, -3.5, 1), G_O(-1, 0, 0, 0),
                   G_O(0, 1, 0, 0), G_O(0, 0, 3, 0), height, width, rayosPorPixel};
  camara.crearImagenPhotonMapping("./renders/disco", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
}

void toro(int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  Toro toro = {G_O(0.0, 0.0, 0.0, 0), G_O(0, 0, 0, 1), 0.8, 0.4, Color(255, 0, 0)};

  geometrias.push_back(&toro);

  Camara camara = {G_O(0, 2.5, -2.5, 1), G_O(-0.5, 0, 0, 0),
                   G_O(0, 0.5, 0.5, 0), G_O(0, -1, 1, 0), height, width, rayosPorPixel};

  camara.crearImagenPhotonMapping("./renders/toro", geometrias, tipoColor, SI_MOSTRAR_PROGRESO, DL_USING_PM);
}

void ampliarObjeto(vector<Triangulo> &tri, int escala)
{

  for (size_t i = 0; i < tri.size(); i++)
  {
    tri[i].v0 = tri[i].v0 * escala;
    tri[i].v1 = tri[i].v1 * escala;
    tri[i].v2 = tri[i].v2 * escala;
  }
}

void ModelToPicture(string ficheroEntrada, string ficheroSalida, int tipoColor, int height, int width, int rayosPorPixel)
{
  vector<Geometria *> geometrias;

  leerPLY(ficheroEntrada, Color(255, 0, 0), false, BRDF(BRDF_DIFUSO), SIN_LUZ, Color());
  Camara camara = camaraOpt(tri, height, width, rayosPorPixel);
  camara.crearImagenPathTracer(ficheroSalida, geometrias, tipoColor, SI_MOSTRAR_PROGRESO);
}