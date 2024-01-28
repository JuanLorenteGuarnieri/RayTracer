#include "Plano.h"

Plano::Plano(float normal_x, float normal_y, float normal_z, float _distancia, Color _color, Textura _tx, BRDF _brdf)
{
    brdf = _brdf;
    float len = sqrt(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z);
    if (len != 0)
    {
        normal = G_O(normal_x /= len, normal_y /= len, normal_z /= len, 0);
    }
    else
    {
        normal = G_O(normal_x, normal_y, normal_z, 0);
    }

    distancia = _distancia;
    color = _color;
    tx = _tx;
    normal.normalize();

    normalRGB = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);
}

Plano::Plano(float normal_x, float normal_y, float normal_z, float _distancia, Color _color, Textura _tx, BRDF _brdf, int tipoLuz, Color _potencia)
{
    brdf = _brdf;
    float len = sqrt(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z);
    if (len != 0)
    {
        normal = G_O(normal_x /= len, normal_y /= len, normal_z /= len, 0);
    }
    else
    {
        normal = G_O(normal_x, normal_y, normal_z, 0);
    }

    distancia = _distancia;
    color = _color;
    tx = _tx;
    normal.normalize();

    normalRGB = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);
    luz = tipoLuz;
    potencia = _potencia;
}

bool Plano::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &_normal)
{
    // Asumiendo que los vectores están normalizados
    float denom = G_O::dot(rayo.direccion, normal);
    if (denom > 1e-20)
    {

        t = G_O::dot(normal, rayo.punto);
        t = -(t - distancia) / denom;

        if (t >= 0)
        {
            if (tipoColor == 1)
            {
                _color = normalRGB;
                _normal = normal;
            }
            else if (tipoColor == 2 && tx.achoTextura != -1)
            {
                G_O p = rayo.punto + rayo.direccion * t;
                _normal = normal;
                int indice = 0, x, y;
                if (normal.x != 0 && normal.y == 0 && normal.z == 0)
                {
                    if (p.z < 0)
                    {
                        x = tx.width - ((int)(abs(p.z) / tx.achoTextura) % tx.width);
                    }
                    else
                    {
                        x = (int)(abs(p.z) / tx.achoTextura) % tx.width;
                    }
                    if (p.y < 0)
                    {
                        y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                    else
                    {
                        y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                }
                else if (normal.y != 0 && normal.x == 0 && normal.z == 0)
                {
                    if (p.x < 0)
                    {
                        x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                    }
                    else
                    {
                        x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                    }
                    if (p.z < 0)
                    {
                        y = tx.height - ((int)(abs(p.z) / tx.achoTextura) % tx.height);
                    }
                    else
                    {
                        y = ((int)(abs(p.z) / tx.achoTextura) % tx.height);
                    }
                }
                else if (normal.z != 0 && normal.y == 0 && normal.x == 0)
                {
                    if (p.x < 0)
                    {
                        x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                    }
                    else
                    {
                        x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                    }
                    if (p.y < 0)
                    {
                        y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                    else
                    {
                        y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                }
                else
                {
                    if (p.x < 0)
                    {
                        x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                    }
                    else
                    {
                        x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                    }
                    if (p.y < 0)
                    {
                        y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                    else
                    {
                        y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                }

                indice = x + y * tx.width;
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
                    G_O p = rayo.punto + rayo.direccion * t;
                    _normal = normal;
                    int indice = 0, x, y;
                    if (normal.x != 0 && normal.y == 0 && normal.z == 0)
                    {
                        if (p.z < 0)
                        {
                            x = tx.width - ((int)(abs(p.z) / tx.achoTextura) % tx.width);
                        }
                        else
                        {
                            x = (int)(abs(p.z) / tx.achoTextura) % tx.width;
                        }
                        if (p.y < 0)
                        {
                            y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                        }
                        else
                        {
                            y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                        }
                    }
                    else if (normal.y != 0 && normal.x == 0 && normal.z == 0)
                    {
                        if (p.x < 0)
                        {
                            x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                        }
                        else
                        {
                            x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                        }
                        if (p.z < 0)
                        {
                            y = tx.height - ((int)(abs(p.z) / tx.achoTextura) % tx.height);
                        }
                        else
                        {
                            y = ((int)(abs(p.z) / tx.achoTextura) % tx.height);
                        }
                    }
                    else if (normal.z != 0 && normal.y == 0 && normal.x == 0)
                    {
                        if (p.x < 0)
                        {
                            x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                        }
                        else
                        {
                            x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                        }
                        if (p.y < 0)
                        {
                            y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                        }
                        else
                        {
                            y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                        }
                    }
                    else
                    {
                        if (p.x < 0)
                        {
                            x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                        }
                        else
                        {
                            x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                        }
                        if (p.y < 0)
                        {
                            y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                        }
                        else
                        {
                            y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                        }
                    }

                    indice = x + y * tx.width;
                    if (indice >= tx.pixeles.size())
                    {
                        indice = tx.pixeles.size() - 1;
                    }

                    _color = Color(tx.pixeles[indice]);
                }
                else
                {
                    _color = color;
                    _normal = normal;
                }
            }

            return true;
        }
    }

    return false;
}

bool Plano::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor)
{

    // Asumiendo que los vectores están normalizados
    float denom = G_O::dot(rayo.direccion, normal);
    if (denom > 1e-20)
    {

        t = G_O::dot(normal, rayo.punto);
        t = -(t - distancia) / denom;

        if (t >= 0)
        {
            if (tipoColor == 1)
            {
                _color = normalRGB;
            }
            else if (tipoColor == 2 && tx.achoTextura != -1)
            {
                G_O p = rayo.punto + rayo.direccion * t;
                int indice = 0, x, y;
                if (normal.x != 0 && normal.y == 0 && normal.z == 0)
                {
                    if (p.z < 0)
                    {
                        x = tx.width - ((int)(abs(p.z) / tx.achoTextura) % tx.width);
                    }
                    else
                    {
                        x = (int)(abs(p.z) / tx.achoTextura) % tx.width;
                    }
                    if (p.y < 0)
                    {
                        y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                    else
                    {
                        y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                }
                else if (normal.y != 0 && normal.x == 0 && normal.z == 0)
                {
                    if (p.x < 0)
                    {
                        x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                    }
                    else
                    {
                        x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                    }
                    if (p.z < 0)
                    {
                        y = tx.height - ((int)(abs(p.z) / tx.achoTextura) % tx.height);
                    }
                    else
                    {
                        y = ((int)(abs(p.z) / tx.achoTextura) % tx.height);
                    }
                }
                else if (normal.z != 0 && normal.y == 0 && normal.x == 0)
                {
                    if (p.x < 0)
                    {
                        x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                    }
                    else
                    {
                        x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                    }
                    if (p.y < 0)
                    {
                        y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                    else
                    {
                        y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                }
                else
                {
                    if (p.x < 0)
                    {
                        x = tx.width - ((int)(abs(p.x) / tx.achoTextura) % tx.width);
                    }
                    else
                    {
                        x = (int)(abs(p.x) / tx.achoTextura) % tx.width;
                    }
                    if (p.y < 0)
                    {
                        y = tx.height - ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                    else
                    {
                        y = ((int)(abs(p.y) / tx.achoTextura) % tx.height);
                    }
                }

                indice = x + y * tx.width;
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

            return true;
        }
    }

    return false;
}

bool Plano::interseccion(float &t, Color &_color, const Rayo rayo)
{

    // Asumiendo que los vectores están normalizados
    float denom = G_O::dot(rayo.direccion, normal);
    if (denom > 1e-20)
    {

        t = G_O::dot(normal, rayo.punto);
        t = -(t - distancia) / denom;

        if (t >= 0)
        {
            _color = color;
            return true;
        }
    }

    return false;
}