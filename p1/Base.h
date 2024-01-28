#ifndef BASE
#define BASE

using namespace std;
#include <iostream>
#include <iomanip>
#include <cmath>
#include "constant.h"
#include <vector>

class Base
{
public:
  float matrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
  int tipo_;
  int eje_ = -1;

  Base();
  Base(float _x, float _y, float _z, const int tipo);
  Base(float _theta, const int eje, const int tipo);
  Base(float _ux, float _uy, float _uz, float _vx, float _vy, float _vz, float _wx, float _wy, float _wz, float _ox, float _oy, float oz, const int tipo);
  static void get_Inverse(Base &a, Base &b);

  friend ostream &operator<<(ostream &os, const Base &obj);
  bool operator==(const Base &b) const;
  void print_base() const;
};

#endif
