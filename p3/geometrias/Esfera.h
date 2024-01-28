#ifndef ESFERA
#define ESFERA

#include "Geometria.h"
#include "../Rayo.h"
#include "../color.h"
#include "../Textura.h"
#include "../../p1/G_O.h"
#include "../constantes.h"

using namespace std;

class Esfera : public Geometria
{
public:
    G_O centro;
    float radio;
    Color color;

    Textura tx;

    Esfera(float _x, float _y, float _z, float _radio, Color _color, Textura _tx = Textura(), BRDF _brdf = BRDF(BRDF_DIFUSO));
    Esfera(float _x, float _y, float _z, float _radio, Color _color, Textura _tx, BRDF _brdf, int tipoLuz, Color _potencia);
    bool interseccion(float &t, Color &_color, const Rayo rayo) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor) override;
    bool interseccion(float &t, Color &_color, Rayo &rayo, G_O &puntoIntersecta);
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal) override;
};

#endif
