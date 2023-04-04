#ifndef STRUCT_H
#define STRUCT_H

// Incluimos las bibliotecas necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Enumeracion para los tipos de usuarios
typedef enum {
    ADMINISTRADOR,
    CLIENTE
} TipoUsuario;

// Estructura de datos para un usuario generico
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
    int usuario_id;
    char nombre[50];
    char apellido[50];
    char dni[20];
    char direccion[100];
    char telefono[20];
} Cliente;

// Estructura de datos para una cuenta bancaria
typedef struct {
    char numeroCuenta[25]; // 24 digitos + el caracter nulo
    float saldo;
    Cliente *cliente;
    char codigoBIC[12]; // 11 caracteres + el caracter nulo
} CuentaBancaria;


// Estructura de datos para una transaccion bancaria
typedef struct {
    int transaccionID;
    char numeroCuentaOrigen[20];
    char numeroCuentaDestino[20];
    float importe;
    time_t fecha;
} Transaccion;

// Estructura de datos para un informe financiero
typedef struct {
    char numeroCuenta[20];
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