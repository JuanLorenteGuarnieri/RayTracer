#include "PlanetaryStation.h"

PlanetaryStation::PlanetaryStation(Planet& planet, float inclination, float azimuth) {
        // Calcular la posición de la estación en UCS a partir de la inclinación y el azimut
        float radius = (planet.axis - planet.reference_city).modulo();
        float x = radius * sin(inclination) * cos(azimuth);
        float y = radius * sin(inclination) * sin(azimuth);
        float z = radius * cos(inclination);
        position.change_value(x, y, z, 0);

        Base azimuth_ = Base(azimuth, EJE_Z, ROTACION);
        Base inclinationX = Base(inclination, EJE_X, ROTACION);
        Base inclinationY = Base(inclination, EJE_Y, ROTACION);

        i = planet.axis.rotacion(azimuth_);
        j = i.rotacion(inclinationX);
        k = position - planet.center;
        k = k.escalar(1.0 / k.modulo());

        cout << "posición: " << position <<  "\nlongitude: " << i << "\nlatitude: " << j << "\nnormal: " << k << endl;

    }