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

void db_inicializar()
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
                               "usuarioID INTEGER,"
                               "nombre TEXT NOT NULL,"
                               "apellido TEXT NOT NULL,"
                               "dni TEXT NOT NULL UNIQUE,"
                               "direccion TEXT,"
                               "telefono TEXT,"
                               "FOREIGN KEY (usuarioID) REFERENCES usuarios (id));";

    rc = sqlite3_exec(db, sql_clientes, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al crear la tabla de clientes: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Crear la tabla de cuentas si no existe
    const char *sql_cuentas = "CREATE TABLE IF NOT EXISTS cuentas ("
                            "numeroCuenta TEXT PRIMARY KEY,"
                            "saldo REAL NOT NULL,"
                            "clienteID INTEGER NOT NULL,"
                            "codigoBIC TEXT NOT NULL,"
                            "FOREIGN KEY (clienteID) REFERENCES clientes (id));";


    rc = sqlite3_exec(db, sql_cuentas, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al crear la tabla de cuentas: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Crear la tabla de transacciones bancarias si no existe
    const char *sql_transacciones = "CREATE TABLE IF NOT EXISTS transacciones ("
                                    "transaccionID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                    "numeroCuentaOrigen TEXT NOT NULL,"
                                    "numeroCuentaDestino TEXT NOT NULL,"
                                    "importe REAL NOT NULL,"
                                    "fecha INTEGER NOT NULL,"
                                    "tipo INTEGER NOT NULL," // Agrega esta linea
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

void db_registrar_cliente(Cliente *nuevo_cliente, Usuario *nuevo_usuario)
{
    nuevo_cliente->usuarioID = db_obtener_usuarioID(nuevo_usuario->nombreUsuario);
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if (abrir_db(&db))
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *sql = "INSERT INTO clientes (usuarioID, nombre, apellido, dni, direccion, telefono) VALUES (?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, nuevo_cliente->usuarioID);
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

void db_actualizar_cliente(int clienteID, Cliente *datos_actualizados, Usuario *usuario_actualizado)
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
             datos_actualizados->nombre, datos_actualizados->apellido, datos_actualizados->dni, datos_actualizados->direccion, datos_actualizados->telefono, clienteID);

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al actualizar el cliente: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        snprintf(sql, sizeof(sql), "UPDATE usuarios SET nombreUsuario = '%s', contrasena = '%s' WHERE usuarioID = %d;",
                 usuario_actualizado->nombreUsuario, usuario_actualizado->contrasena, datos_actualizados->usuarioID);
        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Error al actualizar el usuario: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }

    cerrar_db(db);
}

void db_eliminar_cliente(int clienteID)
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

    snprintf(sql, sizeof(sql), "DELETE FROM clientes WHERE id = %d;", clienteID);

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al eliminar el cliente: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        snprintf(sql, sizeof(sql), "DELETE FROM usuarios WHERE usuarioID = (SELECT usuarioID FROM clientes WHERE id = %d);", clienteID);

        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Error al eliminar el usuario: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }

    cerrar_db(db);
}

Cliente *db_buscar_cliente_por_id(int clienteID)
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
        sqlite3_bind_int(stmt, 1, clienteID);

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

Cliente *db_buscar_cliente_por_usuarioID(int usuarioID)
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

    snprintf(sql, sizeof(sql), "SELECT * FROM clientes WHERE usuarioID = %d;", usuarioID);

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
        strcpy(cliente->nombre, (char *)sqlite3_column_text(stmt, 2));
        strcpy(cliente->apellido, (char *)sqlite3_column_text(stmt, 3));
        strcpy(cliente->dni, (char *)sqlite3_column_text(stmt, 4));
        strcpy(cliente->direccion, (char *)sqlite3_column_text(stmt, 5));
        strcpy(cliente->telefono, (char *)sqlite3_column_text(stmt, 6));
        cliente->usuarioID = sqlite3_column_int(stmt, 1);
    }
    else
    {
        printf("No se encontro ningun cliente con el usuarioID %d\n", usuarioID);
    }
    sqlite3_finalize(stmt);
    cerrar_db(db);

    return cliente;
}

