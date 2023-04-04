#ifndef TRANSACCION_H
#define TRANSACCION_H

#include "../struct.h"

void registrar_transaccion(Transaccion* transaccion);
Transaccion* listar_transacciones(const char* numero_cuenta, int *num_transacciones);
void eliminar_transaccion(int id_transaccion);
Informe *mostrar_informe_financiero(const char* numero_cuenta);

#endif // TRANSACCION_H
