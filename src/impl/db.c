#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "../dec/db.h"
// Iniciar y cerrar conexion con base de datos
int abrir_db(sqlite3 **db)
{
    int rc = sqlite3_open("deustobank.db", db);
    if (rc)
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    return 0;
}

void cerrar_db(sqlite3 *db)
{
    sqlite3_close(db);
}

void db_inicializar() // Necesita Cambio
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Abrir la base de datos
    rc = sqlite3_open("deustobank.db", &db);
    if (rc)
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Crear la tabla de usuarios si no existe
    const char *sql_usuarios = "CREATE TABLE IF NOT EXISTS usuarios ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "nombreUsuario TEXT UNIQUE NOT NULL,"
                               "contrasena TEXT NOT NULL,"
                               "tipo TEXT NOT NULL);";

    rc = sqlite3_exec(db, sql_usuarios, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al crear la tabla de usuarios: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Crear la tabla de clientes si no existe
    const char *sql_clientes = "CREATE TABLE IF NOT EXISTS clientes ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "usuario_id INTEGER,"
                               "nombre TEXT NOT NULL,"
                               "apellido TEXT NOT NULL,"
                               "dni TEXT NOT NULL UNIQUE,"
                               "direccion TEXT,"
                               "telefono TEXT,"
                               "FOREIGN KEY (usuario_id) REFERENCES usuarios (id));";

    rc = sqlite3_exec(db, sql_clientes, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al crear la tabla de clientes: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Crear la tabla de administradores si no existe
    const char *sql_administradores = "CREATE TABLE IF NOT EXISTS administradores ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "usuario_id INTEGER,"
                                      "nombre TEXT NOT NULL,"
                                      "apellido TEXT NOT NULL,"
                                      "dni TEXT NOT NULL UNIQUE,"
                                      "FOREIGN KEY (usuario_id) REFERENCES usuarios (id));";

    rc = sqlite3_exec(db, sql_administradores, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al crear la tabla de administradores: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Crear la tabla de cuentas bancarias si no existe
    const char *sql_cuentas = "CREATE TABLE IF NOT EXISTS cuentas ("
                              "numeroCuenta INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "saldo REAL NOT NULL,"
                              "clienteID INTEGER NOT NULL,"
                              "FOREIGN KEY (clienteID) REFERENCES clientes (id));";

    rc = sqlite3_exec(db, sql_cuentas, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al crear la tabla de cuentas bancarias: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Crear la tabla de transacciones bancarias si no existe
    const char *sql_transacciones = "CREATE TABLE IF NOT EXISTS transacciones ("
                                    "transaccionID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                    "numeroCuentaOrigen INTEGER NOT NULL,"
                                    "numeroCuentaDestino INTEGER NOT NULL,"
                                    "importe REAL NOT NULL,"
                                    "fecha INTEGER NOT NULL,"
                                    "FOREIGN KEY (numeroCuentaOrigen) REFERENCES cuentas (numeroCuenta),"
                                    "FOREIGN KEY (numeroCuentaDestino) REFERENCES cuentas (numeroCuenta));";

    rc = sqlite3_exec(db, sql_transacciones, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al crear la tabla de transacciones bancarias: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Cerrar la base de datos
    sqlite3_close(db);
}

// Metodos cliente
void db_registrar_cliente(Cliente *nuevo_cliente, Usuario *nuevo_usuario)
{

    nuevo_cliente->usuario_id = db_obtener_usuario_id(nuevo_usuario->nombreUsuario);
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if (abrir_db(&db))
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *sql = "INSERT INTO clientes (usuario_id, nombre, apellido, dni, direccion, telefono) VALUES (?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, nuevo_cliente->usuario_id);
        sqlite3_bind_text(stmt, 2, nuevo_cliente->nombre, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, nuevo_cliente->apellido, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, nuevo_cliente->dni, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, nuevo_cliente->direccion, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, nuevo_cliente->telefono, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
            fprintf(stderr, "Error al registrar cliente: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            nuevo_cliente->clienteID = sqlite3_last_insert_rowid(db);
        }
    }
    else
    {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    cerrar_db(db);
}

void db_actualizar_cliente(int id_cliente, Cliente *datos_actualizados, Usuario *usuario_actualizado)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql[500];

    rc = abrir_db(&db);

    if (rc)
    {
        printf("No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    snprintf(sql, sizeof(sql), "UPDATE clientes SET nombre = '%s', apellido = '%s', dni = '%s', direccion = '%s', telefono = '%s' WHERE id = %d;",
             datos_actualizados->nombre, datos_actualizados->apellido, datos_actualizados->dni, datos_actualizados->direccion, datos_actualizados->telefono, id_cliente);

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al actualizar el cliente: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        snprintf(sql, sizeof(sql), "UPDATE usuarios SET nombreUsuario = '%s', contrasena = '%s' WHERE usuarioID = %d;",
                 usuario_actualizado->nombreUsuario, usuario_actualizado->contrasena, datos_actualizados->usuario_id);
        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Error al actualizar el usuario: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }

    cerrar_db(db);
}

void db_eliminar_cliente(int id_cliente)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql[500];

    rc = abrir_db(&db);

    if (rc)
    {
        printf("No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    snprintf(sql, sizeof(sql), "DELETE FROM clientes WHERE id = %d;", id_cliente);

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al eliminar el cliente: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        snprintf(sql, sizeof(sql), "DELETE FROM usuarios WHERE usuarioID = (SELECT usuario_id FROM clientes WHERE id = %d);", id_cliente);

        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Error al eliminar el usuario: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }

    cerrar_db(db);
}

Cliente *db_buscar_cliente_por_id(int id_cliente)
{
    sqlite3 *db;
    if (abrir_db(&db) != 0)
    {
        return NULL;
    }

    const char *sql = "SELECT * FROM clientes WHERE clienteID=?";
    sqlite3_stmt *stmt;
    Cliente *cliente = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id_cliente);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            cliente = (Cliente *)malloc(sizeof(Cliente));
            cliente->clienteID = sqlite3_column_int(stmt, 0);
            strncpy(cliente->nombre, (const char *)sqlite3_column_text(stmt, 1), sizeof(cliente->nombre));
            strncpy(cliente->apellido, (const char *)sqlite3_column_text(stmt, 2), sizeof(cliente->apellido));
            strncpy(cliente->dni, (const char *)sqlite3_column_text(stmt, 3), sizeof(cliente->dni));
            strncpy(cliente->direccion, (const char *)sqlite3_column_text(stmt, 4), sizeof(cliente->direccion));
            strncpy(cliente->telefono, (const char *)sqlite3_column_text(stmt, 5), sizeof(cliente->telefono));
        }
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
    return cliente;
}

Cliente *db_buscar_cliente_por_usuario_id(int usuario_id)

{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char sql[500];
    Cliente *cliente = NULL;

    rc = abrir_db(&db);

    if (rc)
    {
        printf("No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    snprintf(sql, sizeof(sql), "SELECT * FROM clientes WHERE usuario_id = %d;", usuario_id);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        printf("No se puede preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        cliente = malloc(sizeof(Cliente));
        cliente->clienteID = sqlite3_column_int(stmt, 0);
        strcpy(cliente->nombre, (char *)sqlite3_column_text(stmt, 1));
        strcpy(cliente->apellido, (char *)sqlite3_column_text(stmt, 2));
        strcpy(cliente->dni, (char *)sqlite3_column_text(stmt, 3));
        strcpy(cliente->direccion, (char *)sqlite3_column_text(stmt, 4));
        strcpy(cliente->telefono, (char *)sqlite3_column_text(stmt, 5));
        cliente->usuario_id = sqlite3_column_int(stmt, 6);
    }
    else
    {
        printf("No se encontró ningún cliente con el usuario_id %d\n", usuario_id);
    }
    sqlite3_finalize(stmt);
    cerrar_db(db);

    return cliente;
}

int db_obtener_usuario_id(const char *nombreUsuario)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *sql;
    int usuario_id = -1;

    // Abrir la base de datos
    rc = sqlite3_open("deustobank.db", &db);
    if (rc)
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Preparar la consulta SQL
    sql = "SELECT id FROM usuarios WHERE nombreUsuario = ?;";

    // Preparar el statement
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al preparar la declaración SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Vincular el valor del nombre de usuario al statement
    sqlite3_bind_text(stmt, 1, nombreUsuario, -1, SQLITE_STATIC);

    // Ejecutar la consulta y extraer el ID del usuario
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        usuario_id = sqlite3_column_int(stmt, 0);
    }
    else if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

    // Finalizar el statement y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return usuario_id;
}

Usuario *db_validar_credenciales(const char *usuario, const char *contrasena)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Usuario *usuario_validado = NULL;

    if (abrir_db(&db))
    {
        return NULL;
    }

    const char *sql = "SELECT * FROM usuarios WHERE nombreUsuario = ? AND contrasena = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, usuario, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            usuario_validado = (Usuario *)malloc(sizeof(Usuario));
            usuario_validado->usuarioID = sqlite3_column_int(stmt, 0);
            strncpy(usuario_validado->nombreUsuario, (const char *)sqlite3_column_text(stmt, 1), sizeof(usuario_validado->nombreUsuario) - 1);
            strncpy(usuario_validado->contrasena, (const char *)sqlite3_column_text(stmt, 2), sizeof(usuario_validado->contrasena) - 1);
            usuario_validado->tipo = sqlite3_column_int(stmt, 3);
            if (usuario_validado->tipo == CLIENTE)
            {
                Cliente *cliente = db_buscar_cliente_por_usuario_id(usuario_validado->usuarioID);
                if (cliente)
                {
                    usuario_validado->datos = (void *)cliente;
                }
                else
                {
                    free(usuario_validado);
                    usuario_validado = NULL;
                }
            }
            else
            {
                // Si se requiere admitir otros tipos de usuarios, agregar aquí el código correspondiente.
            }
        }

        sqlite3_finalize(stmt);
    }

    cerrar_db(db);

    return usuario_validado;
}

