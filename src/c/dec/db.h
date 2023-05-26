#ifndef DB_H
#define DB_H

#include "../../../lib/sqlite3_wrapper.h"
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

/*
 * Busca un cliente en la base de datos por su ID.
 * @param clienteID: Entero que representa el ID del cliente a buscar.
 * @return: Puntero a una estructura de tipo Cliente si se encuentra el cliente, NULL en caso contrario.
*/
Cliente *db_buscar_cliente_por_id(int clienteID);

/*
 * Busca un cliente en la base de datos por el ID de su usuario asociado.
 * @param usuarioID: Entero que representa el ID del usuario asociado al cliente a buscar.
 * @return: Puntero a una estructura de tipo Cliente si se encuentra el cliente, NULL en caso contrario.
*/
Cliente *db_buscar_cliente_por_usuarioID(int usuarioID);

/*
 * Obtiene el ID de un usuario en la base de datos a partir de su nombre de usuario.
 * @param nombreUsuario: Cadena de caracteres que representa el nombre de usuario a buscar.
 * @return: Entero que representa el ID del usuario si se encuentra, -1 en caso contrario.
*/
int db_obtener_usuarioID(const char *nombreUsuario);

/*
 * Valida las credenciales de un usuario en la base de datos.
 * @param usuario: Cadena de caracteres que representa el nombre de usuario a validar.
 * @param contrasena: Cadena de caracteres que representa la contrasenya a validar.
 * @return: Puntero a una estructura de tipo Usuario si las credenciales son correctas, NULL en caso contrario.
*/
Usuario *db_validar_credenciales(const char *usuario, const char *contrasena);

/*
 * Registra un nuevo usuario en la base de datos.
 * @param nuevo_usuario: Puntero a una estructura de tipo Usuario que contiene los datos del nuevo usuario a registrar.
*/
void db_registrar_usuario(Usuario *nuevo_usuario);

/*
 * Verifica si un usuario existe en la base de datos a partir de su nombre de usuario.
 * @param nombreUsuario: Cadena de caracteres que representa el nombre de usuario a verificar.
 * @return: 1 si el usuario existe, 0 en caso contrario.
*/
int db_existe_usuario(const char *nombreUsuario);

/*
 * Deposita una cantidad de dinero en la cuenta de un cliente en la base de datos.
 * @param clienteID: Entero que representa el ID del cliente en cuya cuenta se desea depositar el dinero.
 * @param cantidad: Cantidad de dinero a depositar en la cuenta del cliente.
*/
void db_depositar_dinero(int clienteID, float cantidad);

/*
 * Retira una cantidad de dinero de la cuenta de un cliente en la base de datos.
 * @param clienteID: Entero que representa el ID del cliente cuya cuenta se desea retirar el dinero.
 * @param cantidad: Cantidad de dinero a retirar de la cuenta del cliente.
*/
void db_retirar_dinero(int clienteID, float cantidad);

/*
 * Transfiere una cantidad de dinero de la cuenta de un cliente origen a la cuenta de un cliente destino en la base de datos.
 * @param clienteID_origen: Entero que representa el ID del cliente origen de la transferencia.
 * @param clienteID_destino: Entero que representa el ID del cliente destino de la transferencia.
 * @param cantidad: Cantidad de dinero a transferir de la cuenta del cliente origen a la cuenta del cliente destino.
*/
void db_transferir_dinero(int clienteID_origen, int clienteID_destino, float cantidad);

/*
 * Cierra la cuenta de un cliente en la base de datos.
 * @param clienteID: Entero que representa el ID del cliente cuya cuenta se desea cerrar.
*/
void db_cerrar_cuenta(int clienteID);

/*
 * Crea una nueva cuenta bancaria para un cliente en la base de datos.
 * @param titular: Puntero a una estructura de tipo Cliente que representa el titular de la cuenta bancaria.
 * @return Puntero a una estructura de tipo CuentaBancaria que representa la nueva cuenta bancaria creada.
*/
CuentaBancaria *db_crear_cuenta(Cliente *titular);

/*
 * Busca una cuenta bancaria en la base de datos por su numero de cuenta.
 * @param numero_cuenta: Cadena de caracteres que representa el numero de cuenta de la cuenta bancaria a buscar.
 * @return Puntero a una estructura de tipo CuentaBancaria que representa la cuenta bancaria encontrada, o NULL si no se encuentra.
*/
CuentaBancaria *db_buscar_cuenta_por_numero(const char *numero_cuenta);

