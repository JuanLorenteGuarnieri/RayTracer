#include "Cono.h"

Cono::Cono(G_O _centro, float _altura, float _radio, Color _color, Textura _tx, BRDF _brdf)
{
    brdf = _brdf;
    centro = _centro;
    altura = _altura;
    radio = _radio;
    color = _color;
    tx = _tx;
}

Cono::Cono(G_O _centro, float _altura, float _radio, Color _color, Textura _tx, BRDF _brdf, int tipoLuz, Color _potencia)
{
    brdf = _brdf;
    centro = _centro;
    altura = _altura;
    radio = _radio;
    color = _color;
    tx = _tx;
    luz = tipoLuz;
    potencia = _potencia;
}

bool Cono::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor)
{

    float A = rayo.punto.x - centro.x;
    float B = rayo.punto.z - centro.z;
    float D = altura - rayo.punto.y + centro.y;

    float tan = (radio / altura) * (radio / altura);

    float a = (rayo.direccion.x * rayo.direccion.x) + (rayo.direccion.z * rayo.direccion.z) - (tan * (rayo.direccion.y * rayo.direccion.y));
    float b = (2 * A * rayo.direccion.x) + (2 * B * rayo.direccion.z) + (2 * tan * D * rayo.direccion.y);
    float c = (A * A) + (B * B) - (tan * (D * D));

    float denom = b * b - 4 * (a * c);
    if (fabs(denom) < 1e-20)
        return false;
    if (denom < 0.0)
        return false;

    float t1 = (-b - sqrt(denom)) / (2 * a);
    float t2 = (-b + sqrt(denom)) / (2 * a);

    if (t1 > t2)
        t = t2;
    else
        t = t1;

    float r = rayo.punto.y + t * rayo.direccion.y;

    if ((r > centro.y) and (r < centro.y + altura))
    {
        if (tipoColor == 1)
        {
            G_O n = rayo.punto + rayo.direccion * t;
            G_O n2 = centro - n;
            n2 = n2 / max(abs(n2.x), abs(n2.z)) * radio;
            n2.y = 0;
            n2 = n2 + centro;
            n2 = n2 - n;
            n = calcularVectorPerpendicular(n2);

            n.normalize();
            _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
        }
        else if (tipoColor == 2)
        {
            G_O n = rayo.punto + rayo.direccion * t;
            n = n - centro;
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

            int py = tx.height - (int)((n.y) * tx.height);
            int indice = px - 1 + py * tx.width;
            if (indice >= tx.pixeles.size())
            {
                indice = tx.pixeles.size() - 1;
            }

            _color = Color(tx.pixeles[indice]);
        }
        else
        {
            if (tx.achoTextura != -1)
            {
                G_O n = rayo.punto + rayo.direccion * t;
                n = n - centro;
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

                int py = tx.height - (int)((n.y) * tx.height);
                int indice = px - 1 + py * tx.width;
                if (indice >= tx.pixeles.size())
                {
                    indice = tx.pixeles.size() - 1;
                }

                _color = Color(tx.pixeles[indice]);
            }
            else
            {
                _color = color;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Cono::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal)
{

    float A = rayo.punto.x - centro.x;
    float B = rayo.punto.z - centro.z;
    float D = altura - rayo.punto.y + centro.y;

    float tan = (radio / altura) * (radio / altura);

    float a = (rayo.direccion.x * rayo.direccion.x) + (rayo.direccion.z * rayo.direccion.z) - (tan * (rayo.direccion.y * rayo.direccion.y));
    float b = (2 * A * rayo.direccion.x) + (2 * B * rayo.direccion.z) + (2 * tan * D * rayo.direccion.y);
    float c = (A * A) + (B * B) - (tan * (D * D));

    float denom = b * b - 4 * (a * c);
    if (fabs(denom) < 1e-20)
        return false;
    if (denom < 0.0)
        return false;

    float t1 = (-b - sqrt(denom)) / (2 * a);
    float t2 = (-b + sqrt(denom)) / (2 * a);

    if (t1 > t2)
        t = t2;
    else
        t = t1;

    float r = rayo.punto.y + t * rayo.direccion.y;

    if ((r > centro.y) and (r < centro.y + altura))
    {
        if (tipoColor == 1)
        {
            G_O n = rayo.punto + rayo.direccion * t;
            G_O n2 = centro - n;
            n2 = n2 / max(abs(n2.x), abs(n2.z)) * radio;
            n2.y = 0;
            n2 = n2 + centro;
            n2 = n2 - n;
            n = calcularVectorPerpendicular(n2);
            n.normalize();
            _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
        }
        else if (tipoColor == 2)
        {
            G_O p = rayo.punto + rayo.direccion * t;
            G_O n = p - centro;
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

            int py = tx.height - (int)((n.y) * tx.height);
            int indice = px - 1 + py * tx.width;
            if (indice >= tx.pixeles.size())
            {
                indice = tx.pixeles.size() - 1;
            }

            _color = Color(tx.pixeles[indice]);
        }
        else
        {
            G_O p = rayo.punto + rayo.direccion * t;
            G_O n = rayo.punto + rayo.direccion * t;
            G_O n2 = centro - n;
            n2 = n2 / max(abs(n2.x), abs(n2.z)) * radio;
            n2.y = 0;
            n2 = n2 + centro;
            n2 = n2 - n;
            n = calcularVectorPerpendicular(n2);
            n.normalize();
            normal = n;
            _color = color;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Cono::interseccion(float &t, Color &_color, const Rayo rayo)
{
    float A = rayo.punto.x - centro.x;
    float B = rayo.punto.z - centro.z;
    float D = altura - rayo.punto.y + centro.y;

    float tan = (radio / altura) * (radio / altura);

    float a = (rayo.direccion.x * rayo.direccion.x) + (rayo.direccion.z * rayo.direccion.z) - (tan * (rayo.direccion.y * rayo.direccion.y));
    float b = (2 * A * rayo.direccion.x) + (2 * B * rayo.direccion.z) + (2 * tan * D * rayo.direccion.y);
    float c = (A * A) + (B * B) - (tan * (D * D));

    float denom = b * b - 4 * (a * c);
    if (fabs(denom) < 1e-20)
        return false;
    if (denom < 0.0)
        return false;

    float t1 = (-b - sqrt(denom)) / (2 * a);
    float t2 = (-b + sqrt(denom)) / (2 * a);

    if (t1 > t2)
        t = t2;
    else
        t = t1;

    float r = rayo.punto.y + t * rayo.direccion.y;

    if ((r > centro.y) and (r < centro.y + altura))
    {
        _color = color;
        return true;
    }
    else
    {
        return false;
    }
}