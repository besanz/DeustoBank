#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dec/cliente.h"
#include "dec/cuenta.h"
#include "dec/transaccion.h"
#include "dec/db.h"
#include "dec/usuario.h"

void pantalla_inicio();
void menu_principal();
void menu_administrador();
void menu_cliente();
void registro_usuario();
Usuario *inicio_sesion_usuario();
void menu_gestionar_clientes();
void menu_gestionar_cuentas();
void menu_gestionar_transferencias();

int main()
{
    db_inicializar();
    temporizador_pantalla_inicial();
    pantalla_inicio();
    return 0;
}

void pantalla_inicio()
{
    int opcion;
    Usuario *usuario;
    Cliente *cliente;

    do
    {
        printf("\nBienvenido a DeustoBank\n");
        printf("1. Iniciar sesion\n");
        printf("2. Registrarse\n");
        printf("3. Salir\n");
        printf("Por favor, elige una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            temporizador_carga_inicio_sesion();
            usuario = inicio_sesion_usuario();
            if (usuario != NULL)
            {
                if (usuario->tipo == 1)
                {
                    temporizador_inicio_sesion();
                    menu_administrador();
                }
                else
                {
                    temporizador_inicio_sesion();
                    cliente = db_buscar_cliente_por_usuarioID(usuario->usuarioID);
                    if (cliente != NULL)
                    {
                        if (cliente_tiene_cuenta(cliente->clienteID))
                        {
                            menu_cliente_con_cuenta(cliente, usuario);
                        }
                        else
                        {
                            menu_cliente_sin_cuenta(cliente, usuario);
                        }
                        free(cliente);
                    }
                    else
                    {
                        printf("Error al buscar el cliente asociado al usuario.\n");
                    }
                }
                free(usuario);
            }
            break;
        case 2:
            registro_usuario();
            break;
        case 3:
            temporizador_salida();
            break;
        default:
            printf("Opcion invalida. Por favor, intente de nuevo.\n");
            break;
        }
    } while (opcion != 3);
}




void registro_usuario()
{
    Cliente nuevo_cliente;
    Usuario nuevo_usuario;
    ////temporizador_pantalla_registro();
    printf("\n--- Registro de nuevo cliente ---\n");
    printf("Nombre de usuario: \n");
    scanf("%s", nuevo_usuario.nombreUsuario);
    printf("Contrasena: \n");
    scanf("%s", nuevo_usuario.contrasena);

    if (db_existe_usuario(nuevo_usuario.nombreUsuario))
    {
        printf("El nombre de usuario ya existe. Por favor, elija otro.\n");
        return;
    }

    printf("Nombre: ");
    scanf("%s", nuevo_cliente.nombre);
    printf("Apellido: ");
    scanf("%s", nuevo_cliente.apellido);
    printf("DNI: ");
    scanf("%s", nuevo_cliente.dni);
    printf("Direccion: ");
    scanf("%s", nuevo_cliente.direccion);
    printf("Telefono: ");
    scanf("%s", nuevo_cliente.telefono);

    // Registrar el nuevo usuario y obtener el ID de usuario generado
    db_registrar_usuario(&nuevo_usuario);
    nuevo_cliente.usuarioID = db_obtener_usuarioID(nuevo_usuario.nombreUsuario);
    ////temporizador_registro_usuario();
    // Pase el nuevo_usuario como argumento adicional
    db_registrar_cliente(&nuevo_cliente, &nuevo_usuario);
    printf("Cliente registrado exitosamente.\n");
}

Usuario *inicio_sesion_usuario()
{

    char nombreUsuario[50], contrasena[50];
    printf("\n--- Inicio de sesion ---\n");
    printf("Nombre de usuario: ");
    scanf("%s", nombreUsuario);
    printf("Contrasena: ");
    scanf("%s", contrasena);

    Usuario *usuario = db_validar_credenciales(nombreUsuario, contrasena);
    Cliente *cliente = NULL;
    if (usuario != NULL)
    {
        cliente = db_buscar_cliente_por_usuarioID(usuario->usuarioID);
    }

    if (cliente != NULL)
    {
        Usuario *usuario = (Usuario *)malloc(sizeof(Usuario));
        usuario->usuarioID = cliente->clienteID;
        strcpy(usuario->nombreUsuario, nombreUsuario);
        strcpy(usuario->contrasena, contrasena);
        usuario->tipo = usuario->tipo;
        usuario->datos = cliente;
        printf("Inicio de sesion exitoso.\n");
        return usuario;
    }
    else
    {
        printf("Error: Credenciales invalidas o usuario no encontrado.\n");
        return NULL;
    }
}

