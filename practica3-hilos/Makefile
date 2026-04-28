# Makefile - Práctica 3
# Sistemas Operativos

CC = gcc

# Flags de compilación:
# -Wall:    Muestra todas las advertencias
# -g:       Información de depuración
# -pthread: Habilita macros y soporte para hilos POSIX
CFLAGS = -Wall -g -pthread

LIBS = -lpthread

TARGET = simula_car
SRCS = simula_car.c

# Regla por defecto
all: $(TARGET)

# Generación del ejecutable
# $@ es el objetivo (simula_car), $^ son las dependencias (.c)
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Regla de limpieza (phony evita conflictos con archivos llamados 'clean')
.PHONY: clean
clean:
	rm -f $(TARGET)
