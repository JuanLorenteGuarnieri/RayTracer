#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include "pixel.h"
#include <math.h>

using namespace std;

/*
 * Clamping:
 * Acotar todos los valores superiores a valor_clamping (1 en coma flotante).
 */
vector<Pixel> clamping(vector<Pixel> &inicial, double valor_clamping, double &maximo);

/*
 * Ecualización:
 * Transformación lineal de los valores desde el mínimo hasta el máximo (normalización)
 */
vector<Pixel> ecualizacion(const vector<Pixel> &imagen, double &maximo);

/*
 * Ecualización y clamp:
 * Combinar los dos anteriores según un parámetro de ”clamping.” Nótese que los dos operadores
 * anteriores pueden considerarse casos particulares de este operador.
 */
vector<Pixel> ecualizacion_campling(const vector<Pixel> &imagen, double valor_clamping, double &maximo);

/*
 * Curva gamma:
 * Aplicar una curva gamma a todos los valores (necesita ecualización primero).
 */
vector<Pixel> curva_gamma(const vector<Pixel> &imagen, double curva_gamma, double &maximo);

/*
 * Clamp y curva gamma:
 * Aplicar una curva gamma después de una operación de clamping
 */
vector<Pixel> clamp_curva_gamma(const vector<Pixel> &imagen, double curva_gamma, double valor_clamping, double &maximo);


vector<Pixel> filtroGaussiano(const vector<Pixel> &imagen, int ancho, int alto);

vector<Pixel> color_to_grey(const vector<Pixel> &imagenRGB);

vector<Pixel> Sobel(const vector<Pixel> &imagen, int ancho, int alto);

/*
 * s: valor de trileta RGB
 * c: resolución
 * s / c
 * conversion_HDR_RGB
 */
vector<Pixel> conversion_HDR_RGB(const vector<Pixel> &imagenHDR, double maximo, int resolucion);

vector<Pixel> conversion_RGB_HDR(const vector<Pixel> &imagenRGB, double maximo, int resolucion);
