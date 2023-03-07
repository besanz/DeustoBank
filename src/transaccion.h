#ifndef TRANSACCION_H
#define TRANSACCION_H

// Incluimos las bibliotecas necesarias
#include <stdio.h>
#include "struct.h"

// Función para realizar una transacción bancaria
void realizarTransaccion(CuentaBancaria *cuentaOrigen, CuentaBancaria *cuentaDestino, float importe);

#endif // TRANSACCION_H