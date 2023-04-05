#ifndef TRANSACCION_H
#define TRANSACCION_H

#include "../struct.h"

/*
 * Registra una transaccion en el sistema.
 * @param transaccion: Puntero al objeto Transaccion que se desea registrar.
*/
void registrar_transaccion(Transaccion *transaccion);

/*
 * Lista todas las transacciones asociadas a una cuenta especifica.
 * @param numero_cuenta: Numero de cuenta de la cual se desea listar las transacciones.
 * @param num_transacciones: Puntero a entero que almacenara la cantidad de transacciones encontradas.
 * @return: Puntero a un arreglo de objetos Transaccion que contiene las transacciones encontradas.
*/
Transaccion *listar_transacciones(const char *numero_cuenta, int *num_transacciones);

/*
 * Elimina una transaccion del sistema.
 * @param id_transaccion: Identificador unico de la transaccion que se desea eliminar.
*/
void eliminar_transaccion(int id_transaccion);
/*
 * Genera un informe financiero para una cuenta especifica.
 * @param numero_cuenta: Numero de cuenta de la cual se desea generar el informe.
 * @return: Puntero a un objeto Informe que contiene la informacion financiera de la cuenta.
*/
Informe *mostrar_informe_financiero(const char *numero_cuenta);

/*
 * Realiza una transferencia de fondos entre dos cuentas.
 * @param cliente_id_origen: Identificador unico del cliente que realiza la transferencia.
*/
void realizar_transferencia(int cliente_id_origen);

#endif // TRANSACCION_H
