// Archivo: transaccion.c

#include "transaccion.h"

// Función para realizar una transacción bancaria
void realizarTransaccion(CuentaBancaria *cuentaOrigen, CuentaBancaria *cuentaDestino, float importe) {
if (cuentaOrigen->saldo < importe) {
printf("No se puede realizar la transacción: saldo insuficiente\n");
return;
}

cuentaOrigen->saldo -= importe;
cuentaDestino->saldo += importe;

printf("Transacción realizada con éxito\n");
}