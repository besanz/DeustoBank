#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdbool.h>
#include "db.h"
#include "../../struct.h"


/*
 * Inicia Sesion
 * @param usuario: Puntero a una cadena de caracteres que representa el nombre de usuario del cliente.
 * @param contrasena: Puntero a una cadena de caracteres que representa la contrasenya del cliente.
 * @return: Puntero a un objeto Cliente si el inicio de sesion es exitoso, NULL en caso contrario.
*/
Cliente* iniciar_sesion(char* usuario, char* contrasena);

/*
 * Verificar Credenciales
 * @param nombreUsuario: Puntero a una cadena de caracteres que representa el nombre de usuario del cliente.
 * @param contrasena: Puntero a una cadena de caracteres que representa la contraseña del cliente.
 * @return: Valor booleano (true/false) que indica si las credenciales son correctas.
*/
bool verificar_credenciales(const char *nombreUsuario, const char *contrasena);

/*
 * Registrar Cliente
 * @param nuevo_cliente: Puntero a un objeto Cliente que representa los datos del nuevo cliente a registrar.
 * @param nuevo_usuario: Puntero a un objeto Usuario que representa los datos del nuevo usuario a registrar.
*/
void registrar_cliente(Cliente* nuevo_cliente, Usuario* nuevo_usuario);

/*
 * Actualizar Cliente
 * @param id_cliente: Entero que representa el ID del cliente a actualizar.
 * @param datos_actualizados: Puntero a un objeto Cliente que representa los datos actualizados del cliente.
 * @param usuario_actualizado: Puntero a un objeto Usuario que representa los datos actualizados del usuario del cliente.
*/
void actualizar_cliente(int id_cliente, Cliente* datos_actualizados, Usuario* usuario_actualizado);

/*
 * Eliminar Cliente
 * @param id_cliente: Entero que representa el ID del cliente a eliminar.
*/
void eliminar_cliente(int id_cliente);

/*
 * Buscar Cliente por ID
 * @param id_cliente: Entero que representa el ID del cliente a buscar.
 * @return: Puntero a un objeto Cliente si se encuentra el cliente, NULL en caso contrario.
*/
Cliente* buscar_cliente_por_id(int id_cliente);

/*
 * Busca a un cliente por el numero de cuenta bancaria.
 * @param numero_cuenta: Numero de cuenta bancaria del cliente a buscar.
*/
Cliente *buscar_cliente_por_cuenta(const char *numero_cuenta);

/*
 * Mostrar Informacion de Cliente
 * @param cliente: Puntero a un objeto Cliente del cual se desea mostrar la informacion.
*/
void mostrar_informacion_cliente(Cliente *cliente);

/*
 * Mostrar Transacciones
 * @param transacciones: Puntero a un arreglo de objetos Transaccion que representa las transacciones a mostrar.
 * @param num_transacciones: Entero que representa el numero de transacciones en el arreglo.
*/
void mostrar_transacciones(Transaccion *transacciones, int num_transacciones);

/*
 * Mostrar Informe
 * @param informe: Puntero a un objeto Informe que se desea mostrar.
*/
void mostrar_informe(Informe *informe);

/*
 * Cliente Tiene Cuenta
 * @param clienteID: Entero que representa el ID del cliente a verificar si tiene cuenta.
 * @return: Entero que representa el numero de cuentas que tiene el cliente.
*/
int cliente_tiene_cuenta(int clienteID);

/*
 * Menu Cliente sin Cuenta
 * @param cliente: Puntero a un objeto Cliente para el cual se muestra el menu.
 * @param usuario: Puntero a un objeto Usuario del cliente.
*/
void menu_cliente_sin_cuenta(Cliente *cliente, Usuario *usuario);

/*
 * Muestra el menu de opciones para un cliente que ya tiene una cuenta bancaria.
 * @param cliente: Puntero al objeto Cliente del cliente con cuenta.
 * @param usuario: Puntero al objeto Usuario del usuario asociado al cliente.
*/
void menu_cliente_con_cuenta(Cliente *cliente, Usuario *usuario);

/*
 * Muestra la informacion de un cliente.
 * @param cliente: Puntero al objeto Cliente del cliente a mostrar la informacion.
*/
void mostrar_informacion_cliente(Cliente *cliente);
/*
 * Libera la memoria asignada a un objeto Cliente.
 * @param cliente: Puntero al objeto Cliente que se desea liberar de la memoria.
*/
void liberar_cliente(Cliente *cliente);

void volver_al_menu(Cliente* cliente, Usuario* usuario);

#endif // CLIENTE_H
