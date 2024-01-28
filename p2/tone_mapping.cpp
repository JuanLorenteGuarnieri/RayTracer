#include "tone_mapping.h"

// Esta función aplica clamping a los píxeles de una imagen, limitando los valores de color a un valor máximo.
vector<Pixel> clamping(vector<Pixel> &inicial, double valor_clamping, double &maximo)
{
    vector<Pixel> result;
    for (const Pixel &p : inicial) {
        Pixel pixel_modificado = p;
        if (p.r > valor_clamping){
            pixel_modificado.r = valor_clamping;
        }
        if (p.g > valor_clamping){
            pixel_modificado.g = valor_clamping;
        }
        if (p.b > valor_clamping){
            pixel_modificado.b = valor_clamping;
        }
        result.push_back(pixel_modificado);
    }
    if (maximo > valor_clamping){
        maximo = valor_clamping;
    }
    return result;
}

// Función para calcular el valor mínimo y máximo de píxeles en la imagen
void calcularMinimoMaximo(const vector<Pixel> &imagen, double &min, double &max, double maxColor)
{
    min = maxColor;
    max = 0;

    for (const Pixel &p : imagen){
        // Calcula el valor promedio de los componentes de color (rojo, verde y azul) en el píxel.
        double valor = (p.r + p.g + p.b) / 3;
        if (valor < min){
            min = valor;
        }
        if (valor > max){
            max = valor;
        }
    }
}

// Esta función realiza la ecualización en una imagen.
vector<Pixel> ecualizacion(const vector<Pixel> &imagen, double &maximo){
    double min, max;
    calcularMinimoMaximo(imagen, min, max, maximo);
    cout << "Min: " << min << endl;
    cout << "Max: " << max << endl;
    double factor = maximo / (max - min);

    vector<Pixel> imagenEcualizada;
    for (const Pixel &p : imagen){
        // Aplica la ecualización a los componentes de color de cada píxel.
        imagenEcualizada.push_back({(p.r - min) * factor, (p.g - min) * factor, (p.b - min) * factor});
    }
    return imagenEcualizada;
}

// Esta función combina la ecualización y el clamping en una imagen.
vector<Pixel> ecualizacion_campling(const vector<Pixel> &imagen, double valor_clamping, double &maximo){
    vector<Pixel> ecualizado = ecualizacion(imagen, maximo);
    return clamping(ecualizado, valor_clamping, maximo);
}

// Esta función aplica una corrección gamma a una imagen.
vector<Pixel> curva_gamma(const vector<Pixel> &imagen, double curva_gamma_, double &maximo){
    vector<Pixel> ecualizado = ecualizacion(imagen, maximo);
    vector<Pixel> result;

    for (const Pixel &p : imagen){
        // Aplica la corrección gamma a los componentes de color de cada píxel.
        result.push_back({pow(p.r / maximo, 1.0 / curva_gamma_) * maximo,
                          pow(p.g / maximo, 1.0 / curva_gamma_) * maximo,
                          pow(p.b / maximo, 1.0 / curva_gamma_) * maximo});
    }

    return result;
}

// Esta función combina la corrección gamma y el clamping en una imagen.
vector<Pixel> clamp_curva_gamma(const vector<Pixel> &imagen, double curva_gamma_, double valor_clamping, double &maximo){
    vector<Pixel> ecualizado = ecualizacion(imagen, maximo);
    vector<Pixel> ec_campl = clamping(ecualizado, valor_clamping, maximo);
    return curva_gamma(ec_campl, curva_gamma_, maximo);
}


vector<double> rgbToGray(const vector<Pixel> &imagen)
{
    vector<double> imagen_gris;
    for (const Pixel &p : imagen)
    {
        // Aplicar la fórmula de promedio ponderado para obtener la intensidad en escala de grises
        double intensidad_gris = 0.299 * p.r + 0.587 * p.g + 0.114 * p.b;
        imagen_gris.push_back(intensidad_gris);
    }
    return imagen_gris;
}


vector<Pixel> color_to_grey(const vector<Pixel> &imagenRGB) {

  vector<Pixel> imagenEscalaDeGrises;
    
    for (const Pixel &p : imagenRGB) {
        Pixel pixelEscalaDeGrises;
        double intensidad = 0.299 * p.r + 0.587 * p.g + 0.114 * p.b;
        pixelEscalaDeGrises.r = intensidad;
        pixelEscalaDeGrises.g = intensidad;
        pixelEscalaDeGrises.b = intensidad;
        imagenEscalaDeGrises.push_back(pixelEscalaDeGrises);
    }
    
    return imagenEscalaDeGrises;
}


