#include "luzGlobal.h"

extern int indTriangulo;
extern TriTextura triTxt;
extern vector<pLuz *> luces;

extern Color shade(vector<Geometria *> geometrias, G_O &intersection, Rayo &rayo, float &_t, G_O &normal, bool isDelta, BRDF brdf, Color objetoColor, const PhotonMap &photon_map, float radio, int nPhotons);
extern PhotonMap map;

void intersectGeometrias(vector<Geometria *> geometrias, Rayo rayo, int tipoColor, float &t, BRDF &brdf, bool &inter, G_O &normalObject, Color &objetoColor, int &tipoLuz, Color &potencia)
{
  for (Geometria *geometria : geometrias)
  {
    float t_temp = 1e12;
    Color color_temp;
    G_O norObjAux;

    if (geometria->interseccion(t_temp, color_temp, rayo, tipoColor, norObjAux))
    {
      if (t_temp < t)
      {
        brdf = geometria->brdf;
        inter = true;
        normalObject = norObjAux;
        t = t_temp;
        objetoColor = color_temp;
        tipoLuz = geometria->luz;
        potencia = geometria->potencia;
      }
    }
  }
}

/**
 * Realiza la intersección entre un rayo y un cuadro delimitado por su caja de límites (AABB).
 *
 * @param ray El rayo con el que se verifica la intersección.
 * @param bmin El vértice mínimo de la caja de límites (AABB).
 * @param bmax El vértice máximo de la caja de límites (AABB).
 * @param t El valor de distancia a lo largo del rayo en el que se realiza la verificación.
 * @return Devuelve verdadero si el rayo intersecta la caja de límites (AABB), falso en caso contrario.
 */
float IntersectAABB(const Rayo &ray, const G_O bmin, const G_O bmax, const float t)
{
  float tx1 = (bmin.x - ray.punto.x) / ray.direccion.x, tx2 = (bmax.x - ray.punto.x) / ray.direccion.x;
  float tmin = min(tx1, tx2), tmax = max(tx1, tx2);
  float ty1 = (bmin.y - ray.punto.y) / ray.direccion.y, ty2 = (bmax.y - ray.punto.y) / ray.direccion.y;
  tmin = max(tmin, min(ty1, ty2)), tmax = min(tmax, max(ty1, ty2));
  float tz1 = (bmin.z - ray.punto.z) / ray.direccion.z, tz2 = (bmax.z - ray.punto.z) / ray.direccion.z;
  tmin = max(tmin, min(tz1, tz2)), tmax = min(tmax, max(tz1, tz2));
  if (tmax >= tmin && tmin < t && tmax > 0)
    return tmin;
  else
    return 1e30f;
}

bool IntersectBVH(Rayo &ray, const int nodeIdx, const int tipoColor, Color &objetoColor, float &t, G_O &normalObject, BRDF &brdf, int &tipoLuz, Color &potencia)
{
  if (tri.empty())
    return false;
  BVHNode &node = bvhNode[nodeIdx];
  if (IntersectAABB(ray, node.aabbMin, node.aabbMax, t) == 1e30f)
    return false;
  if (node.isLeaf())
  {
    bool aux = false;
    for (int i = 0; i < node.primCount; i++)
    {

      float t_temp = 1e11;
      Color color_temp;
      int indTrian = node.firstPrim + i;
      G_O norObjAux;

      if (tri[indTrian].interseccion(t_temp, color_temp, ray, tipoColor, norObjAux, indTrian))
      {
        // cout << "Choque soy el  rayo " << k << " del pixel (" << i << ", " << j << ") -> rgb(" << color_temp.r << ", " << color_temp.g << ", " << color_temp.b << ")" << endl;
        if (t_temp < t)
        {
          brdf = tri[indTrian].brdf;
          aux = true;
          normalObject = norObjAux;
          t = t_temp;
          objetoColor = color_temp;
          tipoLuz = tri[indTrian].luz;
          potencia = tri[indTrian].potencia;
        }
      }
    }
    return aux;
  }
  else
  {
    bool aux1, aux2;
    aux1 = IntersectBVH(ray, node.leftChild, tipoColor, objetoColor, t, normalObject, brdf, tipoLuz, potencia);
    aux2 = IntersectBVH(ray, node.leftChild + 1, tipoColor, objetoColor, t, normalObject, brdf, tipoLuz, potencia);
    return aux1 || aux2;
  }
}