int db_obtener_usuarioID(const char *nombreUsuario)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *sql;
    int usuarioID = -1;

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
        fprintf(stderr, "Error al preparar la declaracion SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Vincular el valor del nombre de usuario al statement
    sqlite3_bind_text(stmt, 1, nombreUsuario, -1, SQLITE_STATIC);

    // Ejecutar la consulta y extraer el ID del usuario
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        usuarioID = sqlite3_column_int(stmt, 0);
        printf("Usuario encontrado con ID: %d\n", usuarioID); // Agregue esta línea
    }
    else if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("No se encontró ningún registro para el usuario: %s\n", nombreUsuario); // Agregue esta línea
    }

    // Finalizar el statement y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return usuarioID;
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
                Cliente *cliente = db_buscar_cliente_por_usuarioID(usuario_validado->usuarioID);
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
                // Si se requiere admitir otros tipos de usuarios, agregar aqui el codigo correspondiente.
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

void db_depositar_dinero(const char *numero_cuenta, float cantidad)
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
        sqlite3_bind_double(stmt, 1, cantidad);
        sqlite3_bind_text(stmt, 2, numero_cuenta, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);

    // Registra la transaccion de deposito en la base de datos
    time_t fecha = time(NULL);
    db_agregar_transaccion(numero_cuenta, numero_cuenta, cantidad, fecha, DEPOSITO);

    sqlite3_exec(db, "COMMIT;", 0, 0, 0);
    cerrar_db(db);
}

void db_retirar_dinero(const char *numero_cuenta, float cantidad)
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
        sqlite3_bind_double(stmt, 1, cantidad);
        sqlite3_bind_text(stmt, 2, numero_cuenta, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", 0, 0, 0);
    cerrar_db(db);
}

void db_transferir_dinero(const char *cuenta_origen, const char *cuenta_destino, float cantidad)
{
    db_retirar_dinero(cuenta_origen, cantidad);
    db_depositar_dinero(cuenta_destino, cantidad);

    // Registrar las transacciones de retiro y depósito en la base de datos
    time_t fecha = time(NULL);
    db_agregar_transaccion(cuenta_origen, cuenta_destino, cantidad, fecha, RETIRADA);
    db_agregar_transaccion(cuenta_origen, cuenta_destino, cantidad, fecha, DEPOSITO);
}


void db_cerrar_cuenta(const char *numero_cuenta)
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
        sqlite3_bind_text(stmt, 1, numero_cuenta, -1, SQLITE_TRANSIENT);
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

CuentaBancaria *db_buscar_cuenta_por_numero(const char *numero_cuenta)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    CuentaBancaria *cuenta = NULL;

    int rc = sqlite3_open("deustobank.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    const char *sql = "SELECT cb.*, c.* FROM cuentas cb INNER JOIN clientes c ON c.clienteID = cb.clienteID WHERE cb.numeroCuenta = ?";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, numero_cuenta, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cuenta = (CuentaBancaria *)malloc(sizeof(CuentaBancaria));
        strncpy(cuenta->numeroCuenta, (const char *)sqlite3_column_text(stmt, 0), sizeof(cuenta->numeroCuenta));
        cuenta->saldo = (float)sqlite3_column_double(stmt, 1);
        cuenta->cliente = (Cliente *)malloc(sizeof(Cliente));
        cuenta->cliente->clienteID = sqlite3_column_int(stmt, 3);
        strncpy(cuenta->codigoBIC, (const char *)sqlite3_column_text(stmt, 4), sizeof(cuenta->codigoBIC));
        strncpy(cuenta->cliente->nombre, (const char *)sqlite3_column_text(stmt, 6), sizeof(cuenta->cliente->nombre));
        strncpy(cuenta->cliente->apellido, (const char *)sqlite3_column_text(stmt, 7), sizeof(cuenta->cliente->apellido));
        strncpy(cuenta->cliente->dni, (const char *)sqlite3_column_text(stmt, 8), sizeof(cuenta->cliente->dni));
        strncpy(cuenta->cliente->direccion, (const char *)sqlite3_column_text(stmt, 9), sizeof(cuenta->cliente->direccion));
        strncpy(cuenta->cliente->telefono, (const char *)sqlite3_column_text(stmt, 10), sizeof(cuenta->cliente->telefono));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return cuenta;
}

