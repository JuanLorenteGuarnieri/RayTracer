#include "./p3/escenas.h"

/*
To compile and execute the program, you can use execute.bat or use the following command in this folder
Compilation:
g++ -std=c++17 ./p3/utils/Semaphore_V4.cpp ./p3/utils/monitor.cpp ./p3/utils/tiempo.cpp ./p3/utils/luzglobal.cpp ./p1/G_O.cpp ./p2/tone_mapping.cpp ./p1/Base.cpp ./p3/BRDF.cpp ./p3/escenas.cpp ./p3/pLuz.cpp ./p3/Color.cpp ./p3/geometrias/Toro.cpp ./p3/geometrias/Elipsoide.cpp ./p3/geometrias/Disco.cpp ./p3/geometrias/Cilindro.cpp ./p3/geometrias/Cono.cpp ./p3/geometrias/Triangulo.cpp ./p3/geometrias/Plano.cpp ./p3/geometrias/Geometria.cpp ./p3/geometrias/Esfera.cpp ./p2/ficheros.cpp ./p3/photon_mapping.cpp ./p3/Camara.cpp ./p3/Rayo.cpp ./p3/Photon.cpp ./main.cpp ./p3/BVH/BVH.cpp -o render -pthread
*/

int main()
{
    // cornellBox(COLOR_CON_NORMALES, 1024, 1024, 64);
    cornellBoxLUZ(COLOR_CON_LUCES, 768, 768, 80);
    return 0;
}