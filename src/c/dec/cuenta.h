#ifndef CUENTA_H
#define CUENTA_H

#include "../../struct.h"

/*
* Crea una nueva cuenta bancaria para un cliente.
* @param clienteID: El ID del cliente al cual se le creara la cuenta.
*/
void crear_cuenta(int clienteID);

/*
* Genera un numero de cuenta bancaria valido.
* @param numero_cuenta: Puntero al buffer donde se almacenara el numero de cuenta generado.
*/
void generar_numero_cuenta(char *numero_cuenta);

/*
* Verifica si una cuenta bancaria existe en el sistema.
* @param numero_cuenta: El numero de cuenta a verificar.
* @return: Retorna 1 si la cuenta existe, 0 si no existe.
*/
int cuenta_existe(const char *numero_cuenta);

/*
* Muestra la informacion de una cuenta bancaria.
* @param clienteID: El numero de cuenta de la cuenta a mostrar informacion.
*/
void mostrar_informacion_cuenta(int clienteID);


void depositar_dinero(int clienteID, float cantidad);


void retirar_dinero(int clienteID, float cantidad);


void transferir_dinero(int clienteID_origen, int clienteID_destino, float cantidad);

/*
* Cierra una cuenta bancaria, eliminandola del sistema.
* @param numero_cuenta: Numero de cuenta a cerrar.
*/
void cerrar_cuenta(int clienteID);

/*
 * Busca una cuenta bancaria por su numero de cuenta.
 * @param numero_cuenta: Numero de cuenta a buscar.
 * @return: Puntero a la cuenta bancaria encontrada, o NULL si no se encuentra.
*/
CuentaBancaria* buscar_cuenta_por_numero(const char *numero_cuenta);

/*
 * Obtiene la cuenta bancaria de un cliente por su ID de cliente.
 * @param clienteID: ID del cliente cuya cuenta se desea obtener.
 * @return: Puntero a la cuenta bancaria del cliente.
*/
CuentaBancaria *db_buscar_cuenta_por_clienteID(int clienteID);

#endif // CUENTA_H
