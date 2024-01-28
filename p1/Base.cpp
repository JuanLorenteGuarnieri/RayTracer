#include "Base.h"

Base::Base()
{
  tipo_ = TRANSLACION;
}

// Date constructor
Base::Base(float _x, float _y, float _z, const int tipo)
{
  tipo_ = tipo;
  if (tipo == TRANSLACION)
  {
    matrix[0][3] = _x;
    matrix[1][3] = _y;
    matrix[2][3] = _z;
  }
  else if (tipo == ESCALA)
  {
    matrix[0][0] = _x;
    matrix[1][1] = _y;
    matrix[2][2] = _z;
  }
}

Base::Base(float _theta, const int eje, const int tipo)
{
  tipo_ = tipo;
  eje_ = eje;
  if (eje == EJE_X)
  {
    matrix[1][1] = cos(_theta);
    matrix[1][2] = -sin(_theta);
    matrix[2][1] = sin(_theta);
    matrix[2][2] = cos(_theta);
  }
  else if (eje == EJE_Y)
  {
    matrix[0][0] = cos(_theta);
    matrix[0][2] = sin(_theta);
    matrix[2][0] = -sin(_theta);
    matrix[2][2] = cos(_theta);
  }
  else if (eje == EJE_Z)
  {
    matrix[0][0] = cos(_theta);
    matrix[0][1] = -sin(_theta);
    matrix[1][0] = sin(_theta);
    matrix[1][1] = cos(_theta);
  }
}

Base::Base(float _ux, float _uy, float _uz, float _vx, float _vy, float _vz, float _wx, float _wy, float _wz, float _ox, float _oy, float _oz, const int tipo)
{
  tipo_ = tipo;
  matrix[0][0] = _ux;
  matrix[0][1] = _vx;
  matrix[0][2] = _wx;
  matrix[0][3] = _ox;
  matrix[1][0] = _uy;
  matrix[1][1] = _vy;
  matrix[1][2] = _wy;
  matrix[1][3] = _oy;
  matrix[2][0] = _uz;
  matrix[2][1] = _vz;
  matrix[2][2] = _wz;
  matrix[2][3] = _oz;
}

void Base::print_base() const
{
  cout << "("
       << ")" << endl;
}

ostream &operator<<(ostream &os, const Base &obj)
{
  for (int i = 0; i < 4; ++i)
  {
    if (i == 0)
    {
      os << " / ";
    }
    else if (i == 3)
    {
      os << " \\ ";
    }
    else
    {
      os << " | ";
    }

    for (int j = 0; j < 4; ++j)
    {
      os << fixed << setprecision(1) << obj.matrix[i][j];
      if (j < 3)
      {
        os << "   ";
      }
    }

    if (i == 0)
    {
      os << " \\" << endl;
    }
    else if (i == 3)
    {
      os << " /" << endl;
    }
    else
    {
      os << " |" << endl;
    }
  }
  return os;
}

bool Base::operator==(const Base &b) const
{
  bool isEqual = true;
  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      if (matrix[i][j] != b.matrix[i][j])
      {
        isEqual = false;
        return false;
      }
    }
  }
  return true;
}

//***********************************************************************************
// INVERSA
//***********************************************************************************
float getDeterminant(const vector<vector<float>> vect)
{
  if (vect.size() != vect[0].size())
  {
    throw runtime_error("Matrix is not quadratic");
  }

  int dimension = vect.size();

  if (dimension == 0)
  {
    return 1;
  }

  if (dimension == 1)
  {
    return vect[0][0];
  }

  // Formula for 2x2-matrix
  if (dimension == 2)
  {
    return vect[0][0] * vect[1][1] - vect[0][1] * vect[1][0];
  }

  float result = 0;
  int sign = 1;
  for (int i = 0; i < dimension; i++)
  {

    // Submatrix
    vector<vector<float>> subVect(dimension - 1, vector<float>(dimension - 1));
    for (int m = 1; m < dimension; m++)
    {
      int z = 0;
      for (int n = 0; n < dimension; n++)
      {
        if (n != i)
        {
          subVect[m - 1][z] = vect[m][n];
          z++;
        }
      }
    }

    // recursive call
    result = result + sign * vect[0][i] * getDeterminant(subVect);
    sign = -sign;
  }

  return result;
}

vector<vector<float>> getTranspose(const vector<vector<float>> matrix1)
{

  // Transpose-matrix: height = width(matrix), width = height(matrix)
  vector<vector<float>> solution(matrix1[0].size(), vector<float>(matrix1.size()));

  // Filling solution-matrix
  for (size_t i = 0; i < matrix1.size(); i++)
  {
    for (size_t j = 0; j < matrix1[0].size(); j++)
    {
      solution[j][i] = matrix1[i][j];
    }
  }
  return solution;
}

vector<vector<float>> getCofactor(const vector<vector<float>> vect)
{
  if (vect.size() != vect[0].size())
  {
    throw runtime_error("Matrix is not quadratic");
  }

  vector<vector<float>> solution(vect.size(), vector<float>(vect.size()));
  vector<vector<float>> subVect(vect.size() - 1, vector<float>(vect.size() - 1));

  for (size_t i = 0; i < vect.size(); i++)
  {
    for (size_t j = 0; j < vect[0].size(); j++)
    {

      int p = 0;
      for (size_t x = 0; x < vect.size(); x++)
      {
        if (x == i)
        {
          continue;
        }
        int q = 0;

        for (size_t y = 0; y < vect.size(); y++)
        {
          if (y == j)
          {
            continue;
          }

          subVect[p][q] = vect[x][y];
          q++;
        }
        p++;
      }
      solution[i][j] = pow(-1, i + j) * getDeterminant(subVect);
    }
  }
  return solution;
}

vector<vector<float>> getInverse(const vector<vector<float>> vect)
{
  if (getDeterminant(vect) == 0)
  {
    throw runtime_error("Determinant is 0");
  }
  float d = 1.0 / getDeterminant(vect);
  vector<vector<float>> solution(vect.size(), vector<float>(vect.size()));

  if (vect.size() == 1)
  {
    vector<float> ans = {0};
    ans[0] = 1.0 / getDeterminant(vect);
    solution[0] = ans;
    return solution;
  }

  for (size_t i = 0; i < vect.size(); i++)
  {
    for (size_t j = 0; j < vect.size(); j++)
    {
      solution[i][j] = vect[i][j];
    }
  }

  solution = getTranspose(getCofactor(solution));

  for (size_t i = 0; i < vect.size(); i++)
  {
    for (size_t j = 0; j < vect.size(); j++)
    {
      solution[i][j] *= d;
    }
  }

  return solution;
}

void Base::get_Inverse(Base &a, Base &b)
{
  vector<vector<float>> v(MAX_DIMENSION, vector<float>(MAX_DIMENSION));

  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      v[i][j] = b.matrix[i][j];
    }
  }

  vector<vector<float>> resultVector(MAX_DIMENSION, vector<float>(MAX_DIMENSION));
  resultVector = getInverse(v);

  // Copia los valores desde resultVector a matrix
  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      a.matrix[i][j] = resultVector[i][j];
    }
  }
}
