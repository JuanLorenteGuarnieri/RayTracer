#include "Elipsoide.h"

Elipsoide::Elipsoide(G_O _centro, G_O _radio, Color _color, Textura _tx, BRDF _brdf)
{
    brdf = _brdf;
    centro = _centro;
    radio = _radio;
    color = _color;
    tx = _tx;
}

Elipsoide::Elipsoide(G_O _centro, G_O _radio, Color _color, Textura _tx, BRDF _brdf, int tipoLuz, Color _potencia)
{
    brdf = _brdf;
    centro = _centro;
    radio = _radio;
    color = _color;
    tx = _tx;
    luz = tipoLuz;
    potencia = _potencia;
}

bool Elipsoide::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal)
{
    float a = ((rayo.direccion.x * rayo.direccion.x) / (radio.x * radio.x)) + ((rayo.direccion.y * rayo.direccion.y) / (radio.y * radio.y)) + ((rayo.direccion.z * rayo.direccion.z) / (radio.z * radio.z));
    float b = 2 * (((rayo.direccion.x * (rayo.punto.x - centro.x)) / (radio.x * radio.x)) + ((rayo.direccion.y * (rayo.punto.y - centro.y)) / (radio.y * radio.y)) + ((rayo.direccion.z * (rayo.punto.z - centro.z)) / (radio.z * radio.z)));
    float c0 = (((rayo.punto.x - centro.x) * (rayo.punto.x - centro.x)) / (radio.x * radio.x)) + (((rayo.punto.y - centro.y) * (rayo.punto.y - centro.y)) / (radio.y * radio.y)) + (((rayo.punto.z - centro.z) * (rayo.punto.z - centro.z)) / (radio.z * radio.z)) - 1;
    float root = b * b - 4 * a * c0;

    G_O p = rayo.punto + rayo.direccion * t;
    G_O n = centro - p;
    n.normalize();
    normal = n;

    if (2 * a < 1e-20)
        return false;

    if (root < 0)
        return false;

    else if (root > 0)
    {
        t = min((-b - sqrt(root)) / (2 * a), (-b + sqrt(root)) / (2 * a));
        if (tipoColor == 1)
        {
            _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
        }
        else if (tipoColor == 2)
        {
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
    t = -b / (2 * a);

    if (tipoColor == 1)
    {
        _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
    }
    else if (tipoColor == 2)
    {
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
    }
    return true;
}

bool Elipsoide::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor)
{
    float a = ((rayo.direccion.x * rayo.direccion.x) / (radio.x * radio.x)) + ((rayo.direccion.y * rayo.direccion.y) / (radio.y * radio.y)) + ((rayo.direccion.z * rayo.direccion.z) / (radio.z * radio.z));
    float b = 2 * (((rayo.direccion.x * (rayo.punto.x - centro.x)) / (radio.x * radio.x)) + ((rayo.direccion.y * (rayo.punto.y - centro.y)) / (radio.y * radio.y)) + ((rayo.direccion.z * (rayo.punto.z - centro.z)) / (radio.z * radio.z)));
    float c0 = (((rayo.punto.x - centro.x) * (rayo.punto.x - centro.x)) / (radio.x * radio.x)) + (((rayo.punto.y - centro.y) * (rayo.punto.y - centro.y)) / (radio.y * radio.y)) + (((rayo.punto.z - centro.z) * (rayo.punto.z - centro.z)) / (radio.z * radio.z)) - 1;
    float root = b * b - 4 * a * c0;

    if (2 * a < 1e-20)
        return false;

    if (root < 0)
        return false;

    else if (root > 0)
    {
        t = min((-b - sqrt(root)) / (2 * a), (-b + sqrt(root)) / (2 * a));
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
    t = -b / (2 * a);

    if (tipoColor == 1)
    {
        G_O n = rayo.punto + rayo.direccion * t;
        n = centro - n;
        n.normalize();
        _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
    }
    else if (tipoColor == 2)
    {
        G_O n = rayo.punto + rayo.direccion * t;
        n = centro - n;
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

bool Elipsoide::interseccion(float &t, Color &_color, const Rayo rayo)
{
    _color = color;

    float a = ((rayo.direccion.x * rayo.direccion.x) / (radio.x * radio.x)) + ((rayo.direccion.y * rayo.direccion.y) / (radio.y * radio.y)) + ((rayo.direccion.z * rayo.direccion.z) / (radio.z * radio.z));
    float b = 2 * (((rayo.direccion.x * (rayo.punto.x - centro.x)) / (radio.x * radio.x)) + ((rayo.direccion.y * (rayo.punto.y - centro.y)) / (radio.y * radio.y)) + ((rayo.direccion.z * (rayo.punto.z - centro.z)) / (radio.z * radio.z)));
    float c0 = (((rayo.punto.x - centro.x) * (rayo.punto.x - centro.x)) / (radio.x * radio.x)) + (((rayo.punto.y - centro.y) * (rayo.punto.y - centro.y)) / (radio.y * radio.y)) + (((rayo.punto.z - centro.z) * (rayo.punto.z - centro.z)) / (radio.z * radio.z)) - 1;
    float root = b * b - 4 * a * c0;

    if (2 * a < 1e-20)
        return false;

    if (root < 0)
        return false;

    else if (root > 0)
    {
        t = min((-b - sqrt(root)) / (2 * a), (-b + sqrt(root)) / (2 * a));
        return true;
    }
    t = -b / (2 * a);
    return true;
}