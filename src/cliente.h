#ifndef CLIENTE_H
#define CLIENTE_H

// Incluimos las bibliotecas necesarias
#include <stdio.h>
#include <string.h>
#include "struct.h"

// Función para crear un nuevo cliente
Cliente crearCliente(char nombre[50], int edad, char direccion[100], char telefono[20]);

// Función para imprimir los datos de un cliente
void imprimirCliente(Cliente cliente);

#endif // CLIENTE_H