#ifndef LUZ_GLOBAL
#define LUZ_GLOBAL

#include "../geometrias/Geometria.h"
#include "../Rayo.h"
#include "../pLuz.h"
#include "../geometrias/Triangulo.h"
#include "../color.h"
#include "../photon.h"
#include "../KdTree.h"
#include "../BRDF.h"
#include "../../p2/pixel.h"
#include "tiempo.h"
#include "../../p1/G_O.h"
#include "../../p2/tone_mapping.h"
#include "../../p1/Base.h"
#include "../../p2/ficheros.h"
#include <chrono>
#include <cstdlib>
#include <thread>
#include <cmath>
#include <algorithm>
#include "../constantes.h"
#include "../BVH/BVH.h"
#include "../BVH/BVHNode.h"

extern vector<Triangulo> tri;
extern vector<BVHNode> bvhNode;

/*
    An additional struct that allows the KD-Tree to access your photon position
*/
struct PhotonAxisPosition
{
  float operator()(const Photon &p, std::size_t i) const
  {
    return p.position(i);
  }
};

/*
    The KD-Tree ready to work in 3 dimensions, with YourPhoton s, under a
    brand-new name: PhotonMap
*/
using PhotonMap = nn::KDTree<Photon, 3, PhotonAxisPosition>;

/**
 * Realiza la intersección de un rayo con una estructura de árbol de caja delimitada jerárquica (BVH).
 *
 * @param ray El rayo con el que se verifica la intersección.
 * @param nodeIdx El índice del nodo actual en la estructura BVH.
 * @param tipoColor El tipo de color para la intersección.
 * @param objetoColor El color resultante de la intersección.
 * @param t El valor de distancia a lo largo del rayo en el que se realiza la verificación.
 * @param normalObject El vector normal del objeto intersectado.
 */
bool IntersectBVH(Rayo &ray, const int nodeIdx, const int tipoColor, Color &objetoColor, float &t, G_O &normalObject, BRDF &brdf, int &tipoLuz, Color &potencia);

void luzGlobal(vector<Geometria *> geometrias, Rayo _rayo, int _tipoColor, G_O _normalObject, Color &_colorSalida, BRDF brdf, Color brdf_mul, float _t, int N, const int &N_MAX, bool typeDirectLight);

void luzGlobalPhoton(vector<Geometria *> geometrias, Rayo &_rayo, int _tipoColor, G_O &_normalObject, Color &_objetoColor, Color &_colorSalida, BRDF &brdf, float &_t, int N, bool &isDelta, bool &seguir);

void luzGlobalBVH(Rayo _rayo, int _tipoColor, G_O _normalObject, Color &_colorSalida, BRDF brdf, Color brdf_mul, int N, int N_MAX);

void trace_outgoing_ray(vector<Geometria *> geometrias, Rayo &_rayo, int _tipoColor, G_O &_normalObject, float &_t, int &N, bool &isDelta, bool &seguir, Color &objetoColor);

void pathTracer(vector<Geometria *> geometrias, Rayo _rayo, int _tipoColor, G_O _normalObject, Color &_colorSalida, BRDF brdf, Color brdf_mul);

void photonMapping(vector<Geometria *> geometrias, Rayo _rayo, int _tipoColor, G_O _normalObject, Color &_colorSalida, BRDF brdf, Color brdf_mul, float _t, int N, const int &N_MAX, bool typeDirectLight);

#endif