vector<Pixel> Sobel(const vector<Pixel> &imagen, int ancho, int alto) {
    vector<Pixel> imagenEscalaDeGrises = color_to_grey(imagen);
    vector<Pixel> imagenBordes(ancho * alto);
    
    // Kernels Sobel
    int kernelGx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernelGy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    // Aplicar el operador Sobel
    for (int y = 1; y < alto - 1; y++) {
        for (int x = 1; x < ancho - 1; x++) {
            double gx = 0.0;
            double gy = 0.0;
            
            // Convolución con los kernels
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int pixelIndex = (y + i) * ancho + (x + j);
                    double intensidad = imagenEscalaDeGrises[pixelIndex].r;
                    gx += kernelGx[i + 1][j + 1] * intensidad;
                    gy += kernelGy[i + 1][j + 1] * intensidad;
                }
            }
            
            // Calcular el gradiente y asignar el resultado
            double gradiente = sqrt(gx * gx + gy * gy);
            imagenBordes[y * ancho + x].r = gradiente;
            imagenBordes[y * ancho + x].g = gradiente;
            imagenBordes[y * ancho + x].b = gradiente;
        }
    }
    
    return imagenBordes;
}

// Esta función convierte una imagen en formato HDR a formato RGB.
vector<Pixel> conversion_HDR_RGB(const vector<Pixel> &imagenHDR, double maximo, int resolucion){
    vector<Pixel> imagenRGB;

    for (const Pixel &pixelHDR : imagenHDR){
        Pixel pixelRGB;
        pixelRGB.r = (pixelHDR.r * maximo) / resolucion;
        pixelRGB.g = (pixelHDR.g * maximo) / resolucion;
        pixelRGB.b = (pixelHDR.b * maximo) / resolucion;
        imagenRGB.push_back(pixelRGB);
    }

    return imagenRGB;
}

// Esta función convierte una imagen en formato RGB a formato HDR.
vector<Pixel> conversion_RGB_HDR(const vector<Pixel> &imagenRGB, double maximo, int resolucion){
    vector<Pixel> imagenHDR;

    for (const Pixel &pixelRGB : imagenRGB){
        Pixel pixelHDR;
        pixelHDR.r = (pixelRGB.r * resolucion) / maximo;
        pixelHDR.g = (pixelRGB.g * resolucion) / maximo;
        pixelHDR.b = (pixelRGB.b * resolucion) / maximo;
        imagenHDR.push_back(pixelHDR);
    }

    return imagenHDR;
}


vector<Pixel> filtroGaussiano(const vector<Pixel> &imagen, int ancho, int alto) {
    // Tamaño del kernel Gaussiano (puedes ajustarlo, esto sería cosa de probar)
    int tamanoKernel = 5;
    int radio = tamanoKernel / 2;

    // Crear un kernel Gaussiano (puedes cambiarlo)
    double kernel[tamanoKernel][tamanoKernel] = {
        {0.0235, 0.0596, 0.0930, 0.0596, 0.0235},
        {0.0596, 0.1207, 0.1884, 0.1207, 0.0596},
        {0.0930, 0.1884, 0.2931, 0.1884, 0.0930},
        {0.0596, 0.1207, 0.1884, 0.1207, 0.0596},
        {0.0235, 0.0596, 0.0930, 0.0596, 0.0235}
    };

    vector<Pixel> imagenSuavizada(imagen);

    for (int y = radio; y < alto - radio; ++y) {
        for (int x = radio; x < ancho - radio; ++x) {
            double nuevaR = 0.0, nuevaG = 0.0, nuevaB = 0.0;

            for (int i = -radio; i <= radio; ++i) {
                for (int j = -radio; j <= radio; ++j) {
                    int pixelIdx = (y + i) * ancho + (x + j);
                    nuevaR += imagen[pixelIdx].r * kernel[i + radio][j + radio];
                    nuevaG += imagen[pixelIdx].g * kernel[i + radio][j + radio];
                    nuevaB += imagen[pixelIdx].b * kernel[i + radio][j + radio];
                }
            }

            int pixelIdx = y * ancho + x;
            imagenSuavizada[pixelIdx].r = static_cast<int>(nuevaR);
            imagenSuavizada[pixelIdx].g = static_cast<int>(nuevaG);
            imagenSuavizada[pixelIdx].b = static_cast<int>(nuevaB);
        }
    }

    return imagenSuavizada;
}