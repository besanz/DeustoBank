#include "cuenta.h"

// Función para crear una nueva cuenta bancaria
CuentaBancaria crearCuentaBancaria(int numeroCuenta, Cliente cliente) {
CuentaBancaria cuenta;
cuenta.numeroCuenta = numeroCuenta;
cuenta.cliente = cliente;
cuenta.saldo = 0;
return cuenta;
}

// Función para imprimir los datos de una cuenta bancaria
void imprimirCuentaBancaria(CuentaBancaria cuenta) {
printf("Número de cuenta: %d\n", cuenta.numeroCuenta);
imprimirCliente(cuenta.cliente);
printf("Saldo: %.2f\n", cuenta.saldo);
}

// Función para realizar un depósito en una cuenta bancaria
void depositar(CuentaBancaria *cuenta, float importe) {
cuenta->saldo += importe;
printf("Depósito realizado con éxito\n");
}

// Función para realizar un retiro de una cuenta bancaria
void retirar(CuentaBancaria *cuenta, float importe) {
if (cuenta->saldo < importe) {
printf("No se puede realizar el retiro: saldo insuficiente\n");
return;
}

cuenta->saldo -= importe;
printf("Retiro realizado con éxito\n");
}