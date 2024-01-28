#ifndef DISCO
#define DISCO

#include "Geometria.h"
#include "../Rayo.h"
#include "../color.h"
#include "../Textura.h"
#include "../../p1/G_O.h"

using namespace std;

class Disco : public Geometria
{
public:
    G_O normal, punto;
    float radio;
    Color color, normalRGB;

    Disco(G_O _normal, G_O _punto, float _radio, Color _color, BRDF _brdf = BRDF(BRDF_DIFUSO));
    Disco(G_O _normal, G_O _punto, float _radio, Color _color, BRDF _brdf, int tipoLuz, Color _potencia);
    bool interseccion(float &t, Color &_color, const Rayo rayo) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal) override;
};

#endif
