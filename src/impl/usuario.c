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
    // system("cls");
    // printf("Descargando actualizaciones.");
    // wait(0.7);
    // system("cls");
    // printf("Descargando actualizaciones..");
    // wait(0.7);
    // system("cls");
    // printf("Descargando actualizaciones...");
    // wait(0.7);
    // system("cls");
    // printf("Inicializando  DeustoBank.");
    // wait(0.7);
    // system("cls");
    // printf("Inicializando  DeustoBank..");
    // wait(0.7);
    // system("cls");
    // printf("Inicializando  DeustoBank...");
    // wait(0.7);
    // system("cls");
}

void temporizador_carga_inicio_sesion()
{
    // system("cls");
    // printf("Cargando inicio de sesion.");
    // wait(0.7);
    // system("cls");
    // printf("Cargando inicio de sesion..");
    // wait(0.7);
    // system("cls");
    // printf("Cargando inicio de sesion...");
    // wait(0.7);
    // system("cls");
    // printf("Cargando inicio de sesion.");
    // wait(0.7);
    // system("cls");
    // printf("Cargando inicio de sesion..");
    // wait(0.7);
    // system("cls");
    // printf("Cargando inicio de sesion...");
    // wait(0.7);
    // system("cls");
}

void temporizador_pantalla_registro()
{
    // system("cls");
    // printf("Entrando en registro de usuarios.");
    // wait(0.7);
    // system("cls");
    // printf("Entrando en registro de usuarios..");
    // wait(0.7);
    // system("cls");
    // printf("Entrando en registro de usuarios...");
    // wait(0.7);
    // system("cls");
    // printf("Entrando en registro de usuarios.");
    // wait(0.7);
    // system("cls");
    // printf("Entrando en registro de usuarios..");
    // wait(0.7);
    // system("cls");
    // printf("Entrando en registro de usuarios...");
    // wait(0.7);
    // system("cls");
}

void temporizador_registro_usuario()
{
    // system("cls");
    // printf("Registrando usuario.");
    // wait(0.7);
    // system("cls");
    // printf("Registrando usuario..");
    // wait(0.7);
    // system("cls");
    // printf("Registrando usuario...");
    // wait(0.7);
    // system("cls");
    // printf("Registrando usuario.");
    // wait(0.7);
    // system("cls");
    // printf("Registrando usuario..");
    // wait(0.7);
    // system("cls");
    // printf("Registrando usuario...");
    // wait(0.7);
    // system("cls");
}
void temporizador_inicio_sesion()
{
//     system("cls");
//     printf("Iniciando sesion.");
//     wait(0.7);
//     system("cls");
//     printf("Iniciando sesion..");
//     wait(0.7);
//     system("cls");
//     printf("Iniciando sesion...");
//     wait(0.7);
//     system("cls");
}
void temporizador_salida()
{
//     system("cls");
//     printf("Cerrando sesion.");
//     wait(0.7);
//     system("cls");
//     printf("Cerrando sesion..");
//     wait(0.7);
//     system("cls");
//     printf("Cerrando sesion...");
//     wait(0.7);
//     system("cls");
//     printf("Saliendo.");
//     wait(0.7);
//     system("cls");
//     printf("Saliendo..");
//     wait(0.7);
//     system("cls");
//     printf("Saliendo...");
//     wait(0.7);
//     system("cls");
//     printf("Gracias por utilizar DeustoBank, vuelva pronto");
//     wait(0.7);
//     system("cls");
}
void temporizador_crear_cuenta()
{
//     system("cls");
//     printf("Creando cuenta.");
//     wait(0.7);
//     system("cls");
//     printf("Creando cuenta..");
//     wait(0.7);
//     system("cls");
//     printf("Creando cuenta...");
//     wait(0.7);
//     system("cls");
//     printf("Validando datos.");
//     wait(0.7);
//     system("cls");
//     printf("Validando datos..");
//     wait(0.7);
//     system("cls");
//     printf("Validando datos...");
//     wait(0.7);
//     system("cls");
//     printf("Cuenta creada con exito");
//     wait(0.7);
//     system("cls");
}
void temporizador_mostrar_datos_cuenta()
{
    // system("cls");
    // printf("Descargando datos del servidor.");
    // wait(0.7);
    // system("cls");
    // printf("Descargando datos del servidor..");
    // wait(0.7);
    // system("cls");
    // printf("Descargando datos del servidor...");
    // wait(0.7);
    // system("cls");
}
void temporizador_realizar_deposito()
{
//     system("cls");
//     printf("Actualizando saldo.");
//     wait(0.7);
//     system("cls");
//     printf("Actualizando saldo..");
//     wait(0.7);
//     system("cls");
//     printf("Actualizando saldo...");
//     wait(0.7);
//     system("cls");
}
void temporizador_realizar_retiro()
{
//     system("cls");
//     printf("Actualizando saldo.");
//     wait(0.7);
//     system("cls");
//     printf("Actualizando saldo..");
//     wait(0.7);
//     system("cls");
//     printf("Actualizando saldo...");
//     wait(0.7);
//     system("cls");
//     printf("Esta operacion de retirada de efectivo es gratis para usted");
//     wait(2.0);
//     system("cls");
}
void temporizador_realizar_transferencia()
{
//     system("cls");
//     printf("Realizando transferencia.");
//     wait(0.7);
//     system("cls");
//     printf("Realizando transferencia..");
//     wait(0.7);
//     system("cls");
//     printf("Realizando transferencia...");
//     wait(0.7);
//     system("cls");
//     printf("Actualizando saldo.");
//     wait(0.7);
//     system("cls");
//     printf("Actualizando saldo..");
//     wait(0.7);
//     system("cls");
//     printf("Actualizando saldo...");
//     wait(0.7);
//     system("cls");
}
void wait(float seconds) {
    // clock_t end_wait;
    // end_wait = clock() + seconds * CLOCKS_PER_SEC;
    // while (clock() < end_wait) {}
}