G_O calcularNormalCorrecta(G_O normal, G_O punto, G_O direccion)
{
  G_O normal2 = normal.escalar(-1);
  direccion = direccion.escalar(-1);
  direccion.normalize();
  G_O p1 = punto + direccion * 5;
  G_O p2 = punto + normal * 5;
  G_O p3 = punto + normal2 * 5;
  p2 = p2 - p1;
  p3 = p3 - p1;
  float dist1 = p2.modulo();
  float dist2 = p3.modulo();
  if (dist1 < dist2)
  {
    return normal;
  }
  else
  {
    return normal2;
  }
}

G_O calcularNormalCorrectaRefraccion(G_O normal, G_O punto, G_O direccion)
{
  G_O normal2 = normal.escalar(-1);
  G_O p1 = punto + direccion;
  G_O p2 = punto + normal;
  G_O p3 = punto + normal2;
  p2 = p2 - p1;
  p3 = p3 - p1;
  float dist1 = p2.modulo();
  float dist2 = p3.modulo();
  if (dist1 < dist2)
  {
    return normal;
  }
  else
  {
    return normal2;
  }
}

Color calcularBRDF(int tipoRebote, BRDF brdf)
{
  Color brdf_color;
  if (tipoRebote == BRDF_DIFUSO)
  {
    brdf_color = (brdf.kd / (brdf.pd));
  }
  else if (tipoRebote == BRDF_ESPECULAR)
  {
    brdf_color = (brdf.ks / brdf.ps);
  }
  else if (tipoRebote == BRDF_REFRACTIVO)
  {
    brdf_color = (brdf.kt / brdf.pt);
  }
  return brdf_color;
}

/**
 * Calcula la iluminación directa en un punto de intersección con la escena.
 *
 * @param rayo            El rayo utilizado para calcular la intersección.
 * @param tipoColor       El tipo de color que se está calculando.
 * @param normalObject    La normal del objeto en el punto de intersección.
 * @param colorSalida     El color resultante de la iluminación directa.
 * @param t               La distancia desde el origen del rayo al punto de intersección.
 * @param geometrias      La lista de geometrías en la escena.
 */
void luzDirecta(Rayo rayo, G_O normalObject, Color &colorSalida, G_O p, vector<Geometria *> geometrias)
{
  Color cAux, c;
  Rayo _rayo = rayo;

  for (pLuz *luz : luces)
  {
    if (luz->invInterseccion(cAux, p, normalObject, geometrias))
    {
      G_O luzDireccion = luz->origen - p;
      float dist = luzDireccion.modulo();
      luzDireccion.normalize();
      float aux = abs(G_O::dot(luzDireccion, normalObject));
      c = c + ((cAux)*aux);
    }
  }
  colorSalida = c;
}

void luzDirectaGeneral(int tipoColor, G_O normalObject, Color &colorSalida, G_O p, vector<Geometria *> geometrias)
{
  Color cAux, cAux2, c;
  bool noChocaTriangulo = false;

  for (pLuz *luz : luces)
  {
    bool noChocaObjeto = luz->invInterseccion(cAux, p, normalObject, geometrias);
    if (tri.size() != 0)
      noChocaTriangulo = luz->invInterseccion(cAux2, p, normalObject, tipoColor);
    else
      cAux2 = cAux;

    if (noChocaObjeto || noChocaTriangulo)
      c = c + cAux2;
  }
  colorSalida = c;
}

/**
 * Calcula la iluminación directa en un punto de intersección con la escena usando BVH (Bounding Volume Hierarchy).
 *
 * @param rayo            El rayo utilizado para calcular la intersección.
 * @param tipoColor       El tipo de color que se está calculando.
 * @param normalObject    La normal del objeto en el punto de intersección.
 * @param colorSalida     El color resultante de la iluminación directa.
 * @param t               La distancia desde el origen del rayo al punto de intersección.
 */
