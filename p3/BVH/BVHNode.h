#ifndef BVHNODE_H
#define BVHNODE_H
#include "../../p1/G_O.h"

class BVHNode
{
public:
  G_O aabbMin, aabbMax;
  int leftChild, firstPrim, primCount;

  bool isLeaf() { return primCount > 0; }
};


#endif