CuentaBancaria *db_buscar_cuenta_por_cliente(int clienteID)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    CuentaBancaria *cuenta = NULL;

    int rc = sqlite3_open("deustobank.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    const char *sql =   "SELECT cb.numeroCuenta, cb.saldo, cb.clienteID, cb.codigoBIC, "
                        "c.nombre, c.apellido, c.dni, c.direccion, c.telefono "
                        "FROM cuentas cb INNER JOIN clientes c ON c.clienteID = cb.clienteID WHERE cb.clienteID = ?";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error en la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, clienteID);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cuenta = (CuentaBancaria *)malloc(sizeof(CuentaBancaria));
        strncpy(cuenta->numeroCuenta, (const char *)sqlite3_column_text(stmt, 0), sizeof(cuenta->numeroCuenta));
        cuenta->saldo = (float)sqlite3_column_double(stmt, 1);
        cuenta->cliente = (Cliente *)malloc(sizeof(Cliente));
        cuenta->cliente->clienteID = sqlite3_column_int(stmt, 3);
        strncpy(cuenta->codigoBIC, (const char *)sqlite3_column_text(stmt, 4), sizeof(cuenta->codigoBIC));

        strncpy(cuenta->cliente->nombre, (const char *)sqlite3_column_text(stmt, 6), sizeof(cuenta->cliente->nombre));
        strncpy(cuenta->cliente->apellido, (const char *)sqlite3_column_text(stmt, 7), sizeof(cuenta->cliente->apellido));
        strncpy(cuenta->cliente->dni, (const char *)sqlite3_column_text(stmt, 8), sizeof(cuenta->cliente->dni));
        strncpy(cuenta->cliente->direccion, (const char *)sqlite3_column_text(stmt, 9), sizeof(cuenta->cliente->direccion));
        strncpy(cuenta->cliente->telefono, (const char *)sqlite3_column_text(stmt, 10), sizeof(cuenta->cliente->telefono));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return cuenta;
}

void db_agregar_transaccion(const char *numeroCuentaOrigen, const char *numeroCuentaDestino, float importe, time_t fecha, TipoTransaccion tipo)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char sql[500];

    rc = abrir_db(&db);
    if (rc)
    {
        printf("No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    snprintf(sql, sizeof(sql), "INSERT INTO transacciones (numeroCuentaOrigen, numeroCuentaDestino, importe, fecha, tipo) VALUES (?, ?, ?, ?, ?);");

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        printf("No se puede preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, numeroCuentaOrigen, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, numeroCuentaDestino, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, importe);
    sqlite3_bind_int64(stmt, 4, fecha);
    sqlite3_bind_int(stmt, 5, tipo);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        printf("Error al insertar la transaccion: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
}

void db_registrar_transaccion(Transaccion *transaccion)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if (abrir_db(&db) != SQLITE_OK)
    {
        printf("Error al abrir la base de datos.\n");
        return;
    }

    const char *sql = "INSERT INTO transacciones (numeroCuentaOrigen, numeroCuentaDestino, importe, fecha, tipo) VALUES (?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        printf("Error al preparar la declaracion: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, transaccion->numeroCuentaOrigen, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, transaccion->numeroCuentaDestino, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, transaccion->importe);

    // Convertir transaccion->fecha a una cadena de caracteres
    char fecha_str[20];
    strftime(fecha_str, sizeof(fecha_str), "%Y-%m-%d %H:%M:%S", localtime(&(transaccion->fecha)));

    // Pasar fecha_str como argumento a sqlite3_bind_text en lugar de transaccion->fecha
    sqlite3_bind_text(stmt, 4, fecha_str, -1, SQLITE_TRANSIENT);
    
    // Vincular el valor del tipo de transaccion
    sqlite3_bind_int(stmt, 5, transaccion->tipo);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        printf("Error al ejecutar la declaracion: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
}

int db_eliminar_transaccion(int id_transaccion)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (abrir_db(&db) != SQLITE_OK)
    {
        printf("Error al abrir la base de datos.\n");
        return -1;
    }

    const char *sql = "DELETE FROM transacciones WHERE transaccionID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, id_transaccion);

    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE)
    {
        printf("Error al eliminar la transaccion: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);

    return result == SQLITE_DONE ? 0 : -1;
}

Transaccion *db_listar_transacciones(const char *numero_cuenta, int *num_transacciones)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    Transaccion *transacciones = NULL;
    int count = 0;

    if (abrir_db(&db) != SQLITE_OK)
    {
        printf("Error al abrir la base de datos.\n");
        return NULL;
    }

    const char *sql = "SELECT transaccionID, numeroCuentaOrigen, numeroCuentaDestino, importe, fecha, tipo FROM transacciones WHERE numeroCuentaOrigen = ? OR numeroCuentaDestino = ? ORDER BY fecha DESC;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, numero_cuenta, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, numero_cuenta, -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count++;
        transacciones = realloc(transacciones, sizeof(Transaccion) * count);

        Transaccion *transaccion = &transacciones[count - 1];
        transaccion->transaccionID = sqlite3_column_int(stmt, 0);
        strncpy(transaccion->numeroCuentaOrigen, (const char *)sqlite3_column_text(stmt, 1), sizeof(transaccion->numeroCuentaOrigen));
        strncpy(transaccion->numeroCuentaDestino, (const char *)sqlite3_column_text(stmt, 2), sizeof(transaccion->numeroCuentaDestino));
        transaccion->importe = (float)sqlite3_column_double(stmt, 3);
        transaccion->fecha = (time_t)sqlite3_column_int64(stmt, 4);
        transaccion->tipo = sqlite3_column_int(stmt, 5);
    }

    *num_transacciones = count;

    sqlite3_finalize(stmt);
    cerrar_db(db);

    return transacciones;
}