void luzDirectaBVH(Rayo rayo, int tipoColor, G_O normalObject, Color &colorSalida, G_O p)
{
  if (tipoColor == COLOR_CON_LUCES)
  {
    Color cAux, c;

    for (pLuz *luz : luces)
    {
      if (luz->invInterseccion(cAux, p, normalObject, tipoColor))
      {
        G_O luzDireccion = luz->origen - p;
        float dist = luzDireccion.modulo();
        luzDireccion.normalize();
        float aux = abs(G_O::dot(luzDireccion, normalObject));
        c = c + ((cAux)*aux);
      }
    }
    colorSalida = c;
  }
}

Rayo refraccion(G_O &_normalObject, G_O p, Rayo _rayo, BRDF brdf)
{
  Rayo rayo = _rayo;
  G_O normal = calcularNormalCorrecta(_normalObject, p, _rayo.direccion);

  float n1 = 1.0; // Índice de refracción del medio original
  float n2 = 1.0; // Índice de refracción del nuevo medio
  if (normal != _normalObject)
    n2 = brdf.ind;
  else
    n1 = brdf.ind;
  float eta = n1 / n2;

  float cos_theta = G_O::dot(_rayo.direccion, normal);
  float theta = acos(cos_theta);
  float sin_theta2 = eta * sin(theta);
  float theta2 = asin(sin_theta2);
  float cos_theta2 = cos(theta2);

  // Calcular la dirección del rayo reflejado utilizando un modelo de reflexión difusa
  G_O aux = G_O(sin_theta2, 0, -cos_theta2, 0);
  aux.normalize();
  _normalObject = calcularNormalCorrecta(_normalObject, p, _rayo.direccion);

  G_O v = _rayo.direccion * _normalObject;
  v.normalize();
  G_O u = _normalObject * v;
  u.normalize();
  u = calcularNormalCorrectaRefraccion(u, p, _rayo.direccion);

  Base T = {u.x, u.y, u.z, v.x, v.y, v.z, normal.x, normal.y, normal.z, p.x, p.y, p.z, CAMBIO_BASE};
  G_O wi = aux.cambio_base(T);
  wi.normalize();
  rayo.direccion = wi;

  rayo.punto = p;
  return rayo;
}

Rayo refleccion(G_O &_normalObject, G_O p, Rayo _rayo)
{
  _normalObject = calcularNormalCorrecta(_normalObject, p, _rayo.direccion);

  Rayo rayo = _rayo;
  rayo.direccion = _rayo.direccion - (_normalObject * 2.0 * G_O::dot(_rayo.direccion, _normalObject));
  rayo.direccion.normalize();
  rayo.punto = p;
  return rayo;
}

void fresnel(G_O I, G_O N, G_O p, float ior, float &kr)
{
  G_O normal = calcularNormalCorrecta(N, p, I);

  float n1 = 1.0; // Índice de refracción del medio original
  float n2 = 1.0; // Índice de refracción del nuevo medio
  if (normal != N)
    n2 = ior;
  else
    n1 = ior;
  float eta = n1 / n2;

  float cos_theta = G_O::dot(I, normal);
  if (cos_theta > 0)
  {
    std::swap(n1, n2);
  }
  float theta = acos(cos_theta);
  float sin_theta2 = eta * sin(theta);
  float theta2 = asin(sin_theta2);
  float cos_theta2 = cos(theta2);
  if (sin_theta2 > 1)
  {
    kr = 1;
  }
  else
  {
    cos_theta = abs(cos_theta);
    float Rs = ((n2 * cos_theta) - (n1 * cos_theta2)) / ((n2 * cos_theta) + (n1 * cos_theta2));
    float Rp = ((n1 * cos_theta) - (n2 * cos_theta2)) / ((n1 * cos_theta) + (n2 * cos_theta2));
    kr = ((Rs * Rs) + (Rp * Rp)) / 2;
  }
}

