#ifndef DB_H
#define DB_H

#include "../../lib/sqlite3.h"
#include "../struct.h"

/* 
 * Abre una conexion con una base de datos SQLite.
 * @param db: Puntero a un puntero de tipo sqlite3, que contendra la referencia a la base de datos abierta.
 * @return: Entero que representa el resultado de la operacion. 0 si se realizo con exito, otro valor en caso de error.
 */
int abrir_db(sqlite3 **db);

/*
 * Cierra la conexion con una base de datos SQLite.
 * @param db: Puntero a una estructura de tipo sqlite3 que representa la base de datos a cerrar.
 */
void cerrar_db(sqlite3 *db);

/*
 * Inicializa una base de datos SQLite, creando las tablas y configurando los parametros necesarios.
 */
void db_inicializar();

/*
 * Registra un nuevo cliente y usuario en la base de datos.
 * @param nuevo_cliente: Puntero a una estructura de tipo Cliente que contiene los datos del nuevo cliente a registrar.
 * @param nuevo_usuario: Puntero a una estructura de tipo Usuario que contiene los datos del nuevo usuario asociado al cliente.
 */
void db_registrar_cliente(Cliente *nuevo_cliente, Usuario *nuevo_usuario);

/*
 * Actualiza los datos de un cliente y usuario en la base de datos.
 * @param clienteID: Entero que representa el ID del cliente a actualizar.
 * @param datos_actualizados: Puntero a una estructura de tipo Cliente que contiene los nuevos datos del cliente.
 * @param usuario_actualizado: Puntero a una estructura de tipo Usuario que contiene los nuevos datos del usuario asociado al cliente.
 */
void db_actualizar_cliente(int clienteID, Cliente *datos_actualizados, Usuario *usuario_actualizado);

/*
 * Elimina un cliente y su usuario asociado de la base de datos.
 * @param clienteID: Entero que representa el ID del cliente a eliminar.
 */
void db_eliminar_cliente(int clienteID);
Cliente *db_buscar_cliente_por_id(int clienteID);
Cliente *db_buscar_cliente_por_usuarioID(int usuarioID);
int db_obtener_usuarioID(const char *nombreUsuario);
Usuario *db_validar_credenciales(const char *usuario, const char *contrasena);
void db_registrar_usuario(Usuario *nuevo_usuario);
int db_existe_usuario(const char *nombreUsuario);

void db_depositar_dinero(int clienteID, float cantidad);
void db_retirar_dinero(int clienteID, float cantidad);
void db_transferir_dinero(int clienteID_origen, int clienteID_destino, float cantidad);
void db_cerrar_cuenta(int clienteID);

CuentaBancaria *db_crear_cuenta(Cliente *titular);
CuentaBancaria *db_buscar_cuenta_por_numero(const char *numero_cuenta);
CuentaBancaria *db_buscar_cuenta_por_cliente(int clienteID);
CuentaBancaria *db_buscar_cuenta_por_clienteID(int clienteID);

void db_agregar_transaccion(const char* numeroCuentaOrigen, const char* numeroCuentaDestino, float importe, time_t fecha, TipoTransaccion tipo);
void db_agregar_transaccion_por_clienteID(int clienteIDOrigen, int clienteIDDestino, float importe, time_t fecha, TipoTransaccion tipo);
void db_registrar_transaccion(Transaccion* transaccion);
int db_eliminar_transaccion(int id_transaccion);
void eliminar_transaccion(int id_transaccion);
Transaccion *db_listar_transacciones(const char* numero_cuenta, int *num_transacciones);
Informe *db_mostrar_informe_financiero(const char *numero_cuenta);


#endif // DB_H