void db_registrar_usuario(Usuario *nuevo_usuario)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if (abrir_db(&db))
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    nuevo_usuario->tipo = CLIENTE;
    const char *sql = "INSERT INTO usuarios (nombreUsuario, contrasena, tipo) VALUES (?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, nuevo_usuario->nombreUsuario, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, nuevo_usuario->contrasena, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, nuevo_usuario->tipo);

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE)
        {
            fprintf(stderr, "Error al insertar el nuevo usuario: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
    }
    cerrar_db(db);
}

int db_existe_usuario(const char *nombreUsuario)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int existe = 0;

    if (abrir_db(&db))
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    const char *sql = "SELECT COUNT(*) FROM usuarios WHERE nombreUsuario = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, nombreUsuario, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            existe = sqlite3_column_int(stmt, 0);
        }
        else
        {
            fprintf(stderr, "Error al consultar la existencia del usuario: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
    }

    cerrar_db(db);
    return existe;
}
// Metodos cuenta
void db_depositar_dinero(int numero_cuenta, float monto)
{
    sqlite3 *db;
    if (abrir_db(&db) != SQLITE_OK)
    {
        return;
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);
    const char *sql = "UPDATE cuentas SET saldo = saldo + ? WHERE numeroCuenta = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_double(stmt, 1, monto);
        sqlite3_bind_int(stmt, 2, numero_cuenta);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", 0, 0, 0);
    cerrar_db(db);
}