Rayo diffusion(G_O &_normalObject, G_O p, Rayo _rayo)
{
  Rayo rayo = _rayo;
  float e_inclinacion = (rand() % 1000) / 1000.0f;
  float e_azimutal = (rand() % 1000) / 1000.0f;

  float inclinacion = acos(sqrt(1 - e_inclinacion));
  float azimutal = 2 * PI * e_azimutal;

  G_O aux = G_O(sin(inclinacion) * cos(azimutal), sin(inclinacion) * sin(azimutal), cos(inclinacion), 0);

  _normalObject = calcularNormalCorrecta(_normalObject, p, _rayo.direccion);

  G_O u = _rayo.direccion * _normalObject;
  u.normalize();
  G_O v = _normalObject * u;
  v.normalize();

  Base T = {u.x, u.y, u.z, v.x, v.y, v.z, _normalObject.x, _normalObject.y, _normalObject.z, p.x, p.y, p.z, CAMBIO_BASE};
  G_O wi = aux.cambio_base(T);
  wi.normalize();
  rayo.direccion = wi;
  rayo.punto = p;

  return rayo;
}

/**
 * Calcula el rebote de un rayo en una superficie dada.
 *
 * @param _rayo El rayo incidente.
 * @param _normalObject La normal de la superficie de intersección.
 * @param _t La distancia desde el punto de origen del rayo al punto de intersección.
 * @param N El número de rebotes restantes (controla la recursión).
 * @param bdrf El BDRF del objeto con el que el rayo ha colisionado
 * @param tipoRebote Tipo de rebote
 *
 * @return Un nuevo rayo que representa la dirección y el punto de origen del rayo reflejado.
 */
Rayo calcularRebote(Rayo _rayo, G_O &_normalObject, float _t, BRDF &brdf, int &tipoRebote)
{
  G_O p = _rayo.punto + _rayo.direccion * _t;
  tipoRebote = brdf.Roulette();
  if (tipoRebote == BRDF_DIFUSO)
  {
    return diffusion(_normalObject, p, _rayo);
  }
  else if (tipoRebote == BRDF_ESPECULAR)
  {
    return refleccion(_normalObject, p, _rayo);
  }
  else if (tipoRebote == BRDF_REFRACTIVO)
  {
    float kr = 0;
    fresnel(_rayo.direccion, _normalObject, p, brdf.ind, kr);
    brdf.kt = Color(1 - kr, 1 - kr, 1 - kr);
    brdf.ks = Color(kr, kr, kr);
    brdf.calculaPorcentaje();
    float pr = (rand() % 10000) / 10000.0f;
    // kr = 0;
    if (pr < kr)
    {
      return refleccion(_normalObject, p, _rayo);
    }
    else
    {
      return refraccion(_normalObject, p, _rayo, brdf);
    }
  }
  else if (tipoRebote == BRDF_NADA)
  {
    return refleccion(_normalObject, p, _rayo);
  }
  return _rayo;
}

/**
 * Calcula la luz global en una escena recursivamente utilizando rebotes de rayos.
 *
 * @param geometrias Un vector de geometrías en la escena.
 * @param _rayo El rayo incidente.
 * @param _tipoColor El tipo de color que se quiere calcular.
 * @param _normalObject  La normal del objeto con el que ha colisionado el rayo.
 * @param _colorSalida El color de salida.
 * @param brdf El modelo de BRDF del objeto con el que ha colisionado el rayo.
 * @param brdf_mul: El factor de reflexión difusa acumulado.
 * @param _t La distancia desde el punto de origen del rayo al punto de intersección.
 * @param N El número de rebotes actuales (controla la recursión).
 * @param N_MAX El número máximo de rebotes permitidos.
 */
