#ifndef CUENTA_H
#define CUENTA_H

#include <string>

class Cuenta {
public:
    Cuenta(int id, std::string nombre, float saldo);
    ~Cuenta();
    int getId();
    std::string getNombre();
    float getSaldo();
    void depositar(float monto);
    void retirar(float monto);
private:
    int id;
    std::string nombre;
    float saldo;
};

#endif
