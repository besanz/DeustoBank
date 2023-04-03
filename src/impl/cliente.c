#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dec/cliente.h"
#include "../dec/db.h"
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

void menu_cliente_con_cuenta(Cliente *cli)
{
    int opcion;

    do
    {
        printf("\nMenu Cliente:\n");
        printf("1. Ver informacion de la cuenta\n");
        printf("2. Depositar dinero\n");
        printf("3. Retirar dinero\n");
        printf("4. Transferir dinero\n");
        printf("5. Ver transacciones\n");
        printf("6. Mostrar informe financiero\n");
        printf("7. Cerrar sesion\n");
        printf("Por favor, elige una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            // Mostrar información de la cuenta
            break;
        case 2:
            // Depositar dinero
            break;
        case 3:
            // Retirar dinero
            break;
        case 4:
            // Transferir dinero
            break;
        case 5:
            // Ver transacciones
            break;
        case 6:
            // Mostrar informe financiero
            break;
        case 7:
            // Cerrar sesión
            break;
        default:
            printf("Opcion invalida. Por favor, intente de nuevo.\n");
            break;
        }
    } while (opcion != 7);
}
