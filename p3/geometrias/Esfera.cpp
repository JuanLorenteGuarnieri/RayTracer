#include "Esfera.h"

Esfera::Esfera(float _x, float _y, float _z, float _radio, Color _color, Textura _tx, BRDF _brdf)
{
    brdf = _brdf;
    centro = G_O(_x, _y, _z, 1);
    radio = _radio;
    color = _color;
    tx = _tx;
}

Esfera::Esfera(float _x, float _y, float _z, float _radio, Color _color, Textura _tx, BRDF _brdf, int tipoLuz, Color _potencia)
{
    brdf = _brdf;
    centro = G_O(_x, _y, _z, 1);
    radio = _radio;
    color = _color;
    tx = _tx;
    luz = tipoLuz;
    potencia = _potencia;
}

bool Esfera::interseccion(float &t, Color &_color, const Rayo rayo)
{
    _color = color;
    float a = G_O::dot(rayo.direccion, rayo.direccion);
    float b = 2 * G_O::dot(rayo.direccion, rayo.punto - centro);
    float c = G_O::dot(rayo.punto - centro, rayo.punto - centro) - radio * radio;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return false;
    }
    else if (discriminant > 0)
    {
        t = (-b + sqrt(discriminant)) / (2 * a);
        float t2 = -b / a - t;
        if (t < 0 && t2 < 0)
        {
            return false;
        }
        else if (t > 0 && t2 < 0)
        {
            return true;
        }
        else if (t < 0 && t2 > 0)
        {
            t = t2;
            return true;
        }
        else if (t > 0 && t2 > 0)
        {
            if (t2 < t)
            {
                t = t2;
            }
            return true;
        }
        return true;
    }
    else
    {
        return true;
    }
}

void mapToPixel(const G_O &normal, int imageWidth, int imageHeight, int &pixelX, int &pixelY)
{
    // Convertir a coordenadas esféricas
    double theta = atan2(normal.z, normal.x);
    double phi = acos(normal.y);

    // Normalizar theta y phi al rango [0, 1]
    double u = (theta + PI) / (2 * PI);
    double v = phi / PI;

    // Mapear a coordenadas de píxeles
    pixelX = imageWidth - 1 - static_cast<int>(u * (imageWidth - 1));
    pixelY = imageHeight - 1 - static_cast<int>(v * (imageHeight - 1));
}

bool Esfera::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal)
{

    float a = G_O::dot(rayo.direccion, rayo.direccion);
    float b = 2 * G_O::dot(rayo.direccion, rayo.punto - centro);
    float c = G_O::dot(rayo.punto - centro, rayo.punto - centro) - radio * radio;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return false;
    }
    else if (discriminant > 0)
    {
        t = (-b + sqrt(discriminant)) / (2 * a);
        float t2 = -b / a - t;

        if (t < 0 && t2 < 0)
        {
            return false;
        }
        else if (t < 0 && t2 > 0)
        {
            t = t2;
        }
        else if (t > 0 && t2 > 0)
        {
            if (t2 < t)
            {
                t = t2;
            }
        }
        if (tipoColor == 1)
        {

            G_O p = rayo.punto + rayo.direccion * t;
            G_O n = centro - p;
            n.normalize();
            normal = n;
            _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
        }
        else if (tipoColor == 2)
        {
            G_O p = rayo.punto + rayo.direccion * t;
            G_O n = centro - p;
            n.normalize();
            normal = n;
            int px;
            if (n.z >= 0)
            {
                px = tx.width - ((n.x + 1) * tx.width / 4);
            }
            else
            {
                px = ((n.x + 1) * tx.width / 4);
            }

            int py = (int)((n.y + 1) * tx.height / 2);
            int indice = px - 1 + py * tx.width;

            _color = Color(tx.pixeles[indice]);
        }
        else
        {
            if (tx.achoTextura != -1)
            {
                G_O p = rayo.punto + rayo.direccion * t;
                G_O n = centro - p;
                n.normalize();
                normal = n;
                int px, py;
                mapToPixel(normal, tx.width, tx.height, px, py);
                int indice = px + py * tx.width;
                if (tx.pixeles.size() < indice)
                {
                    cout << "Error en las texturas de la esfera(tx.pixeles.size() < indice)\n";
                    cout << "Indice: " << indice
                         << "\nTamaño Imagen: " << tx.pixeles.size() << "\n";
                }
                if (indice < 0)
                {
                    cout << "Error en las texturas de la esfera(indice < 0)\n";
                }
                _color = Color(tx.pixeles[indice]);
            }
            else
            {
                G_O p = rayo.punto + rayo.direccion * t;
                G_O n = centro - p;
                n.normalize();
                normal = n;
                _color = color;
            }
        }

        return true;
    }
    else
    {
        return true;
    }
}

bool Esfera::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor)
{

    float a = G_O::dot(rayo.direccion, rayo.direccion);
    float b = 2 * G_O::dot(rayo.direccion, rayo.punto - centro);
    float c = G_O::dot(rayo.punto - centro, rayo.punto - centro) - radio * radio;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return false;
    }
    else if (discriminant > 0)
    {
        t = (-b + sqrt(discriminant)) / (2 * a);
        float t2 = -b / a - t;

        if (t < 0 && t2 < 0)
        {
            return false;
        }
        else if (t < 0 && t2 > 0)
        {
            t = t2;
        }
        else if (t > 0 && t2 > 0)
        {
            if (t2 < t)
            {
                t = t2;
            }
        }
        if (tipoColor == 1)
        {
            G_O p = rayo.punto + rayo.direccion * t;
            G_O n = centro - p;
            n.normalize();
            _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
        }
        else if (tipoColor == 2)
        {
            G_O p = rayo.punto + rayo.direccion * t;
            G_O n = centro - p;
            n.normalize();
            int px;
            if (n.z >= 0)
            {
                px = tx.width - ((n.x + 1) * tx.width / 4);
            }
            else
            {
                px = ((n.x + 1) * tx.width / 4);
            }

            int py = (int)((n.y + 1) * tx.height / 2);
            int indice = px - 1 + py * tx.width;

            _color = Color(tx.pixeles[indice]);
        }
        else
        {
            _color = color;
        }

        return true;
    }
    else
    {
        return true;
    }
}

bool Esfera::interseccion(float &t, Color &_color, Rayo &rayo, G_O &puntoIntersecta)
{
    _color = color;
    G_O l = centro - rayo.punto;
    float s = G_O::dot(l, rayo.direccion);
    float l2 = G_O::dot(l, l);
    if (s < 0 && l2 > (radio * radio))
    {
        return false;
    }
    float m2 = l2 - (s * s);
    if (m2 > (radio * radio))
    {
        return false;
    }
    float q = sqrt(radio * radio - m2);
    if (l2 > (radio * radio))
    {
        t = s - q;
    }
    else
    {
        t = s + q;
    }
    puntoIntersecta = rayo.punto + rayo.direccion * t;
    cout << "Closest intersection at " << '\n'
         << rayo.punto + rayo.direccion * t << "\n";
    return true;
}