void luzGlobal(vector<Geometria *> geometrias, Rayo _rayo, int _tipoColor, G_O _normalObject, Color &_colorSalida, BRDF brdf, Color brdf_mul, float _t, int N, const int &N_MAX, bool typeDirectLight)
{
  if (N <= N_MAX)
  {

    Color objetoColor, colorLuzDirecta, pot;
    int tipoLuz = SIN_LUZ;
    bool inter = false;
    float t = 1e11;

    // Iterar a través de las geometrías y encontrar la intersección más cercana
    intersectGeometrias(geometrias, _rayo, _tipoColor, t, brdf, inter, _normalObject, objetoColor, tipoLuz, pot);

    if (inter)
    {
      int tipoRebote;

      Rayo rayo = calcularRebote(_rayo, _normalObject, t, brdf, tipoRebote);

      objetoColor.normalize();
      G_O p = _rayo.punto + _rayo.direccion * t;
      if (_tipoColor == COLOR_CON_LUCES)
      {

        Color brdf_color = calcularBRDF(tipoRebote, brdf);
        bool cond = (tipoRebote == BRDF_ESPECULAR) || (tipoRebote == BRDF_REFRACTIVO);
        Rayo rayoAux = rayo;
        G_O normalAux = _normalObject;
        float tAux = t;
        G_O pAux = p;
        Color objetoColorAux = objetoColor;
        Color photonMapping = shade(geometrias, pAux, rayoAux, tAux, normalAux, cond, brdf, objetoColorAux, map, RADIO_BUSQUEDA, N_BUSCAR_PHOTONS);
        if (tipoRebote == BRDF_DIFUSO)
        {
          _colorSalida = _colorSalida + (photonMapping * brdf_color * brdf_mul * objetoColor * brdf_color);
        } // else no se añade nada porque se trata de un espejo o de un cristal
        else
        {
          _colorSalida = _colorSalida + (photonMapping * brdf_color * brdf_mul * brdf_color);
          if (typeDirectLight == DL_USING_NE)
          {
            luzDirecta(rayoAux, normalAux, colorLuzDirecta, pAux, geometrias);
            _colorSalida = _colorSalida + (colorLuzDirecta * brdf_color * PI * brdf_mul);
          }
        }

        if (tipoRebote == BRDF_DIFUSO)
        {
          if (typeDirectLight == DL_USING_NE)
          {
            luzDirecta(_rayo, _normalObject, colorLuzDirecta, p, geometrias);
            _colorSalida = _colorSalida + (colorLuzDirecta * brdf_color * PI * brdf_mul * objetoColor);
          }
          brdf_mul = brdf_mul * brdf_color * objetoColor * abs(G_O::dot(rayo.direccion, _normalObject));
        }
        else
        {
          brdf_mul = brdf_mul * brdf_color;
        }
        if (false && tipoRebote != BRDF_NADA && brdf_mul.maxi() > 1e-4)
        {
          luzGlobal(geometrias, rayo, _tipoColor, _normalObject, _colorSalida, brdf, brdf_mul, _t, N + 1, N_MAX, typeDirectLight);
        }
      }
      else
      {
        _colorSalida = objetoColor;
      }
    }
  }
}

