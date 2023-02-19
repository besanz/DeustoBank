#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

#include <string>
#include <ctime>

class Movimiento {
public:
    Movimiento(int id, std::string descripcion, float monto, time_t fecha);
    ~Movimiento();
    int getId();
    std::string getDescripcion();
    float getMonto();
    time_t getFecha();
private:
    int id;
    std::string descripcion;
    float monto;
    time_t fecha;
};

#endif