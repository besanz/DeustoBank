#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../dec/cliente.h"
#include "../dec/usuario.h"
#include "../dec/db.h"
#include "../dec/cuenta.h"
#include "../dec/transaccion.h"

Cliente *iniciar_sesion(char *usuario, char *contrasena)
{
    Usuario *usuario_validado = db_validar_credenciales(usuario, contrasena);

    if (usuario_validado != NULL && usuario_validado->tipo == CLIENTE)
    {
        Cliente *cliente = (Cliente *)usuario_validado->datos;
        return cliente;
    }

    return NULL;
}

bool verificar_credenciales(const char *nombreUsuario, const char *contrasena)
{
    Usuario *usuario = db_validar_credenciales(nombreUsuario, contrasena);
    if (usuario != NULL)
    {
        free(usuario);
        return true;
    }
    return false;
}

void registrar_cliente(Cliente *nuevo_cliente, Usuario *nuevo_usuario)
{
    db_registrar_cliente(nuevo_cliente, nuevo_usuario);
}

void actualizar_cliente(int id_cliente, Cliente *datos_actualizados, Usuario *usuario_actualizado)
{
    db_actualizar_cliente(id_cliente, datos_actualizados, usuario_actualizado);
}

void eliminar_cliente(int id_cliente)
{
    db_eliminar_cliente(id_cliente);
}

Cliente *buscar_cliente_por_id(int id_cliente)
{
    return db_buscar_cliente_por_id(id_cliente);
}

void mostrar_info_cliente(Cliente *cliente)
{
    printf("ID de cliente: %d\n", cliente->clienteID);
    printf("Nombre: %s\n", cliente->nombre);
    printf("Apellido: %s\n", cliente->apellido);
    printf("Direccion: %s\n", cliente->direccion);
    printf("Telefono: %s\n", cliente->telefono);
    printf("DNI: %s\n", cliente->dni);
}

void mostrar_transacciones(Transaccion *transacciones, int num_transacciones)
{
    printf("\nTransacciones:\n");
    for (int i = 0; i < num_transacciones; i++)
    {
        printf("Transaccion %d:\n", i + 1);
        printf("  ID: %d\n", transacciones[i].transaccionID);
        printf("  Cuenta Origen: %d\n", transacciones[i].numeroCuentaOrigen);
        printf("  Cuenta Destino: %d\n", transacciones[i].numeroCuentaDestino);
        printf("  Cantidad: %.2f\n", transacciones[i].importe);
        printf("  Fecha: %s\n", ctime(&transacciones[i].fecha)); // Convertir time_t a string legible
    }
}

void mostrar_informe(Informe *informe)
{
    printf("\nInforme financiero:\n");
    printf("  Balance inicial: %.2f\n", informe->saldoInicial);
    printf("  Balance final: %.2f\n", informe->saldoFinal);
    printf("  Total depositado: %.2f\n", informe->totalDepositos);
    printf("  Total retirado: %.2f\n", informe->totalRetiros);
    printf("  Total transferido: %.2f\n", informe->totalTransferenciasEnviadas);
}

int cliente_tiene_cuenta(int clienteID)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int cuenta_encontrada = 0;

    if (abrir_db(&db))
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    const char *sql = "SELECT COUNT(*) FROM cuentas WHERE clienteID = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, clienteID);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            cuenta_encontrada = sqlite3_column_int(stmt, 0);
        }
        else
        {
            fprintf(stderr, "Error al buscar cuentas: %s\n", sqlite3_errmsg(db));
        }
    }
    else
    {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);

    return cuenta_encontrada;
}

void menu_cliente_sin_cuenta(Cliente *cliente, Usuario *usuario)
{
    int opcion;
    char contrasena[100];

    do
    {
        printf("Bienvenido, %s %s\n", cliente->nombre, cliente->apellido);
        printf("1. Abrir cuenta\n");
        printf("2. Ver informacion del cliente\n");
        printf("3. Cerrar sesion\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            printf("Por favor, verifica tu contrasena: ");
            scanf("%s", contrasena);
            if (strcmp(contrasena, usuario->contrasena) == 0)
            {
                crear_cuenta(cliente->clienteID);
                menu_cliente_con_cuenta(cliente, usuario);
            }
            else
            {
                printf("Contrasena incorrecta. Intenta de nuevo.\n");
            }
            break;
        case 2:
            mostrar_informacion_cliente(cliente);
            break;
        case 3:
            printf("Cerrando sesion...\n");
            break;
        default:
            printf("Opcion invalida, por favor intente nuevamente.\n");
        }
    } while (opcion != 3);
}

