#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

struct Pixel
{
  int r, g, b;
};

int main()
{
  ifstream input("a.ppm");
  if (!input)
  {
    cerr << "No se pudo abrir el archivo a.ppm" << endl;
    return 1;
  }
  string linea;
  string part1, part2;
  string imagen;
  int valorClamping = 2;
  float maxColorMemory;

  while (getline(input, linea))
  {
    istringstream ss(linea);
    getline(ss, part1, '#');
    getline(ss, part2, '#');
    imagen = imagen + part1;
    if (part2.find("MAX=") == 0)
    {
      istringstream ss(part2);
      getline(ss, part2, '=');
      getline(ss, part2, '=');
      maxColorMemory = stof(part2);
      cout << "MaxColorMemory = " << maxColorMemory << endl;
    }
    // cout << part1 << endl;
    // cout << part2 << endl;
  }

  regex pattern("\\s+");
  imagen = regex_replace(imagen, pattern, " ");

  istringstream ss(imagen);

  string format;
  string width1, height1, maxColor1;
  getline(ss, format, ' ');
  getline(ss, width1, ' ');
  getline(ss, height1, ' ');
  getline(ss, maxColor1, ' ');

  int width = stoi(width1);
  int height = stoi(height1);
  int maxColor = stoi(maxColor1);

  cout << "Formato:" << format << endl;
  cout << "Width:" << width << endl;
  cout << "Height:" << height << endl;
  cout << "MaxColor:" << maxColor << endl;

  Pixel image[height][width];

  string r, g, b;
  // Leer los valores de los píxeles
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      getline(ss, r, ' ');
      getline(ss, g, ' ');
      getline(ss, b, ' ');
      image[i][j].r = stoi(r);
      image[i][j].g = stoi(g);
      image[i][j].b = stoi(b);
    }
  }

  // Aplicar el operador de clamping
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image[i][j].r > valorClamping)
        image[i][j].r = valorClamping;
      if (image[i][j].g > valorClamping)
        image[i][j].g = valorClamping;
      if (image[i][j].b > valorClamping)
        image[i][j].b = valorClamping;
    }
  }

  // Guardar la imagen resultante en un nuevo archivo PPM
  ofstream output("output.ppm");
  if (!output)
  {
    cerr << "No se pudo crear el archivo de salida" << endl;
    return 1;
  }
  output << format << endl;
  output << "#MAX=" << maxColorMemory << endl;
  output << width << " " << height << endl;
  output << maxColor << endl;

  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      output << image[i][j].r << " " << image[i][j].g << " " << image[i][j].b << "   ";
    }
    output << endl;
  }

  cout << "Tone mapping completado. La imagen resultante se ha guardado en output.ppm" << endl;

  return 0;
}
