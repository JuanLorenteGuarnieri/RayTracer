#include "Camara.h"

extern int indTriangulo;
extern TriTextura triTxt;
extern vector<pLuz *> luces;

vector<Triangulo> tri;
vector<BVHNode> bvhNode;
PhotonMap map;

Camara::Camara(G_O _origin, G_O _left, G_O _up, G_O _forward, int _h, int _w, int _rayosPorPixel)
{
  origin = _origin;
  left = _left;
  up = _up;
  forward = _forward;
  height = _h;
  width = _w;
  rayosPorPixel = _rayosPorPixel;
  fov = 90;
  distanciaFocal = -1;
  apertura = 0;
}

Camara::Camara(G_O _origin, G_O _left, G_O _up, G_O _forward, int _h, int _w, int _rayosPorPixel, float _fov, float _dFocal, float _apertura)
{
  origin = _origin;
  left = _left;
  up = _up;
  forward = _forward;
  height = _h;
  width = _w;
  rayosPorPixel = _rayosPorPixel;
  fov = _fov;
  distanciaFocal = _dFocal;
  apertura = _apertura;
}

void Camara::rotarCamara(int eje, float angulo)
{
  float theta = angulo * PI / 180;
  Base rotacion = Base(theta, eje, ROTACION);
  forward = forward.rotacion(rotacion);
  left = left.rotacion(rotacion);
  up = up.rotacion(rotacion);
}

G_O Camara::muestrearDisco(float radio)
{
  G_O punto;
  do
  {
    // Genera un punto aleatorio en el cuadrado [-radio, radio] x [-radio, radio]
    punto = G_O(rand() / (float)RAND_MAX * 2 * radio - radio,
                rand() / (float)RAND_MAX * 2 * radio - radio,
                0, 1);
  } while (punto.modulo() * punto.modulo() >= radio * radio); // Comprueba si el punto está dentro del disco

  return punto;
}

inline G_O random_in_unit_disk()
{
  while (true)
  {
    auto p = G_O(((rand() / (float)RAND_MAX) * 2) - 1, ((rand() / (float)RAND_MAX) * 2) - 1, 0, 1);
    if (p.modulo() * p.modulo() < 1)
      return p;
  }
}

G_O defocus_disk_sample(G_O origin, float apertura, float u, float v)
{
  // Returns a random point in the camera defocus disk.
  auto p = random_in_unit_disk();
  p.x = p.x * (u * apertura);
  p.y = p.y * (v * apertura);
  return origin + p;
}

G_O Camara::crearRayoCamara(const int &i, const int &j)
{
  float u = (j + (rand() / (RAND_MAX + 1.0))) / width;
  float v = (i + (rand() / (RAND_MAX + 1.0))) / height;

  float distanciaFoca = width / (2.0 * tan(fov * PI / 360.0));

  // Cálculo de la dirección del rayo con FOV
  float aspectRatio = width / static_cast<float>(height);
  float scale = tan(fov * PI / 360.0); // Convertir FOV a radianes y dividir por 2
  G_O direccionRayo = (forward + (left * (aspectRatio * scale * (2 * u - 1))) + (up * (scale * (2 * v - 1))));
  direccionRayo.normalize();

  // // Punto de enfoque
  G_O puntoEnfoque = origin + direccionRayo * distanciaFocal;

  // // Cálculo del nuevo origen del rayo basado en la apertura
  // G_O desviacionApertura = muestrearDisco(apertura);
  // G_O nuevoOrigen = origin + desviacionApertura;

  // // Ajustar la dirección del rayo para pasar por el punto de enfoque
  // direccionRayo = (puntoEnfoque - nuevoOrigen);
  // direccionRayo.normalize();

  auto ray_origin = (apertura <= 0) ? origin : defocus_disk_sample(origin, apertura, u, v);
  direccionRayo = (puntoEnfoque - ray_origin);
  direccionRayo.normalize();
  return direccionRayo;
}

void Camara::crearImagenPathTracer(const string ficheroDestino, vector<Geometria *> geometrias, int tipoColor, bool mostrarProgreso)
{
  BVH node;
  if (!tri.empty())
  {
    node.rangoValores();
  }
  vector<Pixel> pixeles;
  int totalPixels = width * height;
  int completedPixels = 0;
  int previousProgress = -1; // Inicializado a un valor que no es un múltiplo de 5
  bool ponerTextura = (tipoColor == COLOR_CON_TEXTURA || tipoColor == COLOR_CON_LUCES);
  for (int i = 0; i < totalPixels; i++)
  {
    pixeles.push_back(Pixel());
  }

  if (!tri.empty())
  {
    cout << "\n🌳 PROCESANDO BVH ";
    bvhNode = node.BuildBVH(ponerTextura);
    cout << "COMPLETADO 🌳\n";
  }

  cout << "\n💻 COMENZANDO RENDERIZADO 💻\n";

  // Inicio del cronómetro
  auto start_time = high_resolution_clock::now();
  thread threads[height];
  Monitor monitor(totalPixels, 0, mostrarProgreso, start_time);
  for (int i = height - 1; i >= 0; i--)
  {
    threads[i] = thread(&Camara::paralelizacionPathTracer, this, ref(geometrias), tipoColor, i, ref(pixeles), ref(monitor));
  }

  for (int i = height - 1; i >= 0; i--)
  {
    threads[i].join();
  }
  // Fin del cronómetro
  auto end_time = high_resolution_clock::now();
  auto total_duration = duration_cast<seconds>(end_time - start_time).count();
  cout << "\n🏁 Proceso finalizado en " << total_duration << " segundos 🏁\n\n";

  // guardarImagenBMP(ficheroDestino, pixeles, width, height, 255);
  double maximo = pixelMax(pixeles);
  guardarImagenPPMHDR(ficheroDestino + "HDR", pixeles, "P3", width, height);
  vector<Pixel> res = curva_gamma(pixeles, 2.2, maximo);
  guardarImagenPPM(ficheroDestino, res, "P3", width, height);
}

