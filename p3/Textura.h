#ifndef TEXTURA
#define TEXTURA

#include <vector>
#include <string>
#include "../p2/pixel.h"

using namespace std;

class Textura
{
public:
  int width, height;
  float achoTextura;
  vector<Pixel> pixeles;

  Textura(int _width, int _height, vector<Pixel> _pixeles, float _achoTextura)
  {
    width = _width;
    height = _height;
    pixeles = _pixeles;
    achoTextura = _achoTextura;
  };

  Textura()
  {
    width = 0;
    height = 0;
    vector<Pixel> p;
    pixeles = p;
    achoTextura = -1;
  };
};
class TriTxt
{
public:
  int nFichero;
  float Tv1x, Tv1y, Tv2x, Tv2y, Tv3x, Tv3y;

  TriTxt(float _Tv1x, float _Tv1y, float _Tv2x, float _Tv2y, float _Tv3x, float _Tv3y, int _nFichero)
  {
    Tv1x = _Tv1x;
    Tv1y = _Tv1y;
    Tv2x = _Tv2x;
    Tv2y = _Tv2y;
    Tv3x = _Tv3x;
    Tv3y = _Tv3y;
    nFichero = _nFichero;
  };
};

class TriTextura
{
public:
  vector<Textura> txt;
  vector<TriTxt> tri;
  vector<string> ficheros;

  TriTextura(vector<Textura> _txt, vector<TriTxt> _tri, vector<string> _ficheros)
  {
    txt = _txt;
    tri = _tri;
    ficheros = _ficheros;
  };
  TriTextura()
  {
    vector<Textura> _txt;
    vector<TriTxt> _tri;
    vector<string> _ficheros;

    txt = _txt;
    tri = _tri;
    ficheros = _ficheros;
  };
};

#endif
