#include "cliente.h"

// Función para crear un nuevo cliente
Cliente crearCliente(char nombre[50], int edad, char direccion[100], char telefono[20]) {
Cliente cliente;
strcpy(cliente.nombre, nombre);
cliente.edad = edad;
strcpy(cliente.direccion, direccion);
strcpy(cliente.telefono, telefono);
return cliente;
}

// Función para imprimir los datos de un cliente
void imprimirCliente(Cliente cliente) {
printf("Nombre: %s\n", cliente.nombre);
printf("Edad: %d\n", cliente.edad);
printf("Dirección: %s\n", cliente.direccion);
printf("Teléfono: %s\n", cliente.telefono);
}