Informe *db_mostrar_informe_financiero(const char *numero_cuenta)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT numero_cuenta, saldo_inicial, saldo_final, COUNT(tipo) as num_transacciones, SUM(cantidad) as total_cantidad, tipo FROM transacciones WHERE numero_cuenta = ? GROUP BY tipo";
    int rc;

    rc = sqlite3_open("deustobank.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "No se puede preparar la declaracion: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, numero_cuenta, -1, SQLITE_TRANSIENT);

    Informe *informe = malloc(sizeof(Informe));
    snprintf(informe->numeroCuenta, sizeof(informe->numeroCuenta), "%s", numero_cuenta);
    informe->saldoInicial = 0;
    informe->saldoFinal = 0;
    informe->numDepositos = 0;
    informe->totalDepositos = 0;
    informe->numRetiros = 0;
    informe->totalRetiros = 0;
    informe->numTransferenciasEnviadas = 0;
    informe->totalTransferenciasEnviadas = 0;
    informe->numTransferenciasRecibidas = 0;
    informe->totalTransferenciasRecibidas = 0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char *tipo = sqlite3_column_text(stmt, 5);
        int num_transacciones = sqlite3_column_int(stmt, 3);
        float total_cantidad = (float)sqlite3_column_double(stmt, 4);

        if (strcmp((const char *)tipo, "deposito") == 0)
        {
            informe->numDepositos = num_transacciones;
            informe->totalDepositos = total_cantidad;
        }
        else if (strcmp((const char *)tipo, "retiro") == 0)
        {
            informe->numRetiros = num_transacciones;
            informe->totalRetiros = total_cantidad;
        }
        else if (strcmp((const char *)tipo, "transferencia_enviada") == 0)
        {
            informe->numTransferenciasEnviadas = num_transacciones;
            informe->totalTransferenciasEnviadas = total_cantidad;
        }
        else if (strcmp((const char *)tipo, "transferencia_recibida") == 0)
        {
            informe->numTransferenciasRecibidas = num_transacciones;
            informe->totalTransferenciasRecibidas = total_cantidad;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return informe;
}

/*int get_numero_de_clientes(char sql[]){
    sqlite3 *db;
    abrir_db(db);
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        printf("Error preparing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    int numFilas = 0;
    do {
        result = sqlite3_step(stmt) ;
        if (result == SQLITE_ROW) {
            numFilas++;
        }
    } while (result == SQLITE_ROW);

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        printf("Error finalizing statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }
    return numFilas;
}*/
