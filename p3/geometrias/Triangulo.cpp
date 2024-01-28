#include "Triangulo.h"

int indTriangulo;
TriTextura triTxt;
extern vector<Triangulo> tri;

Triangulo::Triangulo(G_O _v1, G_O _v2, G_O _v3, Color _color, BRDF _brdf)
{
    brdf = _brdf;
    v0 = _v1;
    v1 = _v2;
    v2 = _v3;
    color = _color;
}

Triangulo::Triangulo(G_O _v1, G_O _v2, G_O _v3, Color _color, BRDF _brdf, int tipoLuz, Color _potencia)
{
    brdf = _brdf;
    v0 = _v1;
    v1 = _v2;
    v2 = _v3;
    color = _color;
    luz = tipoLuz;
    potencia = _potencia;
}

float areaTriangulo3D(const G_O &A, const G_O &B, const G_O &C)
{
    G_O AB = B - A;
    G_O AC = C - A;

    // Producto cruz de AB y AC
    G_O cross = AB * AC;

    // Magnitud del producto cruz (área del paralelogramo) dividida por 2
    return cross.modulo() / 2.0f;
}

void EncontrarPuntoInterpolado(G_O p, G_O v1, G_O v2, G_O v3, float &resX, float &resY, const int in)
{
    // Calculamos las áreas
    float areaABC = areaTriangulo3D(v1, v2, v3);
    float lambda1 = areaTriangulo3D(v2, v3, p) / areaABC;
    float lambda2 = areaTriangulo3D(v3, v1, p) / areaABC;
    float lambda3 = areaTriangulo3D(v1, v2, p) / areaABC;

    // Interpolación con pesos normalizados
    resX = lambda1 * triTxt.tri[in].Tv1x + lambda2 * triTxt.tri[in].Tv2x + lambda3 * triTxt.tri[in].Tv3x;
    resY = lambda1 * triTxt.tri[in].Tv1y + lambda2 * triTxt.tri[in].Tv2y + lambda3 * triTxt.tri[in].Tv3y;
    if (resX > 1)
        resX = 1;
    if (resY > 1)
        resY = 1;
}

bool Triangulo::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &_normal)
{
    int in = indTriangulo;
    G_O v0v1 = v1 - v0;
    G_O v0v2 = v2 - v0;
    G_O normal = v0v2 * v0v1;
    G_O pvec = v0v2 * rayo.direccion;
    float det = (G_O::dot(v0v1, pvec));

    if (det > -1e-20 && det < 1e-20)
        return false; // ray parallel to triangle

    float invDet = 1 / det;

    G_O tvec = rayo.punto - v0;
    float u = (G_O::dot(tvec, pvec)) * invDet;
    if (u < 0 || u > 1)
        return false;
    G_O qvec = v0v1 * tvec;
    float v = (G_O::dot(rayo.direccion, qvec)) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    t = (G_O::dot(v0v2, qvec)) * invDet;
    normal.normalize();
    _normal = normal;
    if (tipoColor == 1)
    {
        _color = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);
    }
    else if (tipoColor == 2)
    {
        float x, y;
        G_O p = rayo.punto + rayo.direccion * t;

        EncontrarPuntoInterpolado(p, v0, v1, v2, x, y, in);

        int indTxt = triTxt.tri[in].nFichero;

        int indice = (int)(x * (triTxt.txt[indTxt].width - 1)) + ((int)((1 - y) * (triTxt.txt[indTxt].height - 1))) * triTxt.txt[indTxt].width;
        _color = Color(triTxt.txt[indTxt].pixeles[indice]);
    }
    else
    {
        if (triTxt.txt.size() != 0)
        {
            float x, y;
            G_O p = rayo.punto + rayo.direccion * t;

            EncontrarPuntoInterpolado(p, v0, v1, v2, x, y, in);

            int indTxt = triTxt.tri[in].nFichero;

            int indice = (int)(x * (triTxt.txt[indTxt].width - 1)) + ((int)((1 - y) * (triTxt.txt[indTxt].height - 1))) * triTxt.txt[indTxt].width;

            _color = Color(triTxt.txt[indTxt].pixeles[indice]);
        }
        else
        {
            _color = color;
        }
    }

    return true;
}

