#include "Toro.h"

Toro::Toro(G_O _normal, G_O _centro, float _R, float _r, Color _color, BRDF _brdf)
{
  brdf = _brdf;
  normal = _normal;
  centro = _centro;
  R = _R;
  r = _r;
  normal.normalize();

  normalRGB = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);

  color = _color;
}

Toro::Toro(G_O _normal, G_O _centro, float _R, float _r, Color _color, BRDF _brdf, int tipoLuz, Color _potencia)
{
  brdf = _brdf;
  normal = _normal;
  centro = _centro;
  R = _R;
  r = _r;
  normal.normalize();

  normalRGB = Color(abs(normal.x) * 255, abs(normal.y) * 255, abs(normal.z) * 255);

  color = _color;
  luz = tipoLuz;
  potencia = _potencia;
}

G_O computeNormal(G_O p, float R, float r)
{
  G_O normal;
  double param_squared = R * R + r * r;

  double x = p.x;
  double y = p.y;
  double z = p.z;
  double sum_squared = x * x + y * y + z * z;

  normal.x = 4.0 * x * (sum_squared - param_squared);
  normal.y = 4.0 * y * (sum_squared - param_squared + 2.0 * R * R);
  normal.z = 4.0 * z * (sum_squared - param_squared);

  return (normal);
}

G_O calcularNormal(G_O A, G_O B, double E)
{

  // Calcula el vector AB
  G_O AB = B - A;
  AB.y = 0;

  // Normaliza el vector AB
  AB.normalize();
  AB = AB * E;
  AB = B - AB;
  AB.normalize();

  return AB;
}

int SolveQuadric(double c[], double s[])
{
  double p, q, D;

  /* normal form: x^2 + px + q = 0 */

  p = c[1] / (2 * c[2]);
  q = c[0] / c[2];

  D = p * p - q;

  if (D > -1e-20 && D < 1e-20)
  {
    s[0] = -p;
    return 1;
  }
  else if (D > 0)
  {
    double sqrt_D = sqrt(D);

    s[0] = sqrt_D - p;
    s[1] = -sqrt_D - p;
    return 2;
  }
  else /* if (D < 0) */
    return 0;
}

int solveCubic(double c[], double s[])
{
  int i, num;
  double sub;
  double A, B, C;
  double sq_A, p, q;
  double cb_p, D;

  /* normal form: x^3 + Ax^2 + Bx + C = 0 */

  A = c[2] / c[3];
  B = c[1] / c[3];
  C = c[0] / c[3];

  /*  substitute x = y - A/3 to eliminate quadric term:
  x^3 +px + q = 0 */

  sq_A = A * A;
  p = 1.0 / 3 * (-1.0 / 3 * sq_A + B);
  q = 1.0 / 2 * (2.0 / 27 * A * sq_A - 1.0 / 3 * A * B + C);

  /* use Cardano's formula */

  cb_p = p * p * p;
  D = q * q + cb_p;

  if (D > -1e-20 && D < 1e-20)
  {
    if (q > -1e-20 && q < 1e-20)
    { /* one triple solution */
      s[0] = 0;
      num = 1;
    }
    else
    { /* one single and one double solution */
      double u = cbrt(-q);
      s[0] = 2 * u;
      s[1] = -u;
      num = 2;
    }
  }
  else if (D < 0)
  { /* Casus irreducibilis: three real solutions */
    double phi = 1.0 / 3 * acos(-q / sqrt(-cb_p));
    double t = 2 * sqrt(-p);

    s[0] = t * cos(phi);
    s[1] = -t * cos(phi + PI / 3);
    s[2] = -t * cos(phi - PI / 3);
    num = 3;
  }
  else
  { /* one real solution */
    double sqrt_D = sqrt(D);
    double u = cbrt(sqrt_D - q);
    double v = -cbrt(sqrt_D + q);

    s[0] = u + v;
    num = 1;
  }

  /* resubstitute */

  sub = 1.0 / 3 * A;

  for (i = 0; i < num; ++i)
    s[i] -= sub;

  return num;
}

