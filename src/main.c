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
                    menu_cliente();
                }
                free(usuario);
            }
            break;
        }
        case 2:
            registro_usuario();
            break;
        case 3:
            printf("Gracias por usar DeustoBank. Hasta luego!\n");
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

    printf("\n--- Registro de nuevo cliente ---\n");
    printf("Nombre de usuario: ");
    scanf("%s", nuevo_usuario.nombreUsuario);
    printf("Contraseña: ");
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

    db_registrar_usuario(&nuevo_usuario, "cliente");
    nuevo_cliente.usuario_id = db_obtener_usuario_id(nuevo_usuario.nombreUsuario);

    // Pase el nuevo_usuario como argumento adicional
    db_registrar_cliente(&nuevo_cliente, &nuevo_usuario);
    printf("Cliente registrado exitosamente.\n");
}

void db_registrar_cliente(Cliente *nuevo_cliente, Usuario *usuario)
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

    // Use el ID de usuario del parámetro usuario para vincular el cliente y el usuario
    snprintf(sql, sizeof(sql), "INSERT INTO clientes (usuario_id, nombre, apellido, dni, direccion, telefono) "
                               "VALUES (%d, '%s', '%s', '%s', '%s', '%s');",
             usuario->usuarioID, nuevo_cliente->nombre, nuevo_cliente->apellido, nuevo_cliente->dni, nuevo_cliente->direccion, nuevo_cliente->telefono);

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al registrar el cliente: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    cerrar_db(db);
}

Usuario *inicio_sesion_usuario()
{
    char nombreUsuario[50], contrasena[50];
    printf("\n--- Inicio de sesion ---\n");
    printf("Nombre de usuario: ");
    scanf("%s", nombreUsuario);
    printf("contrasena: ");
    scanf("%s", contrasena);

    Cliente *cliente = db_validar_credenciales(nombreUsuario, contrasena);

    if (cliente != NULL)
    {
        Usuario *usuario = (Usuario *)malloc(sizeof(Usuario));
        usuario->usuarioID = cliente->clienteID;
        strcpy(usuario->nombreUsuario, nombreUsuario);
        strcpy(usuario->contrasena, contrasena);
        usuario->tipo = CLIENTE;
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

void mostrar_menu_principal()
{
    int opcion;

    do
    {
        printf("\nBienvenido al sistema de gestion de clientes\n");
        printf("1. Iniciar sesion\n");
        printf("2. Registrarse\n");
        printf("3. Salir\n");
        printf("Por favor, elija una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            // Iniciar Sesion
            break;
        case 2:
            registro_usuario();
            break;
        case 3:
            printf("Gracias por utilizar nuestro sistema.Hasta pronto!\n");
            break;
        default:
            printf("Opcion no valida. Por favor, elija una opcion del menu.\n");
        }
    } while (opcion != 3);
}

void menu_administrador()
{
    int opcion;

    do
    {
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

void menu_cliente()
{
    int opcion;

    do
    {
        printf("1. Ver informacion de la cuenta\n");
        printf("2. Realizar deposito\n");
        printf("3. Realizar retiro\n");
        printf("4. Realizar transferencia\n");
        printf("5. Ver transacciones\n");
        printf("6. Salir\n");
        printf("Por favor, elija una opcion: ");

        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            // mostrar_informacion_cuenta(num);
            break;
        case 2:
            // menu_realizar_deposito();
            break;
        case 3:
            // menu_realizar_retiro();
            break;
        case 4:
            // menu_realizar_transferencia();
            break;
        case 5:
            // menu_ver_transaccion();
            break;
        case 6:
            printf("Gracias por utilizar nuestro servicio.\n");
            break;
        default:
            printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
            break;
        }
    } while (opcion != 6);
}
/* void menu_gestionar_clientes()
{

}
void menu_gestionar_cuentas()
{

}
void menu_gestionar_transacciones()
{

}

*/