void photonMapping(vector<Geometria *> geometrias, Rayo _rayo, int _tipoColor, G_O _normalObject, Color &_colorSalida, BRDF brdf, Color brdf_mul, float _t, int N, const int &N_MAX, bool typeDirectLight)
{
  if (N <= N_MAX)
  {
    bool inter = false, inter2 = false;
    Color objetoColor, objetoColor2, colorLuzDirecta, pot, pot2;
    BRDF brdf2 = BRDF(BRDF_DIFUSO);
    G_O _normalObject2;
    float t = 1e11, t2 = 1e11;
    int tipoLuz = SIN_LUZ, tipoLuz2 = SIN_LUZ;

    // Iterar a través de las geometrías y encontrar la intersección más cercana
    intersectGeometrias(geometrias, _rayo, _tipoColor, t, brdf, inter, _normalObject, objetoColor, tipoLuz, pot);

    // Iterar a través de las geometrías y encontrar la intersección más cercana
    inter2 = IntersectBVH(_rayo, 0, _tipoColor, objetoColor2, t2, _normalObject2, brdf2, tipoLuz2, pot2);

    if (inter || inter2)
    {
      if (t > t2)
      {
        brdf = brdf2;
        t = t2;
        _normalObject = _normalObject2;
        objetoColor = objetoColor2;
        tipoLuz = tipoLuz2;
        pot = pot2;
      }
      int tipoRebote;

      if (tipoLuz != SIN_LUZ)
      {
        _colorSalida = _colorSalida + pot;
      }
      else
      {
        Rayo rayo = calcularRebote(_rayo, _normalObject, t, brdf, tipoRebote);

        objetoColor.normalize();
        G_O p = _rayo.punto + _rayo.direccion * t;
        if (_tipoColor == COLOR_CON_LUCES)
        {

          Color brdf_color = calcularBRDF(tipoRebote, brdf);
          bool cond = (tipoRebote == BRDF_ESPECULAR) || (tipoRebote == BRDF_REFRACTIVO);
          Rayo rayoAux = rayo;
          G_O normalAux = _normalObject;
          float tAux = t;
          G_O pAux = p;
          Color objetoColorAux = objetoColor;
          Color photonMapping = shade(geometrias, pAux, rayoAux, tAux, normalAux, cond, brdf, objetoColorAux, map, RADIO_BUSQUEDA, N_BUSCAR_PHOTONS);
          if (tipoRebote == BRDF_DIFUSO)
          {
            _colorSalida = _colorSalida + (photonMapping * brdf_color * brdf_mul * objetoColor * brdf_color);
          } // else no se añade nada porque se trata de un espejo o de un cristal
          else
          {
            _colorSalida = _colorSalida + (photonMapping * brdf_color * brdf_mul * brdf_color);
            if (typeDirectLight == DL_USING_NE)
            {
              luzDirectaGeneral(_tipoColor, normalAux, colorLuzDirecta, pAux, geometrias);
              _colorSalida = _colorSalida + (colorLuzDirecta * brdf_color * PI * brdf_mul);
            }
          }

          if (tipoRebote == BRDF_DIFUSO)
          {
            if (typeDirectLight == DL_USING_NE)
            {
              luzDirectaGeneral(_tipoColor, _normalObject, colorLuzDirecta, p, geometrias);
              _colorSalida = _colorSalida + (colorLuzDirecta * brdf_color * PI * brdf_mul * objetoColor);
            }
            brdf_mul = brdf_mul * brdf_color * objetoColor * abs(G_O::dot(rayo.direccion, _normalObject));
          }
        }
        else
        {
          _colorSalida = objetoColor;
        }
      }
    }
  }
}

void luzGlobalPhoton(vector<Geometria *> geometrias, Rayo &_rayo, int _tipoColor, G_O &_normalObject, Color &_objetoColor, Color &_colorSalida, BRDF &brdf, float &_t, int N, bool &isDelta, bool &seguir)
{
  Rayo rayo = _rayo;
  bool inter = false, inter2 = false;
  Color objetoColor, objetoColor2, colorLuzDirecta, pot, pot2;
  BRDF brdf2 = BRDF(BRDF_DIFUSO);
  G_O _normalObject2;
  float t = 1e11, t2 = 1e11;
  int tipoLuz = SIN_LUZ, tipoLuz2 = SIN_LUZ;

  // Iterar a través de las geometrías y encontrar la intersección más cercana
  intersectGeometrias(geometrias, _rayo, _tipoColor, _t, brdf, inter, _normalObject, _objetoColor, tipoLuz, pot);

  // Iterar a través de las geometrías y encontrar la intersección más cercana
  inter2 = IntersectBVH(_rayo, 0, _tipoColor, objetoColor2, t2, _normalObject2, brdf2, tipoLuz2, pot2);

  if (inter || inter2)
  {
    if (_t > t2)
    {
      brdf = brdf2;
      _t = t2;
      _normalObject = _normalObject2;
      _objetoColor = objetoColor2;
      tipoLuz = tipoLuz2;
      pot = pot2;
    }
    if (tipoLuz != SIN_LUZ)
    {
      seguir = false;
    }
    else
    {
      int tipoRebote;
      rayo = calcularRebote(_rayo, _normalObject, _t, brdf, tipoRebote);
      _objetoColor.normalize();

      if (((tipoRebote == BRDF_ESPECULAR) || (tipoRebote == BRDF_REFRACTIVO)))
      {
        isDelta = true;
      }
      if (tipoRebote == BRDF_NADA)
      {
        seguir = false;
      }

      _rayo.direccion.normalize();
      _normalObject.normalize();
      Color brdf_color = calcularBRDF(tipoRebote, brdf);

      if (tipoRebote != BRDF_DIFUSO)
      {
        _objetoColor = Color(1, 1, 1);
      }
      if (tipoRebote == BRDF_DIFUSO)
      {
        _colorSalida = (_colorSalida * _objetoColor * brdf_color * abs(G_O::dot(_rayo.direccion, _normalObject)));
      }
    }
  }
  else
  {
    seguir = false;
  }

  _rayo = rayo;
}

