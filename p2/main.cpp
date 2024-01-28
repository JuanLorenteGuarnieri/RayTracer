#include "tone_mapping.h"
#include "ficheros.h"
#include <iostream>

using namespace std;

/*
    g++ -std=c++11 ficheros.cpp tone_mapping.cpp main.cpp -o main
*/

int main()
{
    string formato = "";
    int width, height, resolucion;
    double maximo;
    vector<Pixel> pixeles;

    leerFichero("images/mpi_office.ppm", formato, width, height, maximo, resolucion, pixeles);

    vector<Pixel> pixeles_rgb = conversion_HDR_RGB(pixeles, maximo, resolucion);
    // vector<Pixel> res = clamping(pixeles_rgb, maximo * 1 / 5, maximo);
    // vector<Pixel> res = ecualizacion(pixeles_rgb, maximo);
    // vector<Pixel> res = ecualizacion_campling(pixeles_rgb, maximo * 1 / 2, maximo);
    // vector<Pixel> res = curva_gamma(pixeles_rgb, 3, maximo);
    // vector<Pixel> res = clamp_curva_gamma(pixeles_rgb, 3, maximo * 1 / 5, maximo);
    // vector<Pixel> res = filtroGaussiano(pixeles_rgb, width, height);
    // vector<Pixel> res = color_to_grey(pixeles_rgb);
    vector<Pixel> res = Sobel(pixeles_rgb, width, height);

    // guardarImagenPPM("output2", res, formato, width, height, maximo, resolucion);
    guardarImagenBMP("output2", res, width, height, maximo);
    return 0;
}