void Camara::paralelizacionPathTracer(vector<Geometria *> geometrias, int tipoColor, int i, vector<Pixel> &pixeles, Monitor &monitor)
{
  srand(i);
  for (int j = width - 1; j >= 0; j--)
  {
    Pixel colorPixel;
    colorPixel.r = 0;
    colorPixel.g = 0;
    colorPixel.b = 0;
    BRDF brdf = BRDF(BRDF_DIFUSO);

    // Bucle para trazar múltiples rayos por cada píxel (anti-aliasing)
    for (int k = 0; k < rayosPorPixel; k++)
    {
      // float u = (j + (rand() / (RAND_MAX + 1.0))) / width;
      // float v = (i + (rand() / (RAND_MAX + 1.0))) / height;

      // // Calcular la dirección del rayo
      // G_O direccionRayo = forward + left * (2 * u - 1) + up * (2 * v - 1);
      G_O direccionRayo = crearRayoCamara(i, j);

      Rayo rayo(origin, direccionRayo);

      Color objetoColor;
      G_O normalObject;

      pathTracer(geometrias, rayo, tipoColor, normalObject, objetoColor, brdf, Color(1, 1, 1));

      colorPixel.r += objetoColor.r / rayosPorPixel;
      colorPixel.g += objetoColor.g / rayosPorPixel;
      colorPixel.b += objetoColor.b / rayosPorPixel;
    }
    pixeles[(height - i - 1) * width + (width - j - 1)] = colorPixel;
    monitor.increaseN();
  }
}

void Camara::crearImagenPhotonMapping(string ficheroDestino, vector<Geometria *> geometrias, int tipoColor, bool mostrarProgreso, bool typeDirectLight)
{

  int totalPixels = width * height;
  vector<Pixel> pixeles;
  int completedPixels = 0;
  int previousProgress = -1; // Inicializado a un valor que no es un múltiplo de 5

  for (int i = 0; i < totalPixels; i++)
  {
    pixeles.push_back(Pixel());
  }
  cout << "☀️ PRE-PROCESANDO PHOTON MAPPING ";
  map = preprocess(geometrias, N_PHOTONS, MAX_SHOTS, typeDirectLight);
  cout << "COMPLETADO ☀️\n";

  cout << "\n💻 COMENZANDO RENDERIZADO 💻\n";
  // Inicio del cronómetro
  auto start_time = high_resolution_clock::now();
  thread threads[height];
  Monitor monitor(totalPixels, 0, mostrarProgreso, start_time);

  for (int i = height - 1; i >= 0; i--)
  {
    threads[i] = thread(&Camara::paralelizacionPhotonMapping, this, ref(geometrias), tipoColor, i, ref(pixeles), ref(monitor), typeDirectLight);
    // threads[i] = thread(&paralelizacion, this, ref(geometrias), tipoColor, i, ref(pixeles));
  }

  for (int i = height - 1; i >= 0; i--)
  {
    threads[i].join();
  }

  // Fin del cronómetro
  auto end_time = high_resolution_clock::now();
  auto total_duration = duration_cast<seconds>(end_time - start_time).count();
  cout << "\n🏁 Render finalizado en " << total_duration << " segundos 🏁\n\n";

  // guardarImagenBMP(ficheroDestino, pixeles, width, height, 255);
  double maximo = pixelMax(pixeles);
  guardarImagenPPMHDR(ficheroDestino + "HDR", pixeles, "P3", width, height);
  vector<Pixel> res = curva_gamma(pixeles, 2.2, maximo);
  guardarImagenPPM(ficheroDestino, res, "P3", width, height);
}

void Camara::paralelizacionPhotonMapping(vector<Geometria *> &geometrias, int tipoColor, int i, vector<Pixel> &pixeles, Monitor &monitor, bool typeDirectLight)
{
  srand(i);
  // j = columna
  for (int j = width - 1; j >= 0; j--)
  {
    Pixel colorPixel;
    colorPixel.r = 0;
    colorPixel.g = 0;
    colorPixel.b = 0;

    // Bucle para trazar múltiples rayos por cada píxel (anti-aliasing)
    for (int k = 0; k < rayosPorPixel; k++)
    {
      // h = coordenada horizontal del píxel
      float h = (j + (rand() / (RAND_MAX + 1.0))) / width;
      // v = coordenada vertical del píxel
      float v = (i + (rand() / (RAND_MAX + 1.0))) / height;

      // Calcular la dirección del rayo
      G_O direccionRayo = forward + left * (2 * h - 1) + up * (2 * v - 1);

      direccionRayo.normalize();
      Rayo rayo(origin, direccionRayo);
      Color colorSalida;
      float t = 1e11;
      Color brdf_mul = Color(1, 1, 1);
      G_O normal;
      BRDF brdf = BRDF(BRDF_DIFUSO);

      photonMapping(geometrias, rayo, tipoColor, normal, colorSalida, brdf, brdf_mul, t, 0, 10, typeDirectLight);

      colorPixel.r += colorSalida.r / rayosPorPixel;
      colorPixel.g += colorSalida.g / rayosPorPixel;
      colorPixel.b += colorSalida.b / rayosPorPixel;
    }

    pixeles[(height - i - 1) * width + (width - j - 1)] = colorPixel;
    monitor.increaseN();
  }
}