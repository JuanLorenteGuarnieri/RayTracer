#ifndef PHOTON
#define PHOTON

#include "../p1/G_O.h"
#include "color.h"
/*
    Your Photon class implementation, which stores each
    photon walk interaction
*/
class Photon
{
public:
  G_O position_; // 3D point of the interaction
  G_O wi;
  Color potencia;
  // It returns the axis i position (x, y or z)
  float position(std::size_t i) const { return position_[i]; }
  Photon(G_O _wi, G_O _origen, Color _p);
  Photon()
  {
    wi = G_O(0, 0, 0, 0);
    position_ = G_O(0, 0, 0, 0);
    potencia = Color(0, 0, 0);
  }
};

#endif