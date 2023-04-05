**DeustoBank - V1**

# Compilación:

```
gcc src/main.c src/impl/cliente.c src/impl/cuenta.c src/impl/transaccion.c src/impl/db.c src/impl/usuario.c lib/sqlite3.c -o startbank -Isrc -Isrc/dec -ILib -DSQLITE_THREADSAFE=0
```

# Ejecución:
```
startbank.exe
```
# Usuarios Test (usuario, contraseña):

- jose, jose
- besanz, besanz
- jokinson, jokinson
- aritz, aritz


# Características

- Creación de cuentas bancarias para clientes
- Consulta de información de cuentas bancarias
- Depósitos y retiros de dinero
- Transferencias entre cuentas bancarias
- Eliminación de cuentas bancarias
- Gestión de clientes y administradores

# Algunas de las opciones:

1. Mostrar información del cliente
2. Mostrar información de la cuenta
3. Realizar ingresos de dinero
4. Realizar retiradas de dinero
5. Enviar dinero a amigos o conocidos (por su usuario)
6. Mostrar una lista de transacciones
7. Imprimir un informe financiero en formato .txt
8. Cerrar la cuenta de forma permanente

# Requisitos
Para ejecutar DeustoBank, necesitarás lo siguiente:

- Compilador de C y C++ (nosotros hemos usado GCC)
- SQLite3 (lib)

Toda la información se almacena en deustobank.db

# Uso:

./deustobank


# Licencia
Este proyecto fue desarrollado siguiendo la metodología de la parte práctica de la asignatura Programacion IV. 

Universidad de Deusto, San Sebastián.