int solveQuartic(double c[], double s[])
{
  double coeffs[4];
  for (size_t i = 0; i < 4; i++)
  {
    coeffs[i] = 0;
  }

  double z, u, v, sub;
  double A, B, C, D;
  double sq_A, p, q, r;
  int i, num;

  /* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

  A = c[3] / c[4];
  B = c[2] / c[4];
  C = c[1] / c[4];
  D = c[0] / c[4];

  /*  substitute x = y - A/4 to eliminate cubic term:
  x^4 + px^2 + qx + r = 0 */

  sq_A = A * A;
  p = -3.0 / 8 * sq_A + B;
  q = 1.0 / 8 * sq_A * A - 1.0 / 2 * A * B + C;
  r = -3.0 / 256 * sq_A * sq_A + 1.0 / 16 * sq_A * B - 1.0 / 4 * A * C + D;

  if (r > -1e-20 && r < 1e-20)
  {
    /* no absolute term: y(y^3 + py + q) = 0 */

    coeffs[0] = q;
    coeffs[1] = p;
    coeffs[2] = 0;
    coeffs[3] = 1;

    num = solveCubic(coeffs, s);

    s[num++] = 0;
  }
  else
  {
    /* solve the resolvent cubic ... */

    coeffs[0] = 1.0 / 2 * r * p - 1.0 / 8 * q * q;
    coeffs[1] = -r;
    coeffs[2] = -1.0 / 2 * p;
    coeffs[3] = 1;

    int shouldBeOne = solveCubic(coeffs, s);

    /* ... and take the one real solution ... */

    z = s[0];

    /* ... to build two quadric equations */

    u = z * z - r;
    v = 2 * z - p;

    if (u > -1e-20 && u < 1e-20)
      u = 0;
    else if (u > 0)
      u = sqrt(u);
    else
      return 0;

    if (v > -1e-20 && v < 1e-20)
      v = 0;
    else if (v > 0)
      v = sqrt(v);
    else
      return 0;

    coeffs[0] = z - u;
    coeffs[1] = q < 0 ? -v : v;
    coeffs[2] = 1;

    num = SolveQuadric(coeffs, s);

    coeffs[0] = z + u;
    coeffs[1] = q < 0 ? v : -v;
    coeffs[2] = 1;

    int tempNum = 0;
    double tempSols[2];
    tempSols[0] = 0;
    tempSols[1] = 0;
    tempNum = SolveQuadric(coeffs, tempSols);

    for (int in = 0; in < tempNum; in++)
    {
      s[in + num] = tempSols[in];
    }
    num += tempNum;
  }

  /* resubstitute */

  sub = 1.0 / 4 * A;

  for (i = 0; i < num; ++i)
    s[i] -= sub;

  return num;
}

bool Toro::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &_normal)
{
  double x1 = rayo.punto.x - centro.x;
  double y1 = rayo.punto.y - centro.y;
  double z1 = rayo.punto.z - centro.z;
  double d1 = rayo.direccion.x;
  double d2 = rayo.direccion.y;
  double d3 = rayo.direccion.z;

  // define the coefficients of the quartic equation
  double sum_d_sqrd = d1 * d1 + d2 * d2 + d3 * d3;
  double e = x1 * x1 + y1 * y1 + z1 * z1 - R * R - r * r;
  double f = x1 * d1 + y1 * d2 + z1 * d3;
  double four_a_sqrd = 4.0 * R * R;

  double E = e * e - four_a_sqrd * (r * r - y1 * y1); // constant term
  double D = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
  double C = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
  double B = 4.0 * sum_d_sqrd * f;
  double A = sum_d_sqrd * sum_d_sqrd; // coefficient of t^4

  double coeffs[5]; // coefficient array for the quartic equation
  double roots[4];  // solution array for the quartic equation
  coeffs[0] = E;
  coeffs[1] = D;
  coeffs[2] = C;
  coeffs[3] = B;
  coeffs[4] = A;

  bool intersected = false;

  // find roots of the quartic equation
  int num_real_roots = solveQuartic(coeffs, roots);

  t = 1.0e10;

  if (num_real_roots == 0)
  {
    return (false);
  }

  // find the smallest root greater than kEpsilon, if any
  // the roots array is not sorted
  for (int j = 0; j < num_real_roots; j++)
  {
    if (roots[j] > 1e-20)
    {
      intersected = true;
      if (roots[j] < t)
      {
        t = roots[j];
      }
    }
  }

  if (!intersected)
  {
    return (false);
  }

  if (tipoColor == 1)
  {
    G_O p = rayo.punto + rayo.direccion * t;
    G_O n = calcularNormal(centro, p, R);
    _normal = n;
    _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
  }
  else
  {
    _color = color;
    G_O p = rayo.punto + rayo.direccion * t;
    G_O n = calcularNormal(centro, p, R);
    _normal = n;
  }
  return true;
}

