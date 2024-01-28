#ifndef GEOMETRICAL_OBJECTS
#define GEOMETRICAL_OBJECTS

using namespace std;
#include "Base.h"
#include <iostream>
#include <cmath>

class G_O
{
public:
    float x;
    float y;
    float z;
    int last;

    G_O();
    G_O(float _x, float _y, float _z, int _last);

    // Funciones miembro estáticas
    static G_O sum(const G_O &a, const G_O &b);
    static G_O resta(const G_O &a, const G_O &b);
    static float dot(const G_O &a, const G_O &b);
    static G_O vectorial_Product(const G_O &a, const G_O &b);
    static G_O fminf(G_O a, const G_O b);
    static G_O fmaxf(G_O a, const G_O b);

    const G_O &change_value(float a, float b, float c, int i);
    float modulo();
    G_O escalar(float num) const;
    void normalize();
    G_O translacion(Base base);
    G_O escalado(Base base);
    G_O rotacion(Base base);
    G_O cambio_base(Base base);

    // Operadores sobrecargados
    G_O operator+(const G_O &b) const;
    G_O operator-(const G_O &b) const;
    G_O operator*(const G_O &b) const;
    G_O operator*(const float b) const;
    G_O operator/(float num) const;
    float operator[](int i) const;
    void operator=(const G_O &b);
    bool operator==(const G_O &b) const;
    bool operator!=(const G_O &b) const;
    bool operator<(const G_O &b) const;
    bool operator>(const G_O &b) const;
    friend ostream &operator<<(ostream &os, const G_O &obj);

    // objeto.normalize() que se modifique el objeto a sí mismo

    void Print() const;
};

#endif
