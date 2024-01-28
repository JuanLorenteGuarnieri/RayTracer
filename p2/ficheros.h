#ifndef FICHEROS_H
#define FICHEROS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "pixel.h"
#include <cstring>
#include <cerrno>
using namespace std;

void leerFichero(string nombreFichero, string &formato, int &width, int &height, double &maximo, int &resolucion, vector<Pixel> &pixeles);
void guardarImagenPPM(string ficheroDestino, vector<Pixel> &pixeles, string formato, int width, int height);
void guardarImagenPPMHDR(string ficheroDestino, vector<Pixel> &pixeles, string formato, int width, int height);
void guardarImagenBMP(const string &ficheroDestino, const vector<Pixel> &pixeles, int width, int height, double maximo);
float pixelMax(vector<Pixel> &pixeles);

#endif // FICHEROS_HPP
