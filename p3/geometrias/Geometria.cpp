#include "Geometria.h"

bool Geometria::interseccion(float &t, Color &_color, const Rayo rayo)
{
  return false;
}

bool Geometria::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor)
{
  return false;
}

bool Geometria::interseccion(float &t, Color &_color, const Rayo rayo, const int tipoColor, G_O &normal)
{
  return false;
}
// Función para calcular un vector perpendicular manteniendo fijas las componentes X y Z
G_O Geometria::calcularVectorPerpendicular(const G_O &vectorOriginal)
{
  G_O vectorPerpendicular;

  // Mantén fijas las componentes X y Z
  vectorPerpendicular.x = vectorOriginal.x;
  vectorPerpendicular.z = vectorOriginal.z;

  // Calcula la componente Y para que el vector sea perpendicular al original
  // Asegúrate de que el vector resultante tenga una longitud de 1
  float longitudXY = std::sqrt(vectorPerpendicular.x * vectorPerpendicular.x + vectorPerpendicular.z * vectorPerpendicular.z);
  vectorPerpendicular.y = -vectorOriginal.x / longitudXY; // Puedes usar la componente X o Z para obtener la perpendicular.

  return vectorPerpendicular;
}