/*
 * Busca una cuenta bancaria en la base de datos por el ID de cliente asociado.
 * @param clienteID: Entero que representa el ID del cliente asociado a la cuenta bancaria a buscar.
 * @return Puntero a una estructura de tipo CuentaBancaria que representa la cuenta bancaria encontrada, o NULL si no se encuentra.
*/
CuentaBancaria *db_buscar_cuenta_por_cliente(int clienteID);

/*
 * Busca una cuenta bancaria en la base de datos por el ID de cliente asociado (mismo que db_buscar_cuenta_por_cliente).
 * @param clienteID: Entero que representa el ID del cliente asociado a la cuenta bancaria a buscar.
 * @return Puntero a una estructura de tipo CuentaBancaria que representa la cuenta bancaria encontrada, o NULL si no se encuentra.
*/
CuentaBancaria *db_buscar_cuenta_por_clienteID(int clienteID);

/*
 * Agrega una nueva transaccion a la base de datos.
 * @param numeroCuentaOrigen: Cadena de caracteres que representa el numero de cuenta origen de la transaccion.
 * @param numeroCuentaDestino: Cadena de caracteres que representa el numero de cuenta destino de la transaccion.
 * @param importe: Cantidad de dinero involucrada en la transaccion.
 * @param fecha: Marca de tiempo que representa la fecha y hora en que se realizo la transaccion.
 * @param tipo: Enumeracion que representa el tipo de transaccion realizada (deposito, retiro, transferencia).
*/
void db_agregar_transaccion(const char* numeroCuentaOrigen, const char* numeroCuentaDestino, float importe, time_t fecha, TipoTransaccion tipo);

/*
 * Agrega una nueva transaccion a la base de datos por los IDs de los clientes asociados a las cuentas de origen y destino.
 * @param clienteIDOrigen: Entero que representa el ID del cliente asociado a la cuenta de origen de la transaccion.
 * @param clienteIDDestino: Entero que representa el ID del cliente asociado a la cuenta de destino de la transaccion.
 * @param importe: Cantidad de dinero involucrada en la transaccion.
 * @param fecha: Marca de tiempo que representa la fecha y hora en que se realizo la transaccion.
 * @param tipo: Enumeracion que representa el tipo de transaccion realizada (deposito, retiro, transferencia).
*/
void db_agregar_transaccion_por_clienteID(int clienteIDOrigen, int clienteIDDestino, float importe, time_t fecha, TipoTransaccion tipo);

/*
 * Registra una transaccion en la base de datos.
 * @param transaccion: Puntero a una estructura de tipo Transaccion que representa la transaccion a registrar.
*/
void db_registrar_transaccion(Transaccion* transaccion);

/*
 * Elimina una transaccion de la base de datos.
 * @param id_transaccion: Entero que representa el ID de la transaccion a eliminar.
 * @return Entero que indica si la operacion fue exitosa (1) o no (0).
*/
int db_eliminar_transaccion(int id_transaccion);

/*
 * Lista todas las transacciones asociadas a una cuenta en particular.
 * @param numero_cuenta: Cadena de caracteres que representa el numero de cuenta de la cual se desean listar las transacciones.
 * @param num_transacciones: Puntero a un entero que representa la cantidad de transacciones encontradas.
 * @return Puntero a un array de estructuras de tipo Transaccion que contiene todas las transacciones asociadas a la cuenta especificada.
*/
Transaccion *db_listar_transacciones(const char* numero_cuenta, int *num_transacciones);

/*
 * Genera un informe financiero para un cliente en particular.
 * @param clienteID: Entero que representa el ID del cliente del cual se generara el informe financiero.
 * @return: Puntero a una estructura de tipo Informe que contiene la informacion del informe financiero generado.
*/
Informe *generar_informe_financiero(int clienteID);
/*
 * Guarda un informe financiero en un archivo de texto.
 * @param informe: Puntero a una estructura de tipo Informe que contiene la informacion del informe financiero a guardar.
 * @param nombre_cliente: Cadena de caracteres que representa el nombre del cliente al que pertenece el informe financiero.
 * @param year: Entero que representa el anyo del informe financiero.
*/
void guardar_informe_en_txt(Informe *informe, const char *nombre_cliente, int year);




#endif // DB_H

