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
