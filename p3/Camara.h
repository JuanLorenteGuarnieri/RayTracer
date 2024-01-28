#ifndef CAMARA
#define CAMARA

#include "geometrias/Geometria.h"
#include "Rayo.h"
#include "pLuz.h"
#include "geometrias/Triangulo.h"
#include "color.h"
#include "photon.h"
#include "./utils/luzGlobal.h"
#include "photon_mapping.h"
#include "BRDF.h"
#include "../p2/pixel.h"
#include "./utils/tiempo.h"
#include "./utils/monitor.h"
#include "../p1/G_O.h"
#include "../p2/tone_mapping.h"
#include "../p1/Base.h"
#include "../p2/ficheros.h"
#include <chrono>
#include <cstdlib>
#include <thread>
#include <cmath>
#include "constantes.h"
#include "BVH/BVH.h"
#include "BVH/BVHNode.h"

using namespace chrono;
using namespace std;

extern vector<Triangulo> tri;
extern vector<BVHNode> bvhNode;

class Camara
{
private:
  /**
   * Realiza la paralelización para renderizar una escena a través de la cámara.
   *
   * @param geometrias Un vector de punteros a objetos de geometría en la escena.
   * @param tipoColor El tipo de cálculo de color a realizar.
   * @param i El índice de fila del píxel en la imagen.
   * @param pixeles Un vector que almacena los píxeles de la imagen resultante.
   */
  void paralelizacionPhotonMapping(vector<Geometria *> &geometrias, int tipoColor, int i, vector<Pixel> &pixeles, Monitor &monitor, bool typeDirectLight);

  void paralelizacionPathTracer(vector<Geometria *> geometrias, int tipoColor, int i, vector<Pixel> &pixeles, Monitor &monitor);

public:
  G_O origin;
  G_O left;
  G_O up;
  G_O forward;
  int height;
  int width;
  int rayosPorPixel;
  float fov;            // Campo de Visión (en grados)
  float distanciaFocal; // Distancia de enfoque
  float apertura;       // Tamaño de la apertura para el desenfoque

  Camara(G_O _origin, G_O _left, G_O _up, G_O _forward, int _h, int _w, int _rayosPorPixel);
  Camara(G_O _origin, G_O _left, G_O _up, G_O _forward, int _h, int _w, int _rayosPorPixel, float _fov, float _dFocal, float _apertura);

  void rotarCamara(int eje, float angulo);

  G_O muestrearDisco(float radio);

  G_O crearRayoCamara(const int &i, const int &j);

  /**
   * Crea y renderiza una imagen de la escena a través de la cámara, y la guarda en un archivo.
   *
   * @param ficheroDestino El nombre del archivo de destino donde se guardará la imagen.
   * @param geometrias Un vector de punteros a objetos de geometría en la escena.
   * @param tipoColor El tipo de cálculo de color a realizar.
   * @param mostrarProgreso Un indicador booleano para mostrar o no el progreso del renderizado.
   */
  void crearImagenPhotonMapping(string ficheroDestino, vector<Geometria *> geometrias, int tipoColor, bool mostrarProgreso, bool typeDirectLight);

  void crearImagenPathTracer(const string ficheroDestino, vector<Geometria *> geometrias, int tipoColor, bool mostrarProgreso);
};

#endif