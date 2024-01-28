#include "G_O.h"
#include "Base.h"
#include "test/GeometricalObjectsTest.h"
#include "elementos/PlanetaryStation.h"
#include "elementos/Planet.h"
/*
Compilar:
        g++ -std=c++11 G_O.cpp Base.cpp main.cpp test/GeometricalObjectsTest.cpp elementos/Planet.cpp elementos/PlanetaryStation.cpp -o main
Ejecutar:
        ./main
*/

int main()
{
        // GeometricalObjectsTest::RunAllTests();
        Planet tierra(G_O(0, 0, 0, 0), G_O(1, 1, 1, 1), G_O(100, 100, 100, 0));
        PlanetaryStation station(tierra, 10, 20);
        return 0;
}
