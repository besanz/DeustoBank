#ifndef DB_H
#define DB_H

#include "struct.h"

// Funciones de la base de datos para interactuar con los datos de los clientes
Cliente* db_validar_credenciales(char* usuario, char* contrasena);
void db_registrar_cliente(Cliente* nuevo_cliente);
void db_actualizar_cliente(int id_cliente, Cliente* datos_actualizados);
void db_eliminar_cliente(int id_cliente);
Cliente* db_buscar_cliente_por_id(int id_cliente);

#endif // DB_H
