#ifndef GEOMETRIA
#define GEOMETRIA

#include "../../p1/G_O.h"
#include "../color.h"
#include "../Rayo.h"
#include "../BRDF.h"

using namespace std;

class Geometria
{
public:
  BRDF brdf = BRDF(BRDF_DIFUSO);
  int luz = SIN_LUZ;
  Color potencia;
  G_O calcularVectorPerpendicular(const G_O &vectorOriginal);
  virtual bool interseccion(float &t, Color &_color, const Rayo rayo);
  virtual bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor);
  virtual bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal);
};

#endif