void trace_outgoing_ray(vector<Geometria *> geometrias, Rayo &_rayo, int _tipoColor, G_O &_normalObject, float &_t, int &N, bool &isDelta, bool &seguir, Color &objetoColor)
{

  if (N > 10)
  {
    seguir = false;
    return;
  }
  isDelta = false;

  Rayo rayo = _rayo;
  bool inter = false, inter2 = false;
  Color objetoColor2, colorLuzDirecta, pot, pot2;
  BRDF brdf2 = BRDF(BRDF_DIFUSO);
  G_O _normalObject2;
  float t = 1e11, t2 = 1e11;
  BRDF brdf(BRDF_NADA);
  int tipoLuz = SIN_LUZ, tipoLuz2 = SIN_LUZ;

  // Iterar a través de las geometrías y encontrar la intersección más cercana
  intersectGeometrias(geometrias, _rayo, _tipoColor, _t, brdf, inter, _normalObject, objetoColor, tipoLuz, pot);

  // Iterar a través de las geometrías y encontrar la intersección más cercana
  inter2 = IntersectBVH(_rayo, 0, _tipoColor, objetoColor2, t2, _normalObject2, brdf2, tipoLuz2, pot2);

  if (inter || inter2)
  {
    if (_t > t2)
    {
      brdf = brdf2;
      _t = t2;
      _normalObject = _normalObject2;
      objetoColor = objetoColor2;
      tipoLuz = tipoLuz2;
      pot = pot2;
    }
    if (tipoLuz != SIN_LUZ)
    {
      seguir = false;
    }
    else
    {
      int tipoRebote;
      rayo = calcularRebote(_rayo, _normalObject, _t, brdf, tipoRebote);
      objetoColor.normalize();
      if ((tipoRebote == BRDF_ESPECULAR) || (tipoRebote == BRDF_REFRACTIVO))
      {
        isDelta = true;
      }
      if (tipoRebote == BRDF_NADA)
      {
        seguir = false;
      }
    }
  }
  else
  {
    seguir = false;
  }

  _rayo = rayo;
  N++;
}

/**
 * Calcula la luz global en una escena utilizando rebotes de rayos con la estructura de datos BVH.
 *
 * @param _rayo El rayo incidente.
 * @param _tipoColor El tipo de cálculo de color a realizar.
 * @param _normalObject La normal de la superficie de intersección.
 * @param _objetoColor El color del objeto en la superficie de intersección.
 * @param _colorSalida El color de salida acumulado.
 * @param brdf El modelo de BRDF del objeto con el que ha colisionado el rayo.
 * @param brdf_mul: El factor de reflexión difusa acumulado.
 * @param _t La distancia desde el punto de origen del rayo al punto de intersección.
 * @param N El número de rebotes actuales (controla la recursión).
 * @param N_MAX El número máximo de rebotes permitidos.
 */