bool Triangulo::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor)
{
    int in = indTriangulo;
    G_O v0v1 = v1 - v0;
    G_O v0v2 = v2 - v0;
    G_O normal = v0v2 * v0v1;
    G_O pvec = v0v2 * rayo.direccion;
    float det = (G_O::dot(v0v1, pvec));

    if (det > -1e-20 && det < 1e-20)
        return false; // ray parallel to triangle

    float invDet = 1 / det;

    G_O tvec = rayo.punto - v0;
    float u = (G_O::dot(tvec, pvec)) * invDet;
    if (u < 0 || u > 1)
        return false;
    G_O qvec = v0v1 * tvec;
    float v = (G_O::dot(rayo.direccion, qvec)) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    t = (G_O::dot(v0v2, qvec)) * invDet;
    normal.normalize();
    if (tipoColor == 1)
    {
        _color = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);
    }
    else if (tipoColor == 2)
    {
        float x, y;
        G_O p = rayo.punto + rayo.direccion * t;

        EncontrarPuntoInterpolado(p, v0, v1, v2, x, y, in);

        int indTxt = triTxt.tri[in].nFichero;

        int indice = (int)(x * (triTxt.txt[indTxt].width - 1)) + ((int)((1 - y) * (triTxt.txt[indTxt].height - 1))) * triTxt.txt[indTxt].width;

        _color = Color(triTxt.txt[indTxt].pixeles[indice]);
    }
    else
    {
        if (triTxt.txt.size() != 0)
        {
            float x, y;
            G_O p = rayo.punto + rayo.direccion * t;

            EncontrarPuntoInterpolado(p, v0, v1, v2, x, y, in);

            int indTxt = triTxt.tri[in].nFichero;

            int indice = (int)(x * (triTxt.txt[indTxt].width - 1)) + ((int)((1 - y) * (triTxt.txt[indTxt].height - 1))) * triTxt.txt[indTxt].width;

            _color = Color(triTxt.txt[indTxt].pixeles[indice]);
        }
        else
        {
            _color = color;
        }
    }

    return true;
}

bool Triangulo::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &_normal, const int in)
{
    G_O v0v1 = v1 - v0;
    G_O v0v2 = v2 - v0;
    G_O normal = v0v2 * v0v1;
    G_O pvec = v0v2 * rayo.direccion;
    float det = (G_O::dot(v0v1, pvec));

    if (det > -1e-9 && det < 1e-9)
        return false; // ray parallel to triangle

    float invDet = 1 / det;

    G_O tvec = rayo.punto - v0;
    float u = (G_O::dot(tvec, pvec)) * invDet;
    if (u < 0 || u > 1)
        return false;
    G_O qvec = v0v1 * tvec;
    float v = (G_O::dot(rayo.direccion, qvec)) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    t = (G_O::dot(v0v2, qvec)) * invDet;
    normal.normalize();
    _normal = normal;
    if (tipoColor == 1)
    {
        _color = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);
    }
    else if (tipoColor == 2)
    {
        float x, y;
        G_O p = rayo.punto + rayo.direccion * t;

        EncontrarPuntoInterpolado(p, v0, v1, v2, x, y, in);

        int indTxt = triTxt.tri[in].nFichero;

        int indice = (int)(x * (triTxt.txt[indTxt].width - 1)) + ((int)((1 - y) * (triTxt.txt[indTxt].height - 1))) * triTxt.txt[indTxt].width;

        _color = Color(triTxt.txt[indTxt].pixeles[indice]);
    }
    else
    {

        if (triTxt.tri[in].nFichero != -1)
        {

            float x, y;
            G_O p = rayo.punto + rayo.direccion * t;
            // cout << "funciona1?\n";
            EncontrarPuntoInterpolado(p, v0, v1, v2, x, y, in);
            int indTxt = triTxt.tri[in].nFichero;
            // cout << indTxt << "\n";

            int indice1 = (int)(x * (triTxt.txt[indTxt].width - 1));
            int indice2 = ((int)((1 - y) * (triTxt.txt[indTxt].height - 1))) * triTxt.txt[indTxt].width;
            int indice = indice1 + indice2;
            // cout << "funciona2?\n";
            // cout << triTxt.txt[indTxt].width << "\n";
            // cout << indice1 << "\n";
            // cout << triTxt.txt[indTxt].height << "\n";
            // cout << indice2 << "\n";
            // cout << triTxt.txt[indTxt].width * triTxt.txt[indTxt].height << "\n";
            // cout << indice << "\n";
            _color = Color(triTxt.txt[indTxt].pixeles[indice]);
            // cout << "funciona3?\n";
        }
        else
        {
            _color = color;
        }
    }

    return true;
}

bool Triangulo::interseccion(float &t, Color &_color, const Rayo rayo)
{
    G_O v0v1 = v1 - v0;
    G_O v0v2 = v2 - v0;
    G_O normal = v0v2 * v0v1;
    G_O pvec = v0v2 * rayo.direccion;
    float det = (G_O::dot(v0v1, pvec));

    float invDet = 1 / det;

    G_O tvec = rayo.punto - v0;
    float u = (G_O::dot(tvec, pvec)) * invDet;
    if (u < 0 || u > 1)
        return false;
    G_O qvec = v0v1 * tvec;
    float v = (G_O::dot(rayo.direccion, qvec)) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    t = (G_O::dot(v0v2, qvec)) * invDet;
    _color = color;
    return true;
}
