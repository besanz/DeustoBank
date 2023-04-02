#ifndef DB_H
#define DB_H

#include "../../lib/sqlite3.h"
#include "../struct.h"

int abrir_db(sqlite3** db);
void cerrar_db(sqlite3* db);

Cliente* db_validar_credenciales(char* usuario, char* contrasena);
void db_registrar_cliente(Cliente* nuevo_cliente);
void db_actualizar_cliente(int id_cliente, Cliente* datos_actualizados);
void db_eliminar_cliente(int id_cliente);
Cliente* db_buscar_cliente_por_id(int id_cliente);

CuentaBancaria* db_crear_cuenta(Cliente* titular);
CuentaBancaria* db_buscar_cuenta_por_numero(int numero_cuenta);
void db_depositar_dinero(int numero_cuenta, float monto);
void db_retirar_dinero(int numero_cuenta, float monto);
void db_transferir_dinero(int cuenta_origen, int cuenta_destino, float monto);
void db_cerrar_cuenta(int numero_cuenta);

#endif // DB_H
