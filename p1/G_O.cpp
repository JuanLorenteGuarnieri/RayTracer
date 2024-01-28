#include "./G_O.h"

G_O::G_O()
{
    x = 0;
    y = 0;
    z = 0;
    last = 0;
}

G_O::G_O(float _x, float _y, float _z, int _last)
{
    x = _x;
    y = _y;
    z = _z;
    last = _last;
}

G_O G_O::sum(const G_O &a, const G_O &b)
{
    // Dirección + Dirección = Dirección
    if (a.last == 0 && b.last == 0)
    {
        return G_O(a.x + b.x, a.y + b.y, a.z + b.z, 0);
    }
    // Dirección + Punto = Punto
    else if (a.last == 0 || b.last == 0)
    {
        return G_O(a.x + b.x, a.y + b.y, a.z + b.z, 1);
    }
    // Punto + Punto = Punto No tiene sentido hacerlo
    else
    {
        return G_O(a.x + b.x, a.y + b.y, a.z + b.z, 1);
    }
}

G_O G_O::resta(const G_O &a, const G_O &b)
{
    // Dirección - Dirección = Dirección
    if (a.last == 0 && b.last == 0)
    {
        return G_O(a.x - b.x, a.y - b.y, a.z - b.z, 0);
    }
    // Punto - Punto = Dirección
    else if (a.last == 1 || b.last == 1)
    {
        return G_O(a.x - b.x, a.y - b.y, a.z - b.z, 0);
    }
    // Dirección - Punto = Punto ?????
    else if (a.last == 0)
    {
        return G_O(a.x - b.x, a.y - b.y, a.z - b.z, 1);
    }
    // Punto - Dirección = Dirección ????????
    else
    {
        return G_O(a.x - b.x, a.y - b.y, a.z - b.z, 0);
    }
}

float G_O::dot(const G_O &a, const G_O &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

G_O G_O::vectorial_Product(const G_O &a, const G_O &b)
{
    // Dirección X Dirección = Dirección
    if (a.last == 0 && b.last == 0)
    {
        return G_O(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 0);
    }
    // Punto X Punto = Punto
    else if (a.last == 1 || b.last == 1)
    {
        return G_O(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 1);
    }
    else
    {
        return G_O(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 2);
    }
}

G_O G_O::translacion(Base base)
{
    if (last == 0)
    {
        return G_O(x, y, z, 0);
    }
    else
    {
        return G_O(x + base.matrix[0][3], y + base.matrix[1][3], z + base.matrix[2][3], 1);
    }
}

G_O G_O::escalado(Base base)
{
    return G_O(x * base.matrix[0][0], y * base.matrix[1][1], z * base.matrix[2][2], last);
}

G_O G_O::rotacion(Base base)
{
    G_O aux;
    if (base.eje_ == EJE_X)
    {
        aux = G_O(x, y * base.matrix[1][1] + z * base.matrix[1][2], y * base.matrix[2][1] + z * base.matrix[2][2], last);
    }
    else if (base.eje_ == EJE_Y)
    {
        aux = G_O(x * base.matrix[0][0] + z * base.matrix[0][2], y, x * base.matrix[2][0] + z * base.matrix[2][2], last);
    }
    else
    {
        aux = G_O(x * base.matrix[0][0] + y * base.matrix[0][1], x * base.matrix[1][0] + y * base.matrix[1][1], z, last);
    }
    aux.normalize();
    return aux;
}

G_O G_O::cambio_base(Base base)
{
    G_O aux = G_O(x * base.matrix[0][0] + y * base.matrix[0][1] + z * base.matrix[0][2] + last * base.matrix[0][3],
                  x * base.matrix[1][0] + y * base.matrix[1][1] + z * base.matrix[1][2] + last * base.matrix[1][3],
                  x * base.matrix[2][0] + y * base.matrix[2][1] + z * base.matrix[2][2] + last * base.matrix[2][3], last);
    aux.normalize();
    return aux;
}

G_O G_O::fminf(G_O a, const G_O b)
{
    return G_O(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), 1);
}

G_O G_O::fmaxf(G_O a, const G_O b)
{
    return G_O(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), 1);
}

const G_O &G_O::change_value(float a, float b, float c, int i)
{
    x = a;
    y = b;
    z = c;
    last = i;
    return *this;
}

float G_O::modulo()
{
    return sqrt(x * x + y * y + z * z);
}

void G_O::normalize()
{
    float length = sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
    this->change_value(x, y, z, last);
}

G_O G_O::escalar(float num) const
{
    return G_O(x * num, y * num, z * num, last);
}

void G_O::Print() const
{
    std::cout << "(" << x << ", " << y << ", " << z << ", " << last << ")" << std::endl;
}

/*************************************************************************************************************
 * RECARGA OPERADORES
 *************************************************************************************************************/
G_O G_O::operator+(const G_O &b) const
{
    return sum(*this, b);
}

G_O G_O::operator-(const G_O &b) const
{
    return resta(*this, b);
}

void G_O::operator=(const G_O &b)
{
    x = b.x;
    y = b.y;
    z = b.z;
    last = b.last;
}

G_O G_O::operator*(const G_O &b) const
{
    return vectorial_Product(*this, b);
}

G_O G_O::operator*(float b) const
{
    return this->escalar(b);
}

float G_O::operator[](int i) const
{
    if (i == 0)
    {
        return x;
    }
    else if (i == 1)
    {
        return y;
    }
    else if (i == 2)
    {
        return z;
    }
    return 0;
}

G_O G_O::operator/(const float num) const
{
    return G_O(x / num, y / num, z / num, last);
}

bool G_O::operator==(const G_O &b) const
{
    return x == b.x && y == b.y && z == b.z && last == b.last;
}

bool G_O::operator!=(const G_O &b) const
{
    return x != b.x || y != b.y || z != b.z || last != b.last;
}

bool G_O::operator<(const G_O &b) const
{
    return x < b.x && y < b.y && z < b.z && last == b.last;
}

bool G_O::operator>(const G_O &b) const
{
    return x > b.x && y > b.y && z > b.z && last == b.last;
}

ostream &operator<<(ostream &os, const G_O &obj)
{
    os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.last << ")";
    return os;
}