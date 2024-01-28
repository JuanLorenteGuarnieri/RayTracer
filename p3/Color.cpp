#include "color.h"

Color Color::sum(const Color &a, const Color &b)
{
    return Color(a.r + b.r, a.g + b.g, a.b + b.b);
}
Color Color::div(const Color &a, float b)
{
    return Color(a.r / b, a.g / b, a.b / b);
}
Color Color::div(const Color &a, const Color &b)
{
    return Color(a.r / b.r, a.g / b.g, a.b / b.b);
}
Color Color::mult(const Color &a, float b)
{
    return Color(a.r * b, a.g * b, a.b * b);
}
Color Color::mult(const Color &a, const Color &b)
{
    return Color(a.r * b.r, a.g * b.g, a.b * b.b);
}

double Color::maxi()
{
    if (r >= b && r >= g)
    {
        return r;
    }
    else if (g >= r && g >= b)
    {
        return g;
    }
    else
    {
        return b;
    }
}

Color Color::operator*(float b) const
{
    return mult(*this, b);
}
Color Color::operator*(const Color &b) const
{
    return mult(*this, b);
}
Color Color::operator+(const Color &b) const
{
    return sum(*this, b);
}
Color Color::operator/(const float b) const
{
    return div(*this, b);
}
Color Color::operator/(const Color &b) const
{
    return div(*this, b);
}
void Color::operator=(const Color &a)
{
    r = a.r;
    g = a.g;
    b = a.b;
}

void Color::normalize()
{

    r /= 255;
    g /= 255;
    b /= 255;
}
