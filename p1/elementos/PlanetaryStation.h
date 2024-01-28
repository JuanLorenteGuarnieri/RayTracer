#ifndef PLANETARY_STATION
#define PLANETARY_STATION

using namespace std;
#include "../G_O.h"
#include "Planet.h"
#include "../Base.h"
#include <iostream>
#include <cmath>

class PlanetaryStation
{
public:
    G_O position; // Posición de la estación en UCS
    G_O i;        // Dirección de tangente a la longitud (i)
    G_O j;        // Dirección de tangente a la latitud (j)
    G_O k;        // Superficie normal del planeta (k) (módulo es 1)

    /**
     * Inclnación: Ánuglo respecto a los ejes del planetas.         Radianes    (0, π)
     * Azimuth: Ángulo alrededor el globo respecto al 0-meridiano.  Radianes    (−π, π]
     *
     */
    PlanetaryStation(Planet &planet, float inclination, float azimuth);
};

#endif