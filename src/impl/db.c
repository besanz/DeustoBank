#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../dec/db.h"

int abrir_db(sqlite3 **db)
{
    int rc = sqlite3_open("deusto_bank.db", db);
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

Cliente *db_validar_credenciales(char *usuario, char *contrasena)
{
    // Implementar la función para validar las credenciales de un cliente
}

void db_registrar_cliente(Cliente *nuevo_cliente)
{
    sqlite3 *db;
    if (abrir_db(&db) != 0)
    {
        return;
    }

    const char *sql = "INSERT INTO clientes (nombre, apellido, dni, direccion, telefono) VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, nuevo_cliente->nombre, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, nuevo_cliente->apellido, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, nuevo_cliente->dni, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, nuevo_cliente->direccion, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, nuevo_cliente->telefono, -1, SQLITE_STATIC);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
}

void db_actualizar_cliente(int id_cliente, Cliente *datos_actualizados)
{
    sqlite3 *db;
    if (abrir_db(&db) != 0)
    {
        return;
    }

    const char *sql = "UPDATE clientes SET nombre=?, apellido=?, dni=?, direccion=?, telefono=? WHERE clienteID=?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, datos_actualizados->nombre, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, datos_actualizados->apellido, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, datos_actualizados->dni, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, datos_actualizados->direccion, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, datos_actualizados->telefono, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, id_cliente);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
}

void db_eliminar_cliente(int id_cliente)
{
    sqlite3 *db;
    if (abrir_db(&db) != 0)
    {
        return;
    }

    const char *sql = "DELETE FROM clientes WHERE clienteID=?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id_cliente);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
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

CuentaBancaria *db_crear_cuenta(Cliente *titular)
{
    sqlite3 *db;
    if (abrir_db(&db) != SQLITE_OK)
    {
        return NULL;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO cuentas (saldo, clienteID) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_double(stmt, 1, 0.0);
        sqlite3_bind_int(stmt, 2, titular->clienteID);
        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            int cuenta_id = sqlite3_last_insert_rowid(db);
            CuentaBancaria *cuenta = malloc(sizeof(CuentaBancaria));
            cuenta->numeroCuenta = cuenta_id;
            cuenta->saldo = 0.0;
            cuenta->cliente = titular;
            sqlite3_finalize(stmt);
            cerrar_db(db);
            return cuenta;
        }
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
    return NULL;
}

CuentaBancaria *db_buscar_cuenta_por_numero(int numero_cuenta)
{
    sqlite3 *db;
    if (abrir_db(&db) != SQLITE_OK)
    {
        return NULL;
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM cuentas WHERE numeroCuenta = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, numero_cuenta);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int cliente_id = sqlite3_column_int(stmt, 2);
            Cliente *cliente = db_buscar_cliente_por_id(cliente_id);

            if (cliente != NULL)
            {
                CuentaBancaria *cuenta = malloc(sizeof(CuentaBancaria));
                cuenta->numeroCuenta = sqlite3_column_int(stmt, 0);
                cuenta->saldo = sqlite3_column_double(stmt, 1);
                cuenta->cliente = cliente;

                sqlite3_finalize(stmt);
                cerrar_db(db);
                return cuenta;
            }
        }
    }
    sqlite3_finalize(stmt);
    cerrar_db(db);
    return NULL;
}

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