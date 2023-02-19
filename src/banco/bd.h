#ifndef BD_H
#define BD_H

#include <string>
#include <sqlite3>

class BD {
public:
    BD();
    ~BD();
    void conectar(std::string nombreArchivo);
    void desconectar();
    void ejecutar(std::string consulta);
    sqlite3_stmt* preparar(std::string consulta);
    void finalizar(sqlite3_stmt* statement);
    int getColumnas(sqlite3_stmt* statement);
    bool hayMasFilas(sqlite3_stmt* statement);
    int avanzar(sqlite3_stmt* statement);
    std::string getColumnaTexto(sqlite3_stmt* statement, int columna);
    int getColumnaEntero(sqlite3_stmt* statement, int columna);
    double getColumnaDouble(sqlite3_stmt* statement, int columna);
private:
    sqlite3* db;
};

#endif