bool Toro::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor)
{
  double x1 = rayo.punto.x - centro.x;
  double y1 = rayo.punto.y - centro.y;
  double z1 = rayo.punto.z - centro.z;
  double d1 = rayo.direccion.x;
  double d2 = rayo.direccion.y;
  double d3 = rayo.direccion.z;

  // define the coefficients of the quartic equation
  double sum_d_sqrd = d1 * d1 + d2 * d2 + d3 * d3;
  double e = x1 * x1 + y1 * y1 + z1 * z1 - R * R - r * r;
  double f = x1 * d1 + y1 * d2 + z1 * d3;
  double four_a_sqrd = 4.0 * R * R;

  double E = e * e - four_a_sqrd * (r * r - y1 * y1); // constant term
  double D = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
  double C = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
  double B = 4.0 * sum_d_sqrd * f;
  double A = sum_d_sqrd * sum_d_sqrd; // coefficient of t^4

  double coeffs[5]; // coefficient array for the quartic equation
  double roots[4];  // solution array for the quartic equation
  coeffs[0] = E;
  coeffs[1] = D;
  coeffs[2] = C;
  coeffs[3] = B;
  coeffs[4] = A;

  bool intersected = false;

  // find roots of the quartic equation
  int num_real_roots = solveQuartic(coeffs, roots);

  t = 1.0e10;

  if (num_real_roots == 0)
  {
    return (false);
  }

  // find the smallest root greater than kEpsilon, if any
  // the roots array is not sorted
  for (int j = 0; j < num_real_roots; j++)
  {
    if (roots[j] > 1e-20)
    {
      intersected = true;
      if (roots[j] < t)
      {
        t = roots[j];
      }
    }
  }

  if (!intersected)
  {
    return (false);
  }

  if (tipoColor == 1)
  {
    G_O n = rayo.punto + rayo.direccion * t;
    n = calcularNormal(centro, n, R);
    _color = Color(abs(n.x) * 255, abs(n.y) * 255, abs(n.z) * 255);
  }
  else
  {
    _color = color;
  }
  return true;
}

bool Toro::interseccion(float &t, Color &_color, const Rayo rayo)
{
  double x1 = rayo.punto.x;
  double y1 = rayo.punto.y;
  double z1 = rayo.punto.z;
  double d1 = rayo.direccion.x;
  double d2 = rayo.direccion.y;
  double d3 = rayo.direccion.z;

  // define the coefficients of the quartic equation
  double sum_d_sqrd = d1 * d1 + d2 * d2 + d3 * d3;
  double e = x1 * x1 + y1 * y1 + z1 * z1 - R * R - r * r;
  double f = x1 * d1 + y1 * d2 + z1 * d3;
  double four_a_sqrd = 4.0 * R * R;

  double E = e * e - four_a_sqrd * (r * r - y1 * y1); // constant term
  double D = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
  double C = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
  double B = 4.0 * sum_d_sqrd * f;
  double A = sum_d_sqrd * sum_d_sqrd; // coefficient of t^4

  double coeffs[5]; // coefficient array for the quartic equation
  double roots[4];  // solution array for the quartic equation
  coeffs[0] = E;
  coeffs[1] = D;
  coeffs[2] = C;
  coeffs[3] = B;
  coeffs[4] = A;

  bool intersected = false;

  // find roots of the quartic equation
  int num_real_roots = solveQuartic(coeffs, roots);

  t = 1.0e10;

  if (num_real_roots == 0)
  {
    return (false);
  }

  // find the smallest root greater than kEpsilon, if any
  // the roots array is not sorted
  for (int j = 0; j < num_real_roots; j++)
  {
    if (roots[j] > 1e-20)
    {
      intersected = true;
      if (roots[j] < t)
      {
        t = roots[j];
      }
    }
  }

  if (!intersected)
  {
    return (false);
  }

  _color = color;
  return true;
}