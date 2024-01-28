#ifndef ESCENAS
#define ESCENAS

#include "Rayo.h"
#include "geometrias/Esfera.h"
#include "geometrias/Plano.h"
#include "geometrias/Triangulo.h"
#include "geometrias/Cono.h"
#include "geometrias/Cilindro.h"
#include "geometrias/Disco.h"
#include "geometrias/Elipsoide.h"
#include "geometrias/Toro.h"
#include "Camara.h"
#include "pLuz.h"
#include "Textura.h"
#include "color.h"
#include "../p1/G_O.h"
#include "../p2/tone_mapping.h"
#include "../p2/ficheros.h"
#include "constantes.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Vertex
{
  float x, y, z;
};

Textura cargarTextura(string ficheroEntrada, float achoTextura);
void leerPLY(string ficheroEntrada, Color color, bool textura, BRDF brdf, int tipoLuz, Color potencia);
void cornellBox(int tipoColor, int height, int width, int rayosPorPixel);
void cornellBoxLUZ(int tipoColor, int height, int width, int rayosPorPixel);
void cornellBoxTexturasLUZ(int tipoColor, int height, int width, int rayosPorPixel);
void verObjeto(string nombre, Camara camara, vector<Geometria *> geometrias, bool normal);
Camara camaraOpt(vector<Triangulo> &triangulos, int height, int width, int rayosPorPixel);
void triangulo(int tipoColor, int height, int width, int rayosPorPixel);
void cono(int tipoColor, int height, int width, int rayosPorPixel);
void cilindro(int tipoColor, int height, int width, int rayosPorPixel);
void elipsoide(int tipoColor, int height, int width, int rayosPorPixel);
void disco(int tipoColor, int height, int width, int rayosPorPixel);
void toro(int tipoColor, int height, int width, int rayosPorPixel);
void ampliarObjeto(vector<Triangulo> &tri, int escala);
void ModelToPicture(string ficheroEntrada, string ficheroSalida, int tipoColor, int height, int width, int rayosPorPixel);

void cornellBoxModelo3D(int tipoColor, int height, int width, int rayosPorPixel);
void cornellBoxFiguras(int tipoColor, int height, int width, int rayosPorPixel);
void cornellBoxSombraDura(int tipoColor, int height, int width, int rayosPorPixel);
void cornellBoxCamaraOptions(int tipoColor, int height, int width, int rayosPorPixel);

#endif