#include "ficheros.h"

// Esta función verifica si una línea comienza con "#MAX=" y extrae el número máximo si es así.
bool check_delimitador(string linea, double &maximo)
{
    bool check = false;
    if (linea.find("#MAX=") == 0)
    {
        size_t posicion_delimitador = linea.find("=");
        string numero_str = linea.substr(posicion_delimitador + 1);
        try
        {
            maximo = stod(numero_str);
            check = true;
        }
        catch (const std::exception &e)
        {
            cerr << "Error al convertir el número: " << e.what() << std::endl;
        }
    }
    return check;
}

// Esta función elimina cualquier comentario en una línea, es decir, elimina todo después del símbolo "#" y devuelve la parte útil.
string ignorar_comentario(string linea)
{
    size_t posicion_delimitador = linea.find("#");
    string util;
    if (posicion_delimitador != string::npos)
    {
        util = linea.substr(0, posicion_delimitador);
    }
    else
    {
        util = linea;
    }
    return util;
}

// Esta función lee los datos de un archivo de imagen.
void leerFichero(string nombreFichero, string &formato, int &width, int &height, double &maximo, int &resolucion, vector<Pixel> &pixeles)
{
    ifstream archivo(nombreFichero);

    if (!archivo)
    {
        cerr << "No se pudo abrir el archivo: " << nombreFichero << "': " << strerror(errno) << endl;
        return;
    }
    // Variables para almacenar cada línea del archivo y el número leído
    string linea;
    bool hayMaximo = false;

    // Lee el formato del archivo (la primera línea) y elimina comentarios si los hay.
    getline(archivo, linea);
    formato = ignorar_comentario(linea);

    // Lee la segunda línea y verifica si contiene un delimitador "#MAX=", si es así, obtiene el número máximo.
    getline(archivo, linea);
    if (linea.find("#") == 0)
    {
        hayMaximo = check_delimitador(linea, maximo);
        getline(archivo, linea);
    }

    if (linea.find("#") == 0)
    {
        hayMaximo = check_delimitador(linea, maximo);
        getline(archivo, linea);
    }

    // Lee la siguiente línea y obtiene el ancho y el alto de la imagen.
    istringstream stream1(ignorar_comentario(linea));
    stream1 >> width >> height;

    // Lee la cuarta línea y obtiene la resolución.
    getline(archivo, linea);
    istringstream stream2(ignorar_comentario(linea));
    stream2 >> resolucion;

    // Lee los píxeles del archivo y los almacena en el vector "pixeles".
    while (!archivo.eof())
    {
        Pixel pixel;
        if (archivo >> pixel.r >> pixel.g >> pixel.b)
        {
            pixeles.push_back(pixel);
        }
    }
}

// Esta función escribe los datos de la imagen en un archivo de destino.
void guardarImagenPPM(string ficheroDestino, vector<Pixel> &pixeles, string formato, int width, int height)
{
    double maximo = pixelMax(pixeles);
    double resolucion = 65530;
    ofstream output(ficheroDestino + ".ppm");
    if (!output)
    {
        cerr << "No se pudo crear el archivo de salida" << endl;
        return;
    }
    output << formato << endl;
    output << "#MAX=" << maximo << endl;
    output << width << " " << height << endl;
    output << resolucion << endl;

    for (int k = 0; k < pixeles.size(); k++)
    {
        // Calcula y escribe los valores de píxeles en el archivo.
        output << static_cast<int>(pixeles[k].r * resolucion / maximo) << " " << static_cast<int>(pixeles[k].g * resolucion / maximo) << " " << static_cast<int>(pixeles[k].b * resolucion / maximo) << "   ";
        if ((k + 1) % width == 0)
        {
            output << endl;
        }
    }
    cout << "🌟 TONE MAPPING COMPLETADO guardando en " + ficheroDestino << " 🌟\n";
}

