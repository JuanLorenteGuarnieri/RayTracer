#ifndef TRIANGULO
#define TRIANGULO

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cerrno>
#include "Geometria.h"
#include "../color.h"
#include "../Textura.h"
#include "../../p1/G_O.h"
#include <algorithm>

using namespace std;

class Triangulo : public Geometria
{
public:
    G_O v0, v1, v2, centroide;
    Color color;

    Triangulo(G_O _v1, G_O _v2, G_O _v3, Color _color, BRDF _brdf = BRDF(BRDF_DIFUSO));
    Triangulo(G_O _v1, G_O _v2, G_O _v3, Color _color, BRDF _brdf, int tipoLuz, Color _potencia);
    bool interseccion(float &t, Color &_color, const Rayo rayo) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor) override;
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal, const int in);
    bool interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal) override;
};

#endif