void luzGlobalBVH(Rayo _rayo, int _tipoColor, G_O _normalObject, Color &_colorSalida, BRDF brdf, Color brdf_mul, int N, int N_MAX)
{
  if (N <= N_MAX)
  {

    Color objetoColor, colorLuzDirecta, pot;

    bool inter = false;
    float t = 1e11;
    int tipoLuz = SIN_LUZ;

    // Iterar a través de las geometrías y encontrar la intersección más cercana
    inter = IntersectBVH(_rayo, 0, _tipoColor, objetoColor, t, _normalObject, brdf, tipoLuz, pot);

    if (inter)
    {
      if (tipoLuz != SIN_LUZ)
      {
        _colorSalida = _colorSalida + objetoColor;
      }
      else
      {
        int tipoRebote;
        Rayo rayo = calcularRebote(_rayo, _normalObject, t, brdf, tipoRebote);
        objetoColor.normalize();
        G_O p = _rayo.punto + _rayo.direccion * t;
        if (_tipoColor == COLOR_CON_LUCES)
        {
          Color brdf_color = calcularBRDF(tipoRebote, brdf);

          if (tipoRebote == BRDF_DIFUSO)
          {
            luzDirectaBVH(_rayo, _tipoColor, _normalObject, colorLuzDirecta, p);

            _colorSalida = _colorSalida + (colorLuzDirecta * brdf_color * PI * brdf_mul * objetoColor);

            brdf_mul = brdf_mul * brdf_color * objetoColor * abs(G_O::dot(rayo.direccion, _normalObject));
          }
          else
          {
            brdf_mul = brdf_mul * brdf_color;
          }
          if (tipoRebote != BRDF_NADA && brdf_mul.maxi() > 1e-4)
          {
            luzGlobalBVH(rayo, _tipoColor, _normalObject, _colorSalida, brdf, brdf_mul, N + 1, N_MAX);
          }
        }
        else
        {
          _colorSalida = objetoColor;
        }
      }
    }
  }
}

void pathTracer(vector<Geometria *> geometrias, Rayo _rayo, int _tipoColor, G_O _normalObject, Color &_colorSalida, BRDF brdf, Color brdf_mul)
{
  bool inter = false, inter2 = false;
  Color objetoColor, objetoColor2, colorLuzDirecta, pot, pot2;
  BRDF brdf2 = BRDF(BRDF_DIFUSO);
  G_O _normalObject2;
  float t = 1e11, t2 = 1e11;
  int tipoLuz = SIN_LUZ, tipoLuz2 = SIN_LUZ;

  // Iterar a través de las geometrías y encontrar la intersección más cercana
  intersectGeometrias(geometrias, _rayo, _tipoColor, t, brdf, inter, _normalObject, objetoColor, tipoLuz, pot);

  // Iterar a través de las geometrías y encontrar la intersección más cercana

  inter2 = IntersectBVH(_rayo, 0, _tipoColor, objetoColor2, t2, _normalObject2, brdf2, tipoLuz2, pot2);

  if (inter || inter2)
  {
    if (t > t2)
    {
      brdf = brdf2;
      t = t2;
      _normalObject = _normalObject2;
      objetoColor = objetoColor2;
      tipoLuz = tipoLuz2;
      pot = pot2;
    }

    int tipoRebote;
    Rayo rayo = calcularRebote(_rayo, _normalObject, t, brdf, tipoRebote);
    Color objetoColorAux = objetoColor;
    objetoColor.normalize();
    G_O p = _rayo.punto + _rayo.direccion * t;
    if (_tipoColor == COLOR_CON_LUCES)
    {
      Color brdf_color = calcularBRDF(tipoRebote, brdf);

      if (tipoRebote == BRDF_DIFUSO)
      {
        if (tipoLuz != SIN_LUZ)
        {
          _colorSalida = _colorSalida + objetoColorAux * PI * brdf_mul;
        }else
        {
          luzDirectaGeneral(_tipoColor, _normalObject, colorLuzDirecta, p, geometrias);

        _colorSalida = _colorSalida + (colorLuzDirecta * brdf_color * PI * brdf_mul * objetoColor);
        
        }
        
        brdf_mul = brdf_mul * brdf_color * objetoColor * abs(G_O::dot(rayo.direccion, _normalObject));
      }
      else
      {
        brdf_mul = brdf_mul * brdf_color;
      }
      if (tipoRebote != BRDF_NADA && brdf_mul.maxi() > 1e-6 && tipoLuz == SIN_LUZ)
      {
        pathTracer(geometrias, rayo, _tipoColor, _normalObject, _colorSalida, brdf, brdf_mul);
      }
    }
    else
    {
      _colorSalida = objetoColor;
    }
  }
}
