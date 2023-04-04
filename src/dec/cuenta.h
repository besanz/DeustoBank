#ifndef CUENTA_H
#define CUENTA_H

#include "../struct.h"

void crear_cuenta(int clienteID);
void generar_numero_cuenta(char *numero_cuenta);
int cuenta_existe(const char *numero_cuenta);

void depositar_dinero(const char* numero_cuenta, float monto);
void retirar_dinero(const char* numero_cuenta, float monto);
void transferir_dinero(const char* cuenta_origen, const char* cuenta_destino, float monto);
void cerrar_cuenta(const char* numero_cuenta);
void mostrar_informacion_cuenta(const char *numero_cuenta);
CuentaBancaria* buscar_cuenta_por_numero(const char *numero_cuenta);

#endif // CUENTA_H