float pixelMax(vector<Pixel> &pixeles)
{
    float res = 0;
    for (Pixel pixel : pixeles)
    {
        if (res < pixel.r)
        {
            res = pixel.r;
        }
        if (res < pixel.g)
        {
            res = pixel.g;
        }
        if (res < pixel.b)
        {
            res = pixel.b;
        }
    }
    return res;
}

// Esta función escribe los datos de la imagen en un archivo de destino.
void guardarImagenPPMHDR(string ficheroDestino, vector<Pixel> &pixeles, string formato, int width, int height)
{
    int resolucion = pixelMax(pixeles);
    ofstream output(ficheroDestino + ".ppm");
    if (!output)
    {
        cerr << "No se pudo crear el archivo de salida" << endl;
        return;
    }
    output << formato << endl;
    output << "#MAX=" << 65530 << endl;
    output << width << " " << height << endl;
    output << resolucion << endl;

    for (int k = 0; k < pixeles.size(); k++)
    {
        // Calcula y escribe los valores de píxeles en el archivo.
        output << pixeles[k].r << " " << pixeles[k].g << " " << pixeles[k].b << "   ";
        if ((k + 1) % width == 0)
        {
            output << endl;
        }
    }
    cout << "🌟 TONE MAPPING COMPLETADO guardando en " + ficheroDestino << " 🌟\n";
}

#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)

// Estructura que define la cabecera de un archivo BMP.
struct BMPHeader
{
    uint16_t fileType{0x4D42};   // BMP file signature
    uint32_t fileSize{0};        // File size in bytes
    uint16_t reserved1{0};       // Reserved
    uint16_t reserved2{0};       // Reserved
    uint32_t dataOffset{54};     // Offset to pixel data
    uint32_t headerSize{40};     // Header size in bytes
    int32_t width{0};            // Image width
    int32_t height{0};           // Image height (positive for bottom-up)
    uint16_t planes{1};          // Number of color planes
    uint16_t bitsPerPixel{24};   // Bits per pixel (3 bytes)
    uint32_t compression{0};     // Compression method (0 for uncompressed)
    uint32_t imageSize{0};       // Image size in bytes (0 for uncompressed)
    int32_t xPixelsPerMeter{0};  // Horizontal resolution (pixels per meter)
    int32_t yPixelsPerMeter{0};  // Vertical resolution (pixels per meter)
    uint32_t colorsUsed{0};      // Number of colors in the palette (0 for 24-bit)
    uint32_t colorsImportant{0}; // Number of important colors (0 for all)
};

#pragma pack(pop)

// Función para guardar una imagen en formato BMP.
void guardarImagenBMP(const std::string &ficheroDestino, const std::vector<Pixel> &pixeles, int width, int height, double maximo)
{
    // Crea un archivo de salida en formato binario.
    std::ofstream output(ficheroDestino + ".bmp", std::ios::binary);
    if (!output)
    {
        std::cerr << "No se pudo crear el archivo de salida" << std::endl;
    }

    // Calcula el tamaño del archivo BMP.
    int fileSize = sizeof(BMPHeader) + static_cast<int>(width) * static_cast<int>(height) * 3;

    // Crea la cabecera BMP.
    BMPHeader header;
    header.fileSize = fileSize;
    header.width = width;
    header.height = height;

    // Escribe la cabecera BMP en el archivo.
    output.write(reinterpret_cast<char *>(&header), sizeof(header));

    // Escribe los datos de píxeles en el archivo, invirtiendo las filas.
    for (int row = height - 1; row >= 0; --row)
    {
        for (int col = 0; col < width; ++col)
        {
            const Pixel &pixel = pixeles[row * width + col];
            uint8_t r = static_cast<uint8_t>(pixel.r * 255.0 / maximo);
            uint8_t g = static_cast<uint8_t>(pixel.g * 255.0 / maximo);
            uint8_t b = static_cast<uint8_t>(pixel.b * 255.0 / maximo);
            output.put(b);
            output.put(g);
            output.put(r);
        }
    }

    std::cout << "La imagen se ha guardado en formato BMP en " << ficheroDestino << std::endl;
}