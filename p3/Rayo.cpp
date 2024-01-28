#include "Rayo.h"

Rayo::Rayo(float _x, float _y, float _z, float dir_x, float dir_y, float dir_z)
{
    punto = G_O(_x, _y, _z, 1);
    direccion = G_O(dir_x, dir_y, dir_z, 0);
}

Rayo::Rayo(G_O _punto, G_O _direccion)
{
    punto = _punto;
    direccion = _direccion;
}

Rayo::Rayo(float _x, float _y, float _z, G_O _direccion)
{
    punto = G_O(_x, _y, _z, 1);
    direccion = _direccion;
}

Rayo::Rayo(G_O _punto, float dir_x, float dir_y, float dir_z)
{
    punto = _punto;
    direccion = G_O(dir_x, dir_y, dir_z, 0);
}