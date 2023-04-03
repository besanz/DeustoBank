#include <stdlib.h>
#include <string.h>

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

void liberar_usuario(Usuario *usuario)
{
    if (usuario)
    {
        free(usuario);
    }
}
