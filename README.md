# RENDERER | Computer Graphics

## Compilation and Execution
To execute the renderer, all you need to do is run execute.bat (Windows) or execute.sh (Linux, it has not been tested if it works), and it will automatically compile and execute.

C++17 is required to run the renderer.

## Features

In this work, both path tracing and photon mapping are included. To switch between them, you only need to change the method of the camera that is called to start rendering in the respective scene
(camara.crearImagenPhotonMapping() | camara.crearImagenPathTracer())

• Path Tracer

• PhotonMapping

• Primitives: Plane, Sphere, Cilinder, Ellipsoids, Disks, Triangles and Torus

• Bounding Volume Hierarchies (BVH) for Triangles

• Fresnel Effects (Realistic coefficients modulating specular/refractive coefficients ks and kt)

• Import model 3D (.ply)

• Textures (images in .ppm format)

• Parallelization (The parallelization will likely consume all the resources of the PC)

• Camera effects (FOV, Focal Length, Aperture)

• Visualize objects normals([x, y, z] to RGB)

## Notes
• There needs to be a folder named /renders for saving the resulting renders. Usually, for each render, 2 images are saved: one in HDR and another with the color range to open and visualize it.

• When importing files, the program can only read .ppm format for images and .ply for 3D models. It is important to note that the input files must be in ASCII and not in binary.

• If you want to check the functionality of the tone mapping, it is necessary to re-download the reference images from the corresponding practice into /images (the files have been deleted due to space reasons).