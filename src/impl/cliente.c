#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../dec/cliente.h"
#include "../dec/usuario.h"
#include "../dec/db.h"
#include "../dec/cuenta.h"
#include "../dec/transaccion.h"
#include "../dec/usuario.h"
#include "../dec/usuario.h"

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

Cliente *buscar_cliente_por_cuenta(const char *numero_cuenta)
{
    sqlite3 *db;
    Cliente *cliente = NULL;

    if (abrir_db(&db) != SQLITE_OK)
    {
        return NULL;
    }

    const char *sql = "SELECT c.clienteID, c.usuarioID, c.nombre, c.apellido, c.dni, c.direccion, c.telefono "
                      "FROM clientes c "
                      "JOIN cuentas cu ON c.clienteID = cu.clienteID "
                      "WHERE cu.numeroCuenta = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, numero_cuenta, -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            cliente = (Cliente *)malloc(sizeof(Cliente));
            cliente->clienteID = sqlite3_column_int(stmt, 0);
            cliente->usuarioID = sqlite3_column_int(stmt, 1);
            strncpy(cliente->nombre, (const char *)sqlite3_column_text(stmt, 2), sizeof(cliente->nombre));
            strncpy(cliente->apellido, (const char *)sqlite3_column_text(stmt, 3), sizeof(cliente->apellido));
            strncpy(cliente->dni, (const char *)sqlite3_column_text(stmt, 4), sizeof(cliente->dni));
            strncpy(cliente->direccion, (const char *)sqlite3_column_text(stmt, 5), sizeof(cliente->direccion));
            strncpy(cliente->telefono, (const char *)sqlite3_column_text(stmt, 6), sizeof(cliente->telefono));
        }
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
    return cliente;
}

void mostrar_informacion_cliente(Cliente *cliente)
{
    if (cliente == NULL)
    {
        printf("Cliente no encontrado.\n");
        return;
    }
    else
    {
        printf("\nInformacion del cliente:\n");
        printf("ID de cliente: %d\n", cliente->clienteID);
        printf("Nombre: %s\n", cliente->nombre);
        printf("Apellido: %s\n", cliente->apellido);
        printf("Direccion: %s\n", cliente->direccion);
        printf("Telefono: %s\n", cliente->telefono);
        printf("DNI: %s\n", cliente->dni);
    }
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
        printf("\n1. Abrir cuenta\n");
        printf("2. Ver informacion del cliente\n");
        printf("3. Cerrar sesion\n");
        printf("\nSeleccione una opcion: ");
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
    CuentaBancaria *cuenta = db_buscar_cuenta_por_clienteID(cliente->clienteID);
    Transaccion *transacciones = NULL;
    int num_transacciones = 0;
    Informe *informe = NULL;

    do
    {
        printf("\nBienvenido al menu de DeustoBank.\n");
        printf("\nQue operacion deseas realizar?\n");
        printf("\n1. Mostrar informacion del cliente\n");
        printf("2. Mostrar informacion de la cuenta\n");
        printf("3. Depositar dinero\n");
        printf("4. Retirar dinero\n");
        printf("5. Realizar transferencia\n");
        printf("6. Mostrar transacciones\n");
        printf("7. Mostrar informe financiero\n");
        printf("8. Cerrar cuenta\n");
        printf("9. Salir\n");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);
        fflush(stdin);

        switch (opcion)
        {
        case 1:
            mostrar_informacion_cliente(cliente);
            break;
        case 2:
            mostrar_informacion_cuenta(cliente->clienteID);
            break;
        case 3:
        {
            float cantidad = 0;
            printf("Ingrese la cantidad a depositar: ");
            scanf("%f", &cantidad);
            depositar_dinero(cliente->clienteID, cantidad);
            printf("Has depositado %.2f euros en su cuenta.\n", cantidad);
            break;
        }
        case 4:
        {
            float cantidad;
            printf("Ingrese la cantidad a retirar: ");
            scanf("%f", &cantidad);
            retirar_dinero(cliente->clienteID, cantidad);
            printf("Se retiraron %.2f euros de su cuenta.\n", cantidad);
            break;
        }
        case 5:
            realizar_transferencia(cliente->clienteID);
            break;
        case 6:
            transacciones = listar_transacciones(cuenta->numeroCuenta, &num_transacciones);
            if (transacciones != NULL)
            {
                mostrar_transacciones(transacciones, num_transacciones);
                free(transacciones);
            }
            else
            {
                printf("No hay transacciones disponibles para mostrar.\n");
            }
            break;
        case 7:
            informe = mostrar_informe_financiero(cuenta->numeroCuenta);
            if (informe != NULL)
            {
                mostrar_informe(informe);
                free(informe);
            }
            else
            {
                printf("No se pudo generar el informe financiero.\n");
            }
            break;
        case 8:
            cerrar_cuenta(cuenta->cliente->clienteID);
            printf("Cuenta cerrada exitosamente.\n");
            menu_cliente_sin_cuenta(cliente, usuario);
            break;
        case 9:
            printf("Saliendo del menu del cliente...\n");
            break;
        default:
            printf("Opcion no valida. Por favor, intente de nuevo.\n");
            break;
        }
    } while (opcion != 9);

    // Liberar recursos
    if (cuenta != NULL)
    {
        free(cuenta);
    }
}

void imprimir_informe(Informe *informe)
{
    if (informe == NULL)
    {
        printf("Informe no encontrado.\n");
        return;
    }

    printf("Numero de cuenta: %s\n", informe->numeroCuenta);
    printf("Saldo inicial: %.2f\n", informe->saldoInicial);
    printf("Saldo final: %.2f\n", informe->saldoFinal);
    printf("Numero de depositos: %d\n", informe->numDepositos);
    printf("Total depositos: %.2f\n", informe->totalDepositos);
    printf("Numero de retiros: %d\n", informe->numRetiros);
    printf("Total retiros: %.2f\n", informe->totalRetiros);
    printf("Numero de transferencias enviadas: %d\n", informe->numTransferenciasEnviadas);
    printf("Total transferencias enviadas: %.2f\n", informe->totalTransferenciasEnviadas);
    printf("Numero de transferencias recibidas: %d\n", informe->numTransferenciasRecibidas);
    printf("Total transferencias recibidas: %.2f\n", informe->totalTransferenciasRecibidas);
}

void liberar_cliente(Cliente *cliente)
{
    if (cliente != NULL)
    {
        free(cliente);
    }
}
