#include "BRDF.h"

/*
  Constructor de la BDRF, ya tiene en cuenta ya tiene en cuenta los distintos
  coeficientes para objetos difusos(kd), especulares(ks) y reflexivos(kt), con
  sus correspondientes probabilidades, teniendo en cuenta que (pd + ps + pt) <= 1,
  en caso de que sea menor que 1 lo que queda lo absorvera (pa -> el rayo no continuara)
  los coeficientes son objetos de tipo Color y no tienen porque estar normalizados ni nada

*/
BRDF::BRDF(Color _kd, Color _ks, Color _kt, float _pa, float _ind)
{
  kd = _kd;
  ks = _ks;
  kt = _kt;
  pa = _pa;
  calculaPorcentaje();
  ind = _ind;
}

BRDF::BRDF(int tipo)
{
  ind = 1;
  switch (tipo)
  {
  case BRDF_DIFUSO:
    kd = Color(1.0, 1.0, 1.0);
    ks = Color(0.0, 0.0, 0.0);
    kt = Color(0.0, 0.0, 0.0);
    pa = 0.1;
    break;
  case BRDF_ESPECULAR:
    kd = Color(0.0, 0.0, 0.0);
    ks = Color(1.0, 1.0, 1.0);
    kt = Color(0.0, 0.0, 0.0);
    pa = 0.1;
    break;
    // case BRDF_REFRACTIVO:
    //   kd = Color(0.0, 0.0, 0.0);
    //   ks = Color(0.05, 0.05, 0.05);
    //   kt = Color(0.95, 0.95, 0.95);
    //   pa = 0.001f;
    //   ind = 1.5f;
    //   break;
  case BRDF_REFRACTIVO:
    kd = Color(0.0, 0.0, 0.0);
    ks = Color(0.0, 0.0, 0.0);
    kt = Color(1.0, 1.0, 1.0);
    pa = 0.1;
    ind = 1.5f;
    break;
  case BRDF_PLASTICO:
    kd = Color(0.6, 0.6, 0.6);
    ks = Color(0.4, 0.4, 0.4);
    kt = Color(0.0, 0.0, 0.0);
    pa = 0.1;
    break;
  case BRDF_METAL:
    kd = Color(0.4, 0.4, 0.4);
    ks = Color(0.6, 0.6, 0.6);
    kt = Color(0.0, 0.0, 0.0);
    pa = 0.1;
    break;
  default:
    break;
  }
  calculaPorcentaje();
}

void BRDF::calculaPorcentaje()
{
  /**
   * LOS PORCENTAJES TIENE QUE SER ALEATORIO
   *
   */
  Color Cpd = kd / (kd + ks + kt);
  Color Cps = ks / (kd + ks + kt);
  Color Cpt = kt / (kd + ks + kt);

  pd = Cpd.maxi();
  ps = Cps.maxi();
  pt = Cpt.maxi();
}
/*
  Devuelve el tipo de rebote/BDSF que se tiene que calcular
*/
int BRDF::Roulette()
{
  float p = (rand() % (int)((1 + pa) * 10000)) / 10000.0f;

  if (p < pd)
  {
    return BRDF_DIFUSO;
  }
  else if (p >= pd && p < pd + ps)
  {
    return BRDF_ESPECULAR;
  }
  else if (p >= pd + ps && p < pd + ps + pt)
  {
    return BRDF_REFRACTIVO;
  }
  else
  {
    return BRDF_NADA;
  }
}