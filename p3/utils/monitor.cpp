#include "monitor.h"

Monitor::Monitor(int _nTotal, int _nCompletados, bool _mostrarProgreso, std::chrono::_V2::system_clock::time_point _start_time)
{
  nTotal = _nTotal;
  n = _nCompletados;
  mostrarProgreso = _mostrarProgreso;
  previousProgress = 0;
  start_time = _start_time;
}

void Monitor::increaseN()
{
  unique_lock<mutex> lck(mtxMonitor);
  n++;
  if (mostrarProgreso)
  {

    // Calcular el porcentaje actual
    int progress = (n * 100) / nTotal;

    // Verificar si se ha alcanzado un nuevo intervalo de porcentaje
    if ((progress % 2 == 0 && progress != previousProgress))
    {
      auto current_time = high_resolution_clock::now();
      int duration = duration_cast<seconds>(current_time - start_time).count();

      // Calcular el tiempo estimado restante
      int estimated_seconds_remaining = 0;
      if (progress != 0)
      {
        estimated_seconds_remaining = ((duration * 100) / progress - duration);
      }

      mostrarBarraProgreso(progress, duration, estimated_seconds_remaining);
      previousProgress = progress;
    }
  }
}
