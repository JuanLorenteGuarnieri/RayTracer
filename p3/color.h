#ifndef COLOR_H
#define COLOR_H

#include "../p2/pixel.h"
#include <math.h>
#include <cmath>

class Color
{
public:
    float r, g, b;

    Color(float _r, float _g, float _b)
    {
        r = _r;
        g = _g;
        b = _b;
    }

    Color(Pixel p)
    {
        r = p.r;
        g = p.g;
        b = p.b;
    }

    // Constructor predeterminado
    Color()
    {
        r = 0.0;
        g = 0.0;
        b = 0.0;
    }

    static Color sum(const Color &a, const Color &b);
    static Color div(const Color &a, float b);
    static Color div(const Color &a, const Color &b);
    static Color mult(const Color &a, float b);
    static Color mult(const Color &a, const Color &b);
    double maxi();

    /**
     * Normaliza el color, asegurando que la longitud del vector de color sea 1.
     */
    void normalize();

    Color operator+(const Color &b) const;
    Color operator/(const Color &b) const;
    Color operator/(float b) const;
    Color operator*(float b) const;
    Color operator*(const Color &b) const;
    void operator=(const Color &a);
};

#endif