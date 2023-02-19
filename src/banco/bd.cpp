#include "bd.h"

BD::BD() {
    db = NULL;
}

BD::~BD() {
    desconectar();
}

void BD::conectar(std::string nombreArchivo) {
    int rc = sqlite3_open(nombreArchivo.c_str(), &db);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("No se pudo abrir la base de datos");
    }
}

void BD::desconectar() {
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}

void BD::ejecutar(std::string consulta) {
    char* error;
    int rc = sqlite3_exec(db, consulta.c_str(), NULL, NULL, &error);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Error al ejecutar consulta: " + std::string(error));
        sqlite3_free(error);
    }
}

sqlite3_stmt* BD::preparar(std::string consulta) {
    sqlite3_stmt* statement;
    int rc = sqlite3_prepare_v2(db, consulta.c_str(), -1, &statement, NULL);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Error al preparar consulta");
    }
    return statement;
}

void BD::finalizar(sqlite3_stmt* statement) {
    sqlite3_finalize(statement);
}

int BD::getColumnas(sqlite3_stmt* statement) {
    return sqlite3_column_count(statement);
}

bool BD::hayMasFilas(sqlite3_stmt* statement) {
    int rc = sqlite3_step(statement);
    if (rc == SQLITE_ROW) {
        return true;
    } else if (rc == SQLITE_DONE) {
        return false;
    } else {
        throw std::runtime_error("Error al consultar filas");
    }
}

int BD::avanzar(sqlite3_stmt* statement) {
    return sqlite3_step(statement);
}

std::string BD::getColumnaTexto(sqlite3_stmt* statement, int columna) {
    const unsigned char* texto = sqlite3_column_text(statement, columna);
    if (texto == NULL) {
        return "";
    } else {
        return std::string((const char*)texto);
    }
}

int BD::getColumnaEntero(sqlite3_stmt* statement, int columna) {
    return sqlite3_column_int(statement, columna);
}

double BD::getColumnaDouble(sqlite3_stmt* statement, int columna) {
    return sqlite3_column_double(statement, columna);
}
