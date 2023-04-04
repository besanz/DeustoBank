#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dec/cliente.h"
#include "dec/cuenta.h"
#include "dec/transaccion.h"
#include "dec/db.h"

void pantalla_inicio();
void menu_principal();
void menu_administrador();
void menu_cliente();
void registro_usuario();
Usuario *inicio_sesion_usuario();
void temporizador_pantalla_inicial();
void temporizador_registro_usuario();
void temporizador_pantalla_registro();
void temporizador_salida();
void temporizador_crear_cuenta();
/* void menu_gestionar_clientes();
void menu_gestionar_cuentas();
void menu_gestionar_transferencias(); */

int main()
{
    db_inicializar();
    pantalla_inicio();
    return 0;
}

void pantalla_inicio()
{
    int opcion;

    do
    {
        ////temporizador_pantalla_inicial();
        printf("\nBienvenido a DeustoBank\n");
        printf("1. Iniciar sesion\n");
        printf("2. Registrarse\n");
        printf("3. Salir\n");
        printf("Por favor, elige una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
        {
            Usuario *usuario = inicio_sesion_usuario();
            if (usuario != NULL)
            {
                if (usuario->tipo == ADMINISTRADOR)
                {
                    menu_administrador();
                }
                else
                {
                    Cliente *cli = db_buscar_cliente_por_usuario_id(usuario->usuarioID);
                    if (cli != NULL)
                    {
                        if (cliente_tiene_cuenta(cli->clienteID))
                        {
                            menu_cliente_con_cuenta(cli, usuario);
                        }
                        else
                        {
                            menu_cliente_sin_cuenta(cli);
                        }
                        free(cli);
                    }
                    else
                    {
                        printf("Error al buscar el cliente asociado al usuario.\n");
                    }
                }
                free(usuario);
            }
            break;
        }
        case 2:
            registro_usuario();
            break;
        case 3:
            // temporizador_salida();
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
    printf("Nombre de usuario: ");
    scanf("%s", nuevo_usuario.nombreUsuario);
    printf("Contrasena: ");
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
    nuevo_cliente.usuario_id = db_obtener_usuario_id(nuevo_usuario.nombreUsuario);
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
        cliente = db_buscar_cliente_por_usuario_id(usuario->usuarioID);
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
            // menu_gestionar_clientes();
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
                Cliente ** listaClientes= malloc(sizeof(Cliente*));
                for (int i=0; i<tamanyo;i++){
                    printf("Cliente %i- %s %s\n", listaClientes[i].id, listaClientes[i].nombre, listaClientes[i].apellido);
                }
                free(listaClientes);
                break;
            case 2:
                Cliente nuevo_cliente;
                printf("Introduzca el id de cliente que quiera actualizar sus datos:\n")
                scanf("%d", nuevo_cliente.id);
                printf("Introduzca los datos actualizados del cliente:\n")
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
                db_actualizar_cliente(, Cliente *datos_actualizados, Usuario *usuario_actualizado);
                break;
            case 3:
                Cliente cliente_a_eliminar;
                printf("Introduzca el id del cliente que quiere eliminar: \n");
                scanf("%i", cliente_a_eliminar.id)
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


void temporizador_pantalla_inicial(){
     system("cls");
        printf("Descargando actualizaciones.");
        sleep(1);
        system("cls");
        printf("Descargando actualizaciones..");
        sleep(1);
        system("cls");
        printf("Descargando actualizaciones...");
        sleep(1);
        system("cls");
        printf("Inicializando  DeustoBank.");
        sleep(1);
        system("cls");
        printf("Inicializando  DeustoBank..");
        sleep(1);
        system("cls");
        printf("Inicializando  DeustoBank...");
        sleep(1);
        system("cls");
}
void temporizador_pantalla_registro(){
     system("cls");
        printf("Entrando en registro de usuarios.");
        sleep(1);
        system("cls");
        printf("Entrando en registro de usuarios..");
        sleep(1);
        system("cls");
        printf("Entrando en registro de usuarios...");
        sleep(1);
        system("cls");
}
void temporizador_registro_usuario(){
     system("cls");
        printf("Registrando usuario.");
        sleep(1);
        system("cls");
        printf("Registrando usuario..");
        sleep(1);
        system("cls");
        printf("Registrando usuario...");
        sleep(1);
        system("cls");
        printf("Registrando usuario.");
        sleep(1);
        system("cls");
        printf("Registrando usuario..");
        sleep(1);
        system("cls");
        printf("Registrando usuario...");
        sleep(1);
        system("cls");
}
void temporizador_pantalla_inicio_sesion(){
     system("cls");
        printf("Iniciando sesion.");
        sleep(1);
        system("cls");
        printf("Iniciando sesion..");
        sleep(1);
        system("cls");
        printf("Iniciando sesion...");
        sleep(1);
        system("cls");
}
void temporizador_salida(){
     system("cls");
        printf("Cerrando sesion.");
        sleep(1);
        system("cls");
        printf("Cerrando sesion..");
        sleep(1);
        system("cls");
        printf("Cerrando sesion...");
        sleep(1);
        system("cls");
        printf("Saliendo.");
        sleep(1);
        system("cls");
        printf("Saliendo..");
        sleep(1);
        system("cls");
        printf("Saliendo...");
        sleep(1);
        system("cls");
        printf("Gracias por utilizar DeustoBank, vuelva pronto");
        sleep(1);
        system("cls");
}
void temporizador_crear_cuenta(){
     system("cls");
        printf("Creando cuenta.");
        sleep(1);
        system("cls");
        printf("Creando cuenta..");
        sleep(1);
        system("cls");
        printf("Creando cuenta...");
        sleep(1);
        system("cls");
        printf("Validando datos.");
        sleep(1);
        system("cls");
        printf("Validando datos..");
        sleep(1);
        system("cls");
        printf("Validando datos...");
        sleep(1);
        system("cls");
        printf("Cuenta creada con exito");
        sleep(1);
        system("cls");
}
*/