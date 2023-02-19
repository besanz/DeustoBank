#ifndef BANCO_H
#define BANCO_H

#include "cuenta.h"
#include <vector>

class Banco {
public:
    Banco();
    ~Banco();
    void agregarCuenta(Cuenta nuevaCuenta);
    void realizarDeposito(int idCuenta, float monto);
    void realizarRetiro(int idCuenta, float monto);
    // ... Agrega aquí otras funciones necesarias para el proyecto ...
private:
    std::vector<Cuenta> cuentas;
};

#endif
