#ifndef PUNTOLUZ
#define PUNTOLUZ

#include "geometrias/Geometria.h"
#include "color.h"
#include "geometrias/Triangulo.h"
#include "../p1/G_O.h"

class pLuz
{
public:
  G_O origen;
  Color color;

  pLuz(G_O _origen, Color _color);
  /*
   * calcula si hay una intersección entre un rayo y las geometrías en una escena, 
   * y si la hay, ajusta el color del punto de intersección basado en la iluminación.
   * Itera a través de una lista de geometrías 
   * @return bool: si hay intersección devuelve el color
   */
  bool invInterseccion(Color &_color, G_O pIntermedio, G_O normal, vector<Geometria *> geometrias);

  /*
   * calcula la intersección entre un rayo y los objetos en una escena, y ajusta el 
   * color del punto de intersección basado en la iluminación utilizando IntersectBVH
   * @return bool: si hay intersección devuelve el color
   */
  bool invInterseccion(Color &_color, G_O pIntermedio, G_O normal, int tipoColor);
};
#endif