void menu_cliente_con_cuenta(Cliente *cliente, Usuario *usuario)
{
    int opcion;
    CuentaBancaria *cuenta = NULL;
    int num_transacciones;
    Transaccion *transacciones;
    Informe *informe;
    cuenta = obtener_cuenta_por_clienteID(cliente->clienteID);

    while (1)
    {
        printf("\n");
        printf("1. Mostrar informacion de la cuenta\n");
        printf("2. Depositar dinero\n");
        printf("3. Retirar dinero\n");
        printf("4. Transferir dinero\n");
        printf("5. Ver historial de transacciones\n");
        printf("6. Ver informe financiero\n");
        printf("7. Cerrar sesion\n");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        float cantidad;
        char numeroCuentaDestino[27];

        switch (opcion)
        {
        case 1:
            mostrar_informacion_cuenta(cuenta->numeroCuenta);
            break;
        case 2:
            printf("Introduzca la cantidad que desea depositar: ");
            scanf("%f", &cantidad);
            depositar_dinero(cuenta->numeroCuenta, cantidad);
            break;
        case 3:
            printf("Introduzca la cantidad que desea retirar: ");
            scanf("%f", &cantidad);
            retirar_dinero(cuenta->numeroCuenta, cantidad);
            break;
        case 4:
            printf("Introduzca el numero de cuenta destino: ");
            scanf("%s", numeroCuentaDestino);
            printf("Introduzca la cantidad que desea transferir: ");
            scanf("%f", &cantidad);
            transferir_dinero(cuenta->numeroCuenta, numeroCuentaDestino, cantidad);
            break;
        case 5:
            transacciones = listar_transacciones(cuenta->numeroCuenta, &num_transacciones);
            mostrar_transacciones(transacciones, num_transacciones);
            break;
        case 6:
            informe = mostrar_informe_financiero(cuenta->numeroCuenta);
            imprimir_informe(informe);
            break;
        case 7:
            printf("Cerrando sesion...\n");
            return;
        default:
            printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
            break;
        }
    }
}

void mostrar_informacion_cliente(Cliente *cliente)
{
    if (cliente == NULL)
    {
        printf("Cliente no encontrado.\n");
        return;
    }

    printf("ID del cliente: %d\n", cliente->clienteID);
    printf("Nombre: %s %s\n", cliente->nombre, cliente->apellido);
    printf("DNI: %s\n", cliente->dni);
    printf("Dirección: %s\n", cliente->direccion);
    printf("Teléfono: %s\n", cliente->telefono);
}

CuentaBancaria *obtener_cuenta_por_clienteID(int clienteID)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    CuentaBancaria *cuenta = NULL;

    if (abrir_db(&db) != SQLITE_OK)
    {
        printf("Error al abrir la base de datos.\n");
        return NULL;
    }

    const char *sql = "SELECT * FROM cuentas WHERE clienteID = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        cerrar_db(db);
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, clienteID);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cuenta = (CuentaBancaria *)malloc(sizeof(CuentaBancaria));
        strncpy(cuenta->numeroCuenta, (const char *)sqlite3_column_text(stmt, 0), sizeof(cuenta->numeroCuenta));
        cuenta->saldo = (float)sqlite3_column_double(stmt, 1);
        cuenta->cliente = buscar_cliente_por_id(sqlite3_column_int(stmt, 2));
        strncpy(cuenta->codigoBIC, (const char *)sqlite3_column_text(stmt, 3), sizeof(cuenta->codigoBIC));
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
    return cuenta;
}

void imprimir_informe(Informe *informe)
{
    if (informe == NULL)
    {
        printf("Informe no encontrado.\n");
        return;
    }

    printf("Número de cuenta: %s\n", informe->numeroCuenta);
    printf("Saldo inicial: %.2f\n", informe->saldoInicial);
    printf("Saldo final: %.2f\n", informe->saldoFinal);
    printf("Número de depósitos: %d\n", informe->numDepositos);
    printf("Total depósitos: %.2f\n", informe->totalDepositos);
    printf("Número de retiros: %d\n", informe->numRetiros);
    printf("Total retiros: %.2f\n", informe->totalRetiros);
    printf("Número de transferencias enviadas: %d\n", informe->numTransferenciasEnviadas);
    printf("Total transferencias enviadas: %.2f\n", informe->totalTransferenciasEnviadas);
    printf("Número de transferencias recibidas: %d\n", informe->numTransferenciasRecibidas);
    printf("Total transferencias recibidas: %.2f\n", informe->totalTransferenciasRecibidas);
}
