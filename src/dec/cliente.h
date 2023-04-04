#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdbool.h>
#include "../dec/db.h"
#include "../struct.h"

Cliente* iniciar_sesion(char* usuario, char* contrasena);
bool verificar_credenciales(const char *nombreUsuario, const char *contrasena);
void registrar_cliente(Cliente* nuevo_cliente, Usuario* nuevo_usuario);
void actualizar_cliente(int id_cliente, Cliente* datos_actualizados, Usuario* usuario_actualizado);
void eliminar_cliente(int id_cliente);
Cliente* buscar_cliente_por_id(int id_cliente);
void mostrar_info_cliente(Cliente *cliente);
void mostrar_transacciones(Transaccion *transacciones, int num_transacciones);
void mostrar_informe(Informe *informe);
int cliente_tiene_cuenta(int clienteID);
void menu_cliente_sin_cuenta(Cliente *cliente);
void menu_cliente_con_cuenta(Cliente *cliente, Usuario *usuario);

void mostrar_informacion_cliente(Cliente *cliente);
CuentaBancaria *obtener_cuenta_por_clienteID(int clienteID);
void imprimir_informe(Informe *informe);




#endif // CLIENTE_H
