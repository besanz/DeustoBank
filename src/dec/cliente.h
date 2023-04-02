#ifndef CLIENTE_H
#define CLIENTE_H

#include "../struct.h"

Cliente* iniciar_sesion(char* usuario, char* contrasena);
void registrar_cliente(Cliente* nuevo_cliente);
void actualizar_cliente(int id_cliente, Cliente* datos_actualizados);
void eliminar_cliente(int id_cliente);
Cliente* buscar_cliente_por_id(int id_cliente);


#endif // CLIENTE_H
