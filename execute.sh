#!/bin/bash

# Define the folder
folder="./renders"

# Check if the folder exists, if not, create it
if [ ! -d "$folder" ]; then
  mkdir "$folder"
fi

# Compile and execute the program
g++ -std=c++17 ./p3/utils/Semaphore_V4.cpp ./p3/utils/monitor.cpp ./p3/utils/tiempo.cpp ./p3/utils/luzGlobal.cpp ./p1/G_O.cpp ./p2/tone_mapping.cpp ./p1/Base.cpp ./p3/BRDF.cpp ./p3/escenas.cpp ./p3/pLuz.cpp ./p3/Color.cpp ./p3/geometrias/Toro.cpp ./p3/geometrias/Elipsoide.cpp ./p3/geometrias/Disco.cpp ./p3/geometrias/Cilindro.cpp ./p3/geometrias/Cono.cpp ./p3/geometrias/Triangulo.cpp ./p3/geometrias/Plano.cpp ./p3/geometrias/Geometria.cpp ./p3/geometrias/Esfera.cpp ./p2/ficheros.cpp ./p3/photon_mapping.cpp ./p3/Camara.cpp ./p3/Rayo.cpp ./p3/photon.cpp ./main.cpp ./p3/BVH/BVH.cpp -o render -pthread
./render
