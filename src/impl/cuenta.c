#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../dec/cuenta.h"
#include "../dec/db.h"
#include "../dec/usuario.h"
#include "../dec/cliente.h"


void generar_numero_cuenta(char *numero_cuenta) {
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }

    const char *prefijo = "DEUSTOBNK";
    unsigned long long numero_aleatorio;

    do {
        numero_aleatorio = (unsigned long long)rand() * (unsigned long long)rand();
        sprintf(numero_cuenta, "%s%017llu", prefijo, numero_aleatorio % 100000000000000000ULL);
    } while (cuenta_existe(numero_cuenta));
}


void crear_cuenta(int clienteID) {
    char numero_cuenta[27];
    char codigoBIC[12] = "DBANKSPAINSS";
    generar_numero_cuenta(numero_cuenta);

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    if (abrir_db(&db)) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *sql = "INSERT INTO cuentas (numeroCuenta, saldo, clienteID, codigoBIC) VALUES (?, 0, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, numero_cuenta, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, clienteID);
        sqlite3_bind_text(stmt, 3, codigoBIC, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);      

        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Error al crear la cuenta bancaria: %s\n", sqlite3_errmsg(db));
        } else {
            printf("Cuenta bancaria creada exitosamente.\n");
            printf("Numero de cuenta: %s\n", numero_cuenta);
        }
    } else {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    cerrar_db(db);
}

int cuenta_existe(const char *numero_cuenta) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int cuenta_encontrada = 0;

    if (abrir_db(&db)) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    const char *sql = "SELECT COUNT(*) FROM cuentas WHERE numeroCuenta = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, numero_cuenta, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            cuenta_encontrada = sqlite3_column_int(stmt, 0);
        } else {
            fprintf(stderr, "Error al buscar cuentas: %s\n", sqlite3_errmsg(db));
        }
    } else {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);

    return cuenta_encontrada;
}

void mostrar_informacion_cuenta(int clienteID) 
{
    CuentaBancaria *cuenta = db_buscar_cuenta_por_clienteID(clienteID);
    if (cuenta)
    {
        printf("\nNumero de cuenta: %s\n", cuenta->numeroCuenta);
        printf("Saldo: %.2f EUR.\n", cuenta->saldo);
        printf("Titular: %s %s\n", cuenta->cliente->nombre, cuenta->cliente->apellido);
    }
    else
    {
        printf("No se encontro la cuenta con el numero %s\n", cuenta->numeroCuenta);
    }
}


void depositar_dinero(int clienteID, float cantidad)
{
    if (cantidad != 0)
    {
        time_t fecha = time(NULL);
        db_agregar_transaccion_por_clienteID(clienteID, clienteID, cantidad, fecha, DEPOSITO);
        db_depositar_dinero(clienteID, cantidad);
    }
    else
    {
        printf("\nPor favor, introduzca una cantidad valida: %d\n", clienteID);
    }
}

void retirar_dinero(int clienteID, float cantidad)
{
    if (cantidad != 0)
    {
        CuentaBancaria *cuenta = db_buscar_cuenta_por_clienteID(clienteID);
        if (cuenta == NULL)
        {
            printf("No se pudo encontrar la cuenta del cliente.\n");
            return;
        }

        if (cuenta->saldo < cantidad)
        {
            printf("Saldo insuficiente para realizar la transaccion.\n");
            free(cuenta);
            return;
        }

        time_t fecha = time(NULL);
        db_agregar_transaccion_por_clienteID(clienteID, clienteID, cantidad, fecha, RETIRADA);
        db_retirar_dinero(clienteID, cantidad);

        free(cuenta);
    }
    else
    {
        printf("\nPor favor, introduzca una cantidad valida.\n");
    }
}

void transferir_dinero(int clienteID_origen, int clienteID_destino, float cantidad)
{
    if (cantidad != 0)
    {
    time_t fecha = time(NULL);
    db_agregar_transaccion_por_clienteID(clienteID_origen, clienteID_destino, cantidad, fecha, TRANSFERENCIA);
    db_transferir_dinero(clienteID_origen, clienteID_destino, cantidad);
    }else
    {
        printf("\nPor favor, introduzca una cantidad valida.\n");
    }
}

void cerrar_cuenta(int clienteID)
{
    db_cerrar_cuenta(clienteID);
}

CuentaBancaria *buscar_cuenta_por_numero(const char *numero_cuenta)
{
    return db_buscar_cuenta_por_numero(numero_cuenta);
}

CuentaBancaria *buscar_cuenta_por_clienteID(int clienteID)
{
    return db_buscar_cuenta_por_clienteID(clienteID);
}



