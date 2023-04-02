#ifndef DB_H
#define DB_H

#include "sqlite3.h"
#include "struct.h"

int abrir_db(sqlite3** db);
void cerrar_db(sqlite3* db);

Cliente* db_validar_credenciales(char* usuario, char* contrasena);
void db_registrar_cliente(Cliente* nuevo_cliente);
void db_actualizar_cliente(int id_cliente, Cliente* datos_actualizados);
void db_eliminar_cliente(int id_cliente);
Cliente* db_buscar_cliente_por_id(int id_cliente);

#endif // DB_H
