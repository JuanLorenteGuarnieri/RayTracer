#ifndef PLANET
#define PLANET

using namespace std;
#include "../G_O.h"
#include <iostream>
#include <cmath>

class Planet
{
public:
    G_O center;
    G_O axis;
    G_O reference_city;
    /*
     * centerObj: vector representa la posición del centro del planeta
     * axisObj: La dirección que conecta el Polo Sur con el Polo Norte del planeta.
     *          La magnitud (módulo) de este vector debe ser el doble del radio del planeta
     * cityObj: posición en el espacio de la ciudad de referencia del planeta desde la cual se mide el azimut (longitud)
     *          distancia entre esta ciudad de referencia y el centro del planeta es igual al radio del planeta.
     *
     */
    Planet(const G_O &centerObj, const G_O &axisObj, const G_O &cityObj);
};

#endif