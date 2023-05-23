#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "../dec/cliente.h"
#include "../dec/usuario.h"
#include "../dec/db.h"
#include "../dec/cuenta.h"
#include "../dec/transaccion.h"
#include "../dec/usuario.h"

int pantalla_inicio()
{
    temporizador_pantalla_inicial();
    int opcion;
    Usuario *usuario;
    Cliente *cliente;

    do
    {
        printf("\nBienvenido a DeustoBank\n");
        printf("\n1. Iniciar sesion\n");
        printf("2. Registrarse\n");
        printf("3. Salir\n");
        printf("\nPor favor, elige una opcion: ");
        scanf("%d", &opcion);


        switch (opcion)
        {
        case 1:
            temporizador_carga_inicio_sesion();
            usuario = inicio_sesion_usuario();

            if (usuario != NULL)
            {
                if (usuario->tipo == 1) //1 = ADMINISTRADOR
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
    return opcion;
}

void registro_usuario()
{
    Cliente nuevo_cliente;
    Usuario nuevo_usuario;

    printf("\n---- Registro de nuevo cliente ----\n");
    printf("\nNombre de usuario: ");
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
    nuevo_cliente.usuarioID = db_obtener_usuarioID(nuevo_usuario.nombreUsuario);
    temporizador_registro_usuario();
    // Pase el nuevo_usuario como argumento adicional
    db_registrar_cliente(&nuevo_cliente, &nuevo_usuario);
    printf("Cliente registrado exitosamente.\n");
}

Usuario *inicio_sesion_usuario()
{

    char nombreUsuario[50], contrasena[50];
    printf("\n---- Inicio de Sesion ----\n");
    printf("\nNombre de usuario: ");
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
        printf("\n1. Gestionar clientes\n");
        printf("2. Gestionar cuentas\n");
        printf("3. Gestionar transferencias\n");
        printf("4. Salir\n");
        printf("\nPor favor, elija una opcion: ");

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


Usuario *crear_usuario(int usuarioID, char *nombreUsuario, char *contrasena, TipoUsuario tipo, void *datos)
{
    Usuario *nuevo_usuario = (Usuario *)malloc(sizeof(Usuario));
    if (!nuevo_usuario)
    {
        return NULL;
    }

    nuevo_usuario->usuarioID = usuarioID;
    strcpy(nuevo_usuario->nombreUsuario, nombreUsuario);
    strcpy(nuevo_usuario->contrasena, contrasena);
    nuevo_usuario->tipo = tipo;
    nuevo_usuario->datos = datos;

    return nuevo_usuario;
}

Usuario *buscar_usuario_por_id(int usuario_id)
{
    sqlite3 *db;
    Usuario *usuario = NULL;

    if (abrir_db(&db) != SQLITE_OK)
    {
        return NULL;
    }

    const char *sql = "SELECT usuarioID, nombreUsuario, contrasena, tipo "
                      "FROM usuarios "
                      "WHERE usuarioID = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, usuario_id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            usuario = (Usuario *)malloc(sizeof(Usuario));
            usuario->usuarioID = sqlite3_column_int(stmt, 0);
            strncpy(usuario->nombreUsuario, (const char *)sqlite3_column_text(stmt, 1), sizeof(usuario->nombreUsuario));
            strncpy(usuario->contrasena, (const char *)sqlite3_column_text(stmt, 2), sizeof(usuario->contrasena));
            usuario->tipo = sqlite3_column_int(stmt, 3);
            usuario->datos = NULL;
        }
    }

    sqlite3_finalize(stmt);
    cerrar_db(db);
    return usuario;
}

void liberar_usuario(Usuario *usuario)
{
    if (usuario)
    {
        free(usuario);
    }
}

void temporizador_pantalla_inicial()
{
    system("cls");
    printf("Conectando con la base de datos.");
    wait(0.4);
    system("cls");
    printf("Conectando con la base de datos..");
    wait(0.4);
    system("cls");
    printf("Conectando con la base de datos...");
    wait(0.4);
    system("cls");
    printf("Inicializando DeustoBank.");
    wait(0.4);
    system("cls");
    printf("Inicializando DeustoBank..");
    wait(0.4);
    system("cls");
    printf("Inicializando DeustoBank...");
    wait(0.4);
    system("cls");
}

void temporizador_carga_inicio_sesion()
{
    system("cls");
    printf("Cargando.");
    wait(0.4);
    system("cls");
    printf("Cargando..");
    wait(0.4);
    system("cls");
    printf("Cargando...");
    wait(0.4);
    system("cls");
    printf("Cargando.");
    wait(0.4);
    system("cls");
    printf("Cargando..");
    wait(0.4);
    system("cls");
    printf("Cargando...");
    wait(0.4);
    system("cls");
}

void temporizador_pantalla_registro()
{
    system("cls");
    printf("Entrando en registro de usuarios.");
    wait(0.4);
    system("cls");
    printf("Entrando en registro de usuarios..");
    wait(0.4);
    system("cls");
    printf("Entrando en registro de usuarios...");
    wait(0.4);
    system("cls");
    printf("Entrando en registro de usuarios.");
    wait(0.4);
    system("cls");
    printf("Entrando en registro de usuarios..");
    wait(0.4);
    system("cls");
    printf("Entrando en registro de usuarios...");
    wait(0.4);
    system("cls");
}

void temporizador_registro_usuario()
{
    system("cls");
    printf("Registrando usuario.");
    wait(0.4);
    system("cls");
    printf("Registrando usuario..");
    wait(0.4);
    system("cls");
    printf("Registrando usuario...");
    wait(0.4);
    system("cls");
    printf("Bienvenido.");
    wait(0.4);
    system("cls");
    printf("Bienvenido..");
    wait(0.4);
    system("cls");
    printf("Bienvenido...");
    wait(0.4);
    system("cls");
}
void temporizador_inicio_sesion()
{
    system("cls");
    printf("Verificando.");
    wait(0.4);
    system("cls");
    printf("Verificando..");
    wait(0.4);
    system("cls");
    printf("Verificando...");
    wait(0.4);
    system("cls");
    system("cls");
    printf("Un momento por favor.");
    wait(0.4);
    system("cls");
    printf("Un momento por favor.");
    wait(0.4);
    system("cls");
    printf("Un momento por favor...");
    wait(0.4);
    system("cls");
}
void temporizador_menu_con_cuenta()
{
    system("cls");
    printf("Volviendo al menu.");
    wait(0.4);
    system("cls");
    printf("Volviendo al menu..");
    wait(0.4);
    system("cls");
    printf("Volviendo al menu...");
    wait(0.4);
    system("cls");
}
void temporizador_salida()
{
    system("cls");
    printf("Cerrando Sesion.");
    wait(0.4);
    system("cls");
    printf("Cerrando Sesion..");
    wait(0.4);
    system("cls");
    printf("Cerrando Sesion...");
    wait(0.4);
    system("cls");
    printf("Finalizando programa.");
    wait(0.4);
    system("cls");
    printf("Finalizando programa..");
    wait(0.4);
    system("cls");
    printf("Finalizando programa...");
    wait(0.4);
    system("cls");
    printf("Gracias por utilizar DeustoBank, vuelva pronto!");
    wait(1);
    system("cls");
}
void temporizador_crear_cuenta()
{
    system("cls");
    printf("Creando cuenta.");
    wait(0.4);
    system("cls");
    printf("Creando cuenta..");
    wait(0.4);
    system("cls");
    printf("Creando cuenta...");
    wait(0.4);
    system("cls");
    printf("Conectando con la base de datos.");
    wait(0.4);
    system("cls");
    printf("Conectando con la base de datos..");
    wait(0.4);
    system("cls");
    printf("Conectando con la base de datos...");
    wait(0.4);
    system("cls");
    printf("Conectando con la base de datos");
    wait(0.4);
    system("cls");
}
void temporizador_mostrar_datos_cuenta()
{
    system("cls");
    printf("Descargando datos de la nube.");
    wait(0.4);
    system("cls");
    printf("Descargando datos de la nube..");
    wait(0.4);
    system("cls");
    printf("Descargando datos de la nube...");
    wait(0.4);
    system("cls");
}
void temporizador_realizar_deposito()
{
    system("cls");
    printf("Espere un momento por favor.");
    wait(0.4);
    system("cls");
    printf("Espere un momento por favor..");
    wait(0.4);
    system("cls");
    printf("Espere un momento por favor...");
    wait(0.4);
    system("cls");
    printf("Operacion realizada.");
    wait(1);
    system("cls");
}
void temporizador_realizar_retiro()
{
    system("cls");
    printf("Espere un momento por favor.");
    wait(0.4);
    system("cls");
    printf("Espere un momento por favor..");
    wait(0.4);
    system("cls");
    printf("Espere un momento por favor...");
    wait(0.4);
    system("cls");
    printf("Gracias por confiar en DeustoBank.");
    wait(0.6);
    system("cls");
}
void temporizador_realizar_transferencia()
{
    system("cls");
    printf("Realizando transferencia.");
    wait(0.4);
    system("cls");
    printf("Realizando transferencia..");
    wait(0.4);
    system("cls");
    printf("Realizando transferencia...");
    wait(0.4);
    system("cls");
    printf("Espere un momento por favor.");
    wait(0.4);
    system("cls");
    printf("Espere un momento por favor..");
    wait(0.4);
    system("cls");
    printf("Espere un momento por favor...");
    wait(0.4);
    system("cls");
}
void wait(float seconds) {
    clock_t end_wait;
    end_wait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < end_wait) {}
}