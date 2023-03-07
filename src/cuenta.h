#ifndef CUENTA_H
#define CUENTA_H

// Incluimos las bibliotecas necesarias
#include <stdio.h>
#include <string.h>
#include "cliente.h"
#include "struct.h"

// Función para crear una nueva cuenta bancaria
CuentaBancaria crearCuentaBancaria(int numeroCuenta, Cliente cliente);

// Función para imprimir los datos de una cuenta bancaria
void imprimirCuentaBancaria(CuentaBancaria cuenta);

// Función para realizar un depósito en una cuenta bancaria
void depositar(CuentaBancaria *cuenta, float importe);

// Función para realizar un retiro de una cuenta bancaria
void retirar(CuentaBancaria *cuenta, float importe);

#endif // CUENTA_H