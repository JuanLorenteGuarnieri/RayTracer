#ifndef CONO
#define CONO

#include "Geometria.h"
#include "../Rayo.h"
#include "../color.h"
#include "../Textura.h"
#include "../../p1/G_O.h"

using namespace std;

class Cono : public Geometria
{
public:
    G_O centro;
    float altura, radio;
    Color color;

    Textura tx;

    Cono(G_O _centro, float _altura, float _radio, Color _color, Textura _tx = Textura(), BRDF _brdf = BRDF(BRDF_DIFUSO));
    Cono(G_O _centro, float _altura, float _radio, Color _color, Textura _tx, BRDF _brdf, int tipoLuz, Color _potencia);
    bool interseccion(float &t, Color &_color, const Rayo rayo) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal) override;
};

#endif
