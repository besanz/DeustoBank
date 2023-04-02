#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dec/cliente.h"
#include "../dec/db.h"

Cliente* iniciar_sesion(char* usuario, char* contrasena) {
    return db_validar_credenciales(usuario, contrasena);
}

void registrar_cliente(Cliente* nuevo_cliente) {
    db_registrar_cliente(nuevo_cliente);
}

void actualizar_cliente(int id_cliente, Cliente* datos_actualizados) {
    db_actualizar_cliente(id_cliente, datos_actualizados);
}

void eliminar_cliente(int id_cliente) {
    db_eliminar_cliente(id_cliente);
}

Cliente* buscar_cliente_por_id(int id_cliente) {
    return db_buscar_cliente_por_id(id_cliente);
}
