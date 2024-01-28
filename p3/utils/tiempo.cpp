#include "tiempo.h"

// Función para obtener una representación en forma de string del tiempo en días, horas, minutos y segundos.
string obtenerTiempo(int segundos)
{
  // Calculamos días, horas, minutos y segundos a partir de los segundos totales.
  int dias = segundos / 86400;           // 86400 segundos en un día
  int horas = (segundos % 86400) / 3600; // 3600 segundos en una hora
  int minutos = (segundos % 3600) / 60;  // 60 segundos en un minuto
  int segundosRestantes = segundos % 60;

  // Determinamos si debemos mostrar días, horas y minutos (si son mayores que cero).
  bool mostrarDias = dias > 0;
  bool mostrarHoras = horas > 0 || mostrarDias;
  bool mostrarMinutos = minutos > 0 || mostrarHoras;

  // Creamos un string para almacenar la representación del tiempo.
  string tiempoString;

  // Agregamos los componentes de tiempo al string.
  if (mostrarDias)
  {
    tiempoString += to_string(dias) + "d ";
  }

  if (mostrarHoras)
  {
    tiempoString += to_string(horas) + "h ";
  }

  if (mostrarMinutos)
  {
    tiempoString += to_string(minutos) + "m ";
  }

  tiempoString += to_string(segundosRestantes) + "s";

  return tiempoString;
}

// Función para mostrar una barra de progreso
void mostrarBarraProgreso(int progress, int duration, int estimated_seconds_remaining)
{
  int barWidth = 50; // Ancho total de la barra de progreso
  float progressRatio = static_cast<float>(progress) / 100.0f;
  int barLength = static_cast<int>(progressRatio * barWidth);

  cout << "[";
  for (int i = 0; i < barWidth; ++i)
  {
    if (i < barLength)
    {
      cout << "\x1B[32m"; // Cambiar el color del texto a verde
      cout << "░";        // Carácter para indicar el progreso
    }
    else
    {
      cout << "\x1B[31m";
      cout << "▒"; // Carácter en blanco para la parte restante
    }
  }
  cout << "\x1B[0m"; // Restaurar el color del texto a su estado normal
  cout << "] " << progress << "% [ ⏳ " << obtenerTiempo(duration) << " | ⏰ " << obtenerTiempo(estimated_seconds_remaining) << " ]"
       << "\r";
  cout.flush(); // Limpiar el búfer de salida para mostrar la barra de progreso
}
