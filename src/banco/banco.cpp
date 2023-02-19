#include "banco.h"
#include "bd.h"
#include "cuenta.h"
#include "movimiento.h"

Banco::Banco() {
    // Constructor, inicializa el objeto
}

Banco::~Banco() {
    // Destructor, libera la memoria utilizada por el objeto
}

void Banco::agregarCuenta(Cuenta nuevaCuenta) {
    // Agrega una cuenta a la base de datos
    // Utiliza la clase BD para realizar la operación de inserción
    BD::insertarCuenta(nuevaCuenta);
}

void Banco::realizarDeposito(int idCuenta, float monto) {
    // Realiza un depósito en la cuenta especificada por idCuenta
    // Utiliza la clase Cuenta para actualizar el saldo de la cuenta
    // Utiliza la clase Movimiento para agregar un nuevo registro de movimiento
    Cuenta cuenta = BD::buscarCuentaPorId(idCuenta);
    cuenta.depositar(monto);
    BD::actualizarCuenta(cuenta);
    Movimiento nuevoMovimiento(idCuenta, "Depósito", monto);
    BD::insertarMovimiento(nuevoMovimiento);
}

void Banco::realizarRetiro(int idCuenta, float monto) {
    // Realiza un retiro de la cuenta especificada por idCuenta
    // Utiliza la clase Cuenta para actualizar el saldo de la cuenta
    // Utiliza la clase Movimiento para agregar un nuevo registro de movimiento
    Cuenta cuenta = BD::buscarCuentaPorId(idCuenta);
    cuenta.retirar(monto);
    BD::actualizarCuenta(cuenta);
    Movimiento nuevoMovimiento(idCuenta, "Retiro", monto);
    BD::insertarMovimiento(nuevoMovimiento);
}

// ... otras funciones necesarias para el proyecto ...
