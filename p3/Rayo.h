#ifndef RAYO
#define RAYO

#include "../p1/G_O.h"

using namespace std;

class Rayo
{
public:
    G_O punto;
    G_O direccion;
    G_O rDireccion;
    Rayo(float _x, float _y, float _z, float dir_x, float dir_y, float dir_z);
    Rayo(G_O _punto, G_O _direccion);
    Rayo(float _x, float _y, float _z, G_O _direccion);
    Rayo(G_O _punto, float dir_x, float dir_y, float dir_z);
};

#endif