void db_retirar_dinero(int numero_cuenta, float monto)
{
    sqlite3 *db;
    if (abrir_db(&db) != SQLITE_OK)
    {
        return;
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);
    const char *sql = "UPDATE cuentas SET saldo = saldo - ? WHERE numeroCuenta = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_double(stmt, 1, monto);
        sqlite3_bind_int(stmt, 2, numero_cuenta);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", 0, 0, 0);
    cerrar_db(db);
}

void db_transferir_dinero(int cuenta_origen, int cuenta_destino, float monto)
{
    db_retirar_dinero(cuenta_origen, monto);
    db_depositar_dinero(cuenta_destino, monto);
}

void db_cerrar_cuenta(int numero_cuenta)
{
    sqlite3 *db;
    if (abrir_db(&db) != SQLITE_OK)
    {
        return;
    }
    const char *sql = "DELETE FROM cuentas WHERE numeroCuenta = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, numero_cuenta);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    cerrar_db(db);
}

CuentaBancaria *db_crear_cuenta(Cliente *titular)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char *sql;

    abrir_db(&db);

    sql = "INSERT INTO cuentas (numeroCuenta, saldo, clienteID, codigoBIC) VALUES (?, 0, ?, 'DBANKSPAINSS');";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    char numeroCuenta[25];
    snprintf(numeroCuenta, sizeof(numeroCuenta), "DEUSTOBANK%016d", titular->clienteID);
    sqlite3_bind_text(stmt, 1, numeroCuenta, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, titular->clienteID);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Error al insertar la nueva cuenta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        cerrar_db(db);
        return NULL;
    }
    sqlite3_finalize(stmt);
    cerrar_db(db);

    CuentaBancaria *nueva_cuenta = malloc(sizeof(CuentaBancaria));
    strcpy(nueva_cuenta->numeroCuenta, numeroCuenta);
    nueva_cuenta->saldo = 0;
    nueva_cuenta->cliente = titular;
    strcpy(nueva_cuenta->codigoBIC, "DBANKSPAINSS");

    return nueva_cuenta;
}

void db_registrar_transaccion(Transaccion *transaccion)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    abrir_db(&db);

    char fecha_str[20];
    strftime(fecha_str, sizeof(fecha_str), "%Y-%m-%d %H:%M:%S", localtime(&transaccion->fecha));

    char *sql = "INSERT INTO transacciones (numeroCuentaOrigen, numeroCuentaDestino, importe, fecha) VALUES (?, ?, ?, ?)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, transaccion->numeroCuentaOrigen);
        sqlite3_bind_int(stmt, 2, transaccion->numeroCuentaDestino);
        sqlite3_bind_double(stmt, 3, transaccion->importe);
        sqlite3_bind_text(stmt, 4, fecha_str, -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Error al registrar la transaccion: %s\n", sqlite3_errmsg(db));
        }
    }
    else
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
}

CuentaBancaria* db_buscar_cuenta_por_numero(int numero_cuenta)
{
    
}


CuentaBancaria *db_buscar_cuenta_por_cliente(int clienteID) {
    // Implementación de la función aquí
}

