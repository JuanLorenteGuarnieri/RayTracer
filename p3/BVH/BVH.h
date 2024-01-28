#ifndef BVH_H
#define BVH_H

#include "../geometrias/Geometria.h"
#include "../geometrias/Triangulo.h"
#include "../../p1/G_O.h"
#include "BVHNode.h"

extern vector<Triangulo> tri;
extern TriTextura triTxt;

class BVH
{
public:
  G_O aabbMin, aabbMax;
  int leftChild, firstPrim, primCount;
  BVH(){}
  void rangoValores();
  vector<BVHNode> BuildBVH(bool ponerTextura);
  bool isLeaf() { return primCount > 0; }
};


#endif