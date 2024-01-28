#ifndef BRDF_H
#define BRDF_H

#include "constantes.h"
#include "color.h"
#include "../p1/G_O.h"

const int BRDF_DIFUSO = 1;
const int BRDF_ESPECULAR = 2;
const int BRDF_REFRACTIVO = 3;
const int BRDF_NADA = 4;
const int BRDF_PLASTICO = 5;
const int BRDF_METAL = 6;
const int BRDF_MADERA = 7;

class BRDF
{
public:
  Color kd, ks, kt;
  float pd, ps, pt, pa;
  float ind;
  BRDF(Color _kd, Color _ks, Color _kt, float _pa, float _ind);
  BRDF(int tipo);
  int Roulette();
  void calculaPorcentaje();
};

#endif