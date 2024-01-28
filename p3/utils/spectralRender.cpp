#include <iostream>
#include <vector>
#include <cmath>

// Define una función espectral de ejemplo (puedes usar tus propios valores).
std::vector<double> spectralData = {0.8, 0, 0, 0, 0, 0, 0, 0};

// Curvas estándar de CIE para RGB (valores normalizados)
std::vector<double> CIE_X = {0.490, 0.255, 0.158, 0.101, 0.065, 0.042, 0.026, 0.016};
std::vector<double> CIE_Y = {0.389, 0.813, 0.988, 1.000, 0.859, 0.676, 0.478, 0.335};
std::vector<double> CIE_Z = {0.121, 0.236, 0.437, 0.637, 0.747, 0.678, 0.502, 0.348};

// Función para convertir la representación espectral en RGB
std::vector<double> convertToRGB(const std::vector<double> &spectralData)
{
  std::vector<double> RGB(3, 0.0);
  for (int i = 0; i < spectralData.size(); i++)
  {
    RGB[0] += spectralData[i] * CIE_X[i];
    RGB[1] += spectralData[i] * CIE_Y[i];
    RGB[2] += spectralData[i] * CIE_Z[i];
  }
  return RGB;
}

int main()
{
  // Convertir representación espectral a RGB
  std::vector<double> RGB = convertToRGB(spectralData);

  // Asegúrate de que los valores RGB estén en el rango [0, 1]
  for (int i = 0; i < 3; i++)
  {
    if (RGB[i] < 0.0)
      RGB[i] = 0.0;
    if (RGB[i] > 1.0)
      RGB[i] = 1.0;
  }

  // Escala los valores RGB al rango [0, 255] para su visualización
  std::vector<int> RGB_255(3);
  for (int i = 0; i < 3; i++)
  {
    RGB_255[i] = static_cast<int>(RGB[i] * 255.0);
  }

  // Imprime los valores RGB
  std::cout << "RGB (0-255): R = " << RGB_255[0] << ", G = " << RGB_255[1] << ", B = " << RGB_255[2] << std::endl;

  return 0;
}
