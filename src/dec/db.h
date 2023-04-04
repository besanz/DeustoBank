#ifndef DB_H
#define DB_H

#include "../../lib/sqlite3.h"
#include "../struct.h"

int abrir_db(sqlite3 **db);
void cerrar_db(sqlite3 *db);
void db_inicializar();

void db_registrar_cliente(Cliente *nuevo_cliente, Usuario *nuevo_usuario);
void db_actualizar_cliente(int clienteID, Cliente *datos_actualizados, Usuario *usuario_actualizado);
void db_eliminar_cliente(int clienteID);
Cliente *db_buscar_cliente_por_id(int clienteID);
Cliente *db_buscar_cliente_por_usuarioID(int usuarioID);
int db_obtener_usuarioID(const char *nombreUsuario);
Usuario *db_validar_credenciales(const char *usuario, const char *contrasena);
void db_registrar_usuario(Usuario *nuevo_usuario);
int db_existe_usuario(const char *nombreUsuario);

void db_depositar_dinero(const char* numero_cuenta, float cantidad);
void db_retirar_dinero(const char* numero_cuenta, float cantidad);
void db_transferir_dinero(const char* cuenta_origen, const char* cuenta_destino, float cantidad);
void db_cerrar_cuenta(const char* numero_cuenta);

CuentaBancaria *db_crear_cuenta(Cliente *titular);
CuentaBancaria *db_buscar_cuenta_por_numero(const char *numero_cuenta);
CuentaBancaria *db_buscar_cuenta_por_cliente(int clienteID);

void db_agregar_transaccion(const char* numeroCuentaOrigen, const char* numeroCuentaDestino, float importe, time_t fecha, TipoTransaccion tipo);
void db_registrar_transaccion(Transaccion* transaccion);
int db_eliminar_transaccion(int id_transaccion);
void eliminar_transaccion(int id_transaccion);
Transaccion *db_listar_transacciones(const char* numero_cuenta, int *num_transacciones);
Informe *db_mostrar_informe_financiero(const char *numero_cuenta);
//int get_numero_de_clientes(char sql[]);

#endif // DB_H

