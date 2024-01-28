#ifndef TORO
#define TORO

#include "Geometria.h"
#include "../Rayo.h"
#include "../color.h"
#include "../../p1/G_O.h"

using namespace std;

class Toro : public Geometria
{
public:
    G_O normal, centro;
    float R, r;
    Color color, normalRGB;

    Toro(G_O _normal, G_O _centro, float _R, float _r, Color _color, BRDF _brdf = BRDF(BRDF_DIFUSO));
    Toro(G_O _normal, G_O _centro, float _R, float _r, Color _color, BRDF _brdf, int tipoLuz, Color _potencia);
    bool interseccion(float &t, Color &_color, const Rayo rayo) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal) override;
};

#endif
