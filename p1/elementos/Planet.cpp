#include "Planet.h"

Planet::Planet(const Geometrical_objects& centerObj, const Geometrical_objects& axisObj, const Geometrical_objects& cityObj){
    float axisRadius = axis.modulo();
    float radioAxis = axisRadius / 2;
    float centerToCityRadius = (center - reference_city).modulo();

    double diferencia = abs(radioAxis - centerToCityRadius);
    if ( diferencia <= 1e-6){
        center = centerObj;
        axis = axisObj;
        reference_city = cityObj;
        cout << "La distancia obtenida es " << diferencia << endl;
    } else{
        cout << "La distancia obtenida es mayor a 10^-6 " << diferencia << endl;
    }
    cout << "centro: " << center << "\naxis: " << axis << "\ncity: " << reference_city << endl;
}