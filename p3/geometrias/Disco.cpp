#include "Disco.h"

Disco::Disco(G_O _normal, G_O _punto, float _radio, Color _color, BRDF _brdf)
{
    brdf = _brdf;
    normal = _normal;
    punto = _punto;
    radio = _radio;
    normal.normalize();

    normalRGB = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);

    color = _color;
}

Disco::Disco(G_O _normal, G_O _punto, float _radio, Color _color, BRDF _brdf, int tipoLuz, Color _potencia)
{
    brdf = _brdf;
    normal = _normal;
    punto = _punto;
    radio = _radio;
    normal.normalize();

    normalRGB = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);

    color = _color;
    luz = tipoLuz;
    potencia = _potencia;
}

bool Disco::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &_normal)
{
    // Asumiendo que los vectores están normalizados
    float denom = G_O::dot(rayo.direccion, normal);
    if (abs(denom) > 1e-20)
    {
        G_O p0l0 = punto - rayo.punto;
        t = G_O::dot(p0l0, normal) / denom;

        if (t >= 0)
        {
            if (tipoColor == 1)
            {
                _color = normalRGB;
            }
            else
            {
                _normal = normal;
                _color = color;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    G_O p = rayo.punto + rayo.direccion * t;
    G_O v = p - punto;
    float d2 = G_O::dot(v, v);
    return (sqrtf(d2) <= radio);
}

bool Disco::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor)
{
    // Asumiendo que los vectores están normalizados
    float denom = G_O::dot(rayo.direccion, normal);
    if (abs(denom) > 1e-20)
    {
        G_O p0l0 = punto - rayo.punto;
        t = G_O::dot(p0l0, normal) / denom;

        if (t >= 0)
        {
            if (tipoColor == 1)
            {
                _color = normalRGB;
            }
            else
            {
                _color = color;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    G_O p = rayo.punto + rayo.direccion * t;
    G_O v = p - punto;
    float d2 = G_O::dot(v, v);
    return (sqrtf(d2) <= radio);
}

bool Disco::interseccion(float &t, Color &_color, const Rayo rayo)
{
    // Asumiendo que los vectores están normalizados
    float denom = G_O::dot(rayo.direccion, normal);
    if (abs(denom) > 1e-20)
    {
        G_O p0l0 = punto - rayo.punto;
        t = G_O::dot(p0l0, normal) / denom;

        if (t >= 0)
        {
            _color = color;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    G_O p = rayo.punto + rayo.direccion * t;
    G_O v = p - punto;
    float d2 = G_O::dot(v, v);
    return (sqrtf(d2) <= radio);
}