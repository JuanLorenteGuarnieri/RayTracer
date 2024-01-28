#ifndef ELIPSOIDE
#define ELIPSOIDE

#include "Geometria.h"
#include "../Rayo.h"
#include "../color.h"
#include "../Textura.h"
#include "../../p1/G_O.h"
#include "../constantes.h"

using namespace std;

class Elipsoide : public Geometria
{
public:
    G_O centro, radio;
    Color color;

    Textura tx;

    Elipsoide(G_O _centro, G_O _dir, Color _color, Textura _tx = Textura(), BRDF _brdf = BRDF(BRDF_DIFUSO));
    Elipsoide(G_O _centro, G_O _radio, Color _color, Textura _tx, BRDF _brdf, int tipoLuz, Color _potencia);
    bool interseccion(float &t, Color &_color, const Rayo rayo) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal) override;
};

#endif
