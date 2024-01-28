# Makefile para compilar y ejecutar el programa en Windows

# Nombre del ejecutable
TARGET = main

# Compilador
CC = g++

# Opciones de compilación
CFLAGS = -std=c++11

# Archivos fuente
SRCS = main.cpp Geometrical_objects.cpp

# Archivos objeto generados
OBJS = $(SRCS:.cpp=.o)

# Regla para compilar los archivos fuente en archivos objeto
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar el programa
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Regla para limpiar archivos objeto y el ejecutable
clean:
	rm -f $(OBJS) $(TARGET)

# Regla predeterminada para compilar y ejecutar el programa
all: $(TARGET)
	./$(TARGET)
