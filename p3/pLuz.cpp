#include "pLuz.h"

extern bool IntersectBVH(Rayo &ray, const int nodeIdx, const int tipoColor, Color &objetoColor, float &t, G_O &normalObject, BRDF &brdf, int &tipoLuz, Color &potencia);

pLuz::pLuz(G_O _origen, Color _color)
{

  origen = _origen;
  color = _color;
}

bool pLuz::invInterseccion(Color &_color, G_O pIntermedio, G_O normal, vector<Geometria *> geometrias)
{
  G_O dir = pIntermedio - origen;
  Rayo rayo(origen, dir);

  // Realizar la intersección del rayo con la escena y calcular el color
  // Aquí deberías implementar la lógica de intersección y color de tu escena
  float t1 = 1e11;

  // Iterar a través de las geometrías y encontrar la intersección más cercana
  for (Geometria *geometria : geometrias)
  {
    float t_temp = 1e11;
    Color color_temp;

    if (geometria->interseccion(t_temp, color_temp, rayo, 3))
    {
      // cout << "Choque soy el  rayo " << k << " del pixel (" << i << ", " << j << ") -> rgb(" << color_temp.r << ", " << color_temp.g << ", " << color_temp.b << ")" << endl;
      if (t_temp < t1)
      {
        t1 = t_temp;
      }
    }
  }
  G_O p = rayo.punto + rayo.direccion * t1;
  G_O dir2 = p - origen;

  float mod = dir.modulo();
  float mod2 = dir2.modulo();

  if (abs(mod2 - mod) > 0.005)
  {
    return false;
  }
  else
  {
    float mod = dir.modulo();
    dir.normalize();
    float a = abs(G_O::dot(normal, dir));

    _color = color * (a / (mod * mod));
    return true;
  }
}

bool pLuz::invInterseccion(Color &_color, G_O pIntermedio, G_O normal, int tipoColor)
{
  G_O dir = pIntermedio - origen;
  Rayo rayo(origen, dir);

  // Realizar la intersección del rayo con la escena y calcular el color
  // Aquí deberías implementar la lógica de intersección y color de tu escena
  float t1 = 1e11;
  Color color_temp;
  G_O _nor;
  BRDF brdf = BRDF(BRDF_DIFUSO);
  int tipoLuz = SIN_LUZ;
  Color pot;
  bool inter = IntersectBVH(rayo, 0, 0, color_temp, t1, _nor, brdf, tipoLuz, pot);
  if (!inter)
  {
    float mod = dir.modulo();
    dir.normalize();
    float a = abs(G_O::dot(normal, dir));

    _color = color * (a / (mod * mod));
    return true;
  }

  G_O p = rayo.punto + rayo.direccion * t1;
  G_O dir2 = p - origen;

  float mod = dir.modulo();
  float mod2 = dir2.modulo();

  if (abs(mod2 - mod) > 0.005)
    return false;
  else
  {
    float mod = dir.modulo();
    dir.normalize();
    float a = abs(G_O::dot(normal, dir));

    _color = color * (a / (mod * mod));
    return true;
  }
}
