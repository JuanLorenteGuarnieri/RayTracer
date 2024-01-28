#ifndef TIEMPO_H
#define TIEMPO_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string obtenerTiempo(int segundos);
void mostrarBarraProgreso(int progress, int duration, int estimated_seconds_remaining);


#endif