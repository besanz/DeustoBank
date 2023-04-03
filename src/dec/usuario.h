#ifndef USUARIO_H
#define USUARIO_H

#include "../struct.h"

Usuario *crear_usuario(int usuarioID, char *nombreUsuario, char *contrasena, TipoUsuario tipo, void *datos);
void liberar_usuario(Usuario *usuario);

#endif // USUARIO_H
