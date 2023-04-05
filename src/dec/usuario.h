#ifndef USUARIO_H
#define USUARIO_H

#include <time.h>

#include "../struct.h"

/*
 * Crea un nuevo usuario con los datos proporcionados.
 * @param usuarioID: Identificador del usuario.
 * @param nombreUsuario: Nombre de usuario del usuario.
 * @param contrasena: Contrasenya del usuario.
 * @param tipo: Tipo de usuario (por ejemplo, administrador, cliente, etc.).
 * @param datos: Puntero a los datos especificos del tipo de usuario.
 * @return: Puntero al usuario creado.
*/
Usuario *crear_usuario(int usuarioID, char *nombreUsuario, char *contrasena, TipoUsuario tipo, void *datos);

/*
 * Libera la memoria asignada a un usuario.
 * @param usuario: Puntero al usuario a liberar.
*/
void liberar_usuario(Usuario *usuario);

/*
 * Busca un usuario por su identificador.
 * @param usuario_id: Identificador del usuario a buscar.
 * @return: Puntero al usuario encontrado, o NULL si no se encuentra.
*/
Usuario *buscar_usuario_por_id(int usuario_id);

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_pantalla_inicial();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_carga_inicio_sesion();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_inicio_sesion();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_registro_usuario();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_pantalla_registro();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_salida();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_crear_cuenta();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_mostrar_datos_cuenta();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_realizar_deposito();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_realizar_retiro();

/*
 * Usabilidad y experiencia del cliente
*/
void temporizador_realizar_transferencia();

/*
 * Usabilidad y experiencia del cliente
 *@param seconds: segundos a esperar.
*/
void wait(float seconds);

#endif // USUARIO_H