void menu_administrador()
{
    int opcion;

    do
    {
        system("cls");
        printf("1. Gestionar clientes\n");
        printf("2. Gestionar cuentas\n");
        printf("3. Gestionar transferencias\n");
        printf("4. Salir\n");
        printf("Por favor, elija una opcion: ");

        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            //menu_gestionar_clientes();
            break;
        case 2:
            // menu_gestionar_cuentas_cliente();
            break;
        case 3:
            // menu_gestionar_transferencias();
            break;
        case 4:
            printf("Gracias por utilizar nuestro servicio.\n");
            break;
        default:
            printf("Opcion invalida. Por favor, elija una opcion del menu.\n");
            break;
        }
    } while (opcion != 4);
}

/*
void menu_gestionar_clientes()
{
    int opcion;

    do {
        system("cls");
        printf("1. Mostrar lista de clientes");
        printf("2. Actualizar cliente\n");
        printf("3. Eliminar cliente\n");
        printf("4. Salir\n");
        printf("Por favor, elija una opcion: ");

        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                
                system("cls");
                int tamanyo=get_numero_filas_cliente("SELECT * FROM Cliente");
                Cliente * listaClientes= malloc(sizeof(Cliente));
                for (int i=0; i<tamanyo;i++){
                    printf("Cliente %i- %s %s\n", listaClientes[i].clienteID, listaClientes[i].nombre, listaClientes[i].apellido);
                }
                free(listaClientes);
                
                break;
            case 2:
                //Falta terminar

                Cliente *nuevo_cliente;
                printf("Introduzca el id de cliente que quiera actualizar sus datos:\n");
                scanf("%d", nuevo_cliente->clienteID);
                printf("Introduzca los datos actualizados del cliente:\n");
                printf("Nombre de usuario: ");
                printf("Nombre: ");
                scanf("%s", nuevo_cliente->nombre);
                printf("Apellido: ");
                scanf("%s", nuevo_cliente->apellido);
                printf("DNI: ");
                scanf("%s", nuevo_cliente->dni);
                printf("Direccion: ");
                scanf("%s", nuevo_cliente->direccion);
                printf("Telefono: ");
                scanf("%s", nuevo_cliente->telefono);
                
                //db_actualizar_cliente(nuevo_cliente->clienteID, nuevo_cliente, nuevo_usuario);
                break;
            case 3:
                int id_eliminado;
                printf("Introduzca el id del cliente que quiere eliminar: \n");
                scanf("%i", id_eliminado);
                Cliente *cliente_a_eliminar=buscar_cliente_por_id(id_eliminado);
                eliminar_cliente(cliente_a_eliminar);
                break;
            case 4:
                printf("Gracias por utilizar nuestro servicio.\n");
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 4);

}
*/
/*
void menu_gestionar_cuentas()
{
    int opcion;

    do {
        system("cls");
        printf("1. Mostrar informacion de la cuenta\n");
        printf("2. Eliminar cuenta\n");
        printf("3. Salir\n");
        printf("Por favor, elija una opcion: ");

        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Codigo para mostrar informacion de la cuenta
                break;
            case 2:
                // Codigo para eliminar la cuenta
                break;
            case 3:
                printf("Gracias por utilizar nuestro servicio.\n");
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 3);

}
void menu_gestionar_transacciones()
{
    int opcion;

    do {
        system("cls");
        printf("1. Mostrar informe financiero\n");
        printf("2. Eliminar transaccion\n");
        printf("3. Salir\n");
        printf("Por favor, elija una opcion: ");

        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Codigo para mostrar el informe financiero
                break;
            case 2:
                // Codigo para eliminar una transaccion
                break;
            case 3:
                printf("Gracias por utilizar nuestro servicio.\n");
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 3);

}

*/