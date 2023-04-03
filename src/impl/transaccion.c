#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../dec/transaccion.h"


TransaccionNodo* transacciones = NULL;

TransaccionNodo* crear_nodo_transaccion(Transaccion* transaccion) {
    TransaccionNodo* nodo = (TransaccionNodo*) malloc(sizeof(TransaccionNodo));
    nodo->transaccion = *transaccion;
    nodo->siguiente = NULL;
    return nodo;
}

void registrar_transaccion(Transaccion* transaccion) {
    TransaccionNodo* nodo = crear_nodo_transaccion(transaccion);
    nodo->siguiente = transacciones;
    transacciones = nodo;
}

Transaccion* listar_transacciones(int numero_cuenta) {
    TransaccionNodo* nodo_actual = transacciones;
    int contador = 0;

    while (nodo_actual != NULL) {
        if (nodo_actual->transaccion.numeroCuentaOrigen == numero_cuenta ||
            nodo_actual->transaccion.numeroCuentaDestino == numero_cuenta) {
            contador++;
        }
        nodo_actual = nodo_actual->siguiente;
    }

    if (contador == 0) {
        return NULL;
    }

    Transaccion* transacciones_cuenta = (Transaccion*) malloc(sizeof(Transaccion) * contador);
    nodo_actual = transacciones;
    int i = 0;

    while (nodo_actual != NULL) {
        if (nodo_actual->transaccion.numeroCuentaOrigen == numero_cuenta ||
            nodo_actual->transaccion.numeroCuentaDestino == numero_cuenta) {
            transacciones_cuenta[i++] = nodo_actual->transaccion;
        }
        nodo_actual = nodo_actual->siguiente;
    }

    return transacciones_cuenta;
}

void eliminar_transaccion(int id_transaccion) {
    TransaccionNodo* nodo_actual = transacciones;
    TransaccionNodo* nodo_anterior = NULL;

    while (nodo_actual != NULL) {
        if (nodo_actual->transaccion.transaccionID == id_transaccion) {
            if (nodo_anterior == NULL) {
                transacciones = nodo_actual->siguiente;
            } else {
                nodo_anterior->siguiente = nodo_actual->siguiente;
            }
            free(nodo_actual);
            return;
        }
        nodo_anterior = nodo_actual;
        nodo_actual = nodo_actual->siguiente;
    }
}

Informe* mostrar_informe_financiero(int numero_cuenta) {
    Informe* informe = (Informe*) malloc(sizeof(Informe));

    // Inicializar el informe
    informe->numeroCuenta = numero_cuenta;
    informe->saldoInicial = 0;
    informe->saldoFinal = 0;
    informe->numDepositos = 0;
    informe->totalDepositos = 0;
    informe->numRetiros = 0;
    informe->totalRetiros = 0;
    informe->numTransferenciasEnviadas = 0;
    informe->totalTransferenciasEnviadas = 0;
    informe->numTransferenciasRecibidas = 0;
    informe->totalTransferenciasRecibidas = 0;

    TransaccionNodo* nodo_actual = transacciones;

    while (nodo_actual != NULL) {
        Transaccion* transaccion = &(nodo_actual->transaccion);

        if (transaccion->numeroCuentaOrigen == numero_cuenta) {
            informe->numTransferenciasEnviadas++;
            informe->totalTransferenciasEnviadas += transaccion->importe;
            informe->saldoFinal -= transaccion->importe;
        }

        if (transaccion->numeroCuentaDestino == numero_cuenta) {
            informe->numTransferenciasRecibidas++;
            informe->totalTransferenciasRecibidas += transaccion->importe;
            informe->saldoFinal += transaccion->importe;
        }

        nodo_actual = nodo_actual->siguiente;
    }

    informe->saldoInicial =  informe->saldoFinal - (informe->totalDepositos - informe->totalRetiros + informe->totalTransferenciasRecibidas - informe->totalTransferenciasEnviadas);

    return informe;
}