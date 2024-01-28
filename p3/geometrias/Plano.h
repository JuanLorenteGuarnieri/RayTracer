#ifndef PLANO
#define PLANO

#include "Geometria.h"
#include "../Rayo.h"
#include "../color.h"
#include "../Textura.h"
#include "../../p1/G_O.h"

using namespace std;

class Plano : public Geometria
{
public:
    G_O normal;
    float distancia;
    Color color, normalRGB;

    Textura tx;

    Plano(float normal_x, float normal_y, float normal_z, float _distancia, Color _color, Textura _tx = Textura(), BRDF _brdf = BRDF(BRDF_DIFUSO));
    Plano(float normal_x, float normal_y, float normal_z, float _distancia, Color _color, Textura _tx, BRDF _brdf, int tipoLuz, Color _potencia);
    bool interseccion(float &t, Color &_color, const Rayo rayo) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal) override;
};

#endif
