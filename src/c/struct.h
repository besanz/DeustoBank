#ifndef STRUCT_H
#define STRUCT_H

// Incluimos las bibliotecas necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enumeración para los tipos de usuarios
typedef enum {
    ADMINISTRADOR,
    CLIENTE
} TipoUsuario;

// Estructura de datos para un usuario genérico
typedef struct {
    int usuarioID;
    char nombreUsuario[50];
    char contrasena[50];
    TipoUsuario tipo;
    void *datos;
} Usuario;

// Estructura de datos para un cliente bancario
typedef struct {
    int clienteID;
    char nombre[50];
    char apellido[50];
    char dni[20];
    char direccion[100];
    char telefono[20];
} Cliente;

// Estructura de datos para una cuenta bancaria
typedef struct {
    int numeroCuenta;
    float saldo;
    Cliente *cliente;
} CuentaBancaria;

// Estructura de datos para una transacción bancaria
typedef struct {
    int transaccionID;
    int numeroCuentaOrigen;
    int numeroCuentaDestino;
    float importe;
    time_t fecha;
} Transaccion;

// Estructura de datos para un informe financiero
typedef struct {
    int numeroCuenta;
    float saldoInicial;
    float saldoFinal;
    int numDepositos;
    float totalDepositos;
    int numRetiros;
    float totalRetiros;
    int numTransferenciasEnviadas;
    float totalTransferenciasEnviadas;
    int numTransferenciasRecibidas;
    float totalTransferenciasRecibidas;
} Informe;

#endif // STRUCT_H