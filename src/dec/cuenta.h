#ifndef CUENTA_H
#define CUENTA_H

#include "../struct.h"

CuentaBancaria* crear_cuenta(Cliente* titular);
void mostrar_informacion_cuenta(int numero_cuenta);
void depositar_dinero(int numero_cuenta, float monto);
void retirar_dinero(int numero_cuenta, float monto);
void transferir_dinero(int cuenta_origen, int cuenta_destino, float monto);
void cerrar_cuenta(int numero_cuenta);
CuentaBancaria* buscar_cuenta_por_numero(int numero_cuenta);


#endif // CUENTA_H
