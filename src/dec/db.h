#ifndef DB_H
#define DB_H

#include "../../lib/sqlite3.h"
#include "../struct.h"

int abrir_db(sqlite3** db);
void cerrar_db(sqlite3* db);
void db_inicializar();

Usuario *db_validar_credenciales(const char *usuario, const char *contrasena); //Actualizado
void db_registrar_cliente(Cliente *nuevo_cliente, Usuario *usuario);
void db_actualizar_cliente(int id_cliente, Cliente* datos_actualizados); //NEcesita camnbio
void db_eliminar_cliente(int id_cliente); //Necesita cambio
Cliente* db_buscar_cliente_por_id(int id_cliente);

void db_registrar_usuario(Usuario *nuevo_usuario); //Sin hacer
int db_existe_usuario(const char *nombreUsuario); //Sin hacer

CuentaBancaria* db_crear_cuenta(Cliente* titular);
CuentaBancaria* db_buscar_cuenta_por_numero(int numero_cuenta);
void db_depositar_dinero(int numero_cuenta, float monto);
void db_retirar_dinero(int numero_cuenta, float monto);
void db_transferir_dinero(int cuenta_origen, int cuenta_destino, float monto);
void db_cerrar_cuenta(int numero_cuenta);

#endif // DB_H
