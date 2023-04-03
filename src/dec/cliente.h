#ifndef CLIENTE_H
#define CLIENTE_H

#include "../struct.h"

Cliente* iniciar_sesion(char* usuario, char* contrasena);
void registrar_cliente(Cliente* nuevo_cliente, Usuario* nuevo_usuario);
void actualizar_cliente(int id_cliente, Cliente* datos_actualizados, Usuario* usuario_actualizado);
void eliminar_cliente(int id_cliente);
Cliente* buscar_cliente_por_id(int id_cliente);
void menu_cliente_con_cuenta(Cliente *cli);
void mostrar_info_cliente(Cliente *cliente);


#endif // CLIENTE_H
