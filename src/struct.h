#ifndef STRUCT_H
#define STRUCT_H

// Incluimos las bibliotecas necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura de datos para una cuenta bancaria
typedef struct {
int numeroCuenta;
char nombreTitular[50];
float saldo;
Cliente *cliente;
} CuentaBancaria;

// Estructura de datos para una transacción bancaria
typedef struct {
int numeroCuentaOrigen;
int numeroCuentaDestino;
float importe;
} Transaccion;

// Estructura de datos para un cliente bancario
typedef struct {
char nombre[50];
int edad;
char direccion[100];
char telefono[20];
} Cliente;

#endif // STRUCT_H