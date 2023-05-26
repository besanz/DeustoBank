**DeustoBank - V2.0**

# Compilación:
```
gcc -c src/c/impl/cliente.c -o obj/cliente.o -Isrc/c/dec -Ilib
gcc -c src/c/impl/cuenta.c -o obj/cuenta.o -Isrc/c/dec -Ilib
gcc -c src/c/impl/transaccion.c -o obj/transaccion.o -Isrc/c/dec -Ilib
gcc -c src/c/impl/db.c -o obj/db.o -Isrc/c/dec -Ilib
gcc -c src/c/impl/usuario.c -o obj/usuario.o -Isrc/c/dec -Ilib
gcc -c lib/sqlite3.c -o obj/sqlite3.o -DSQLITE_THREADSAFE=0

g++ -c src/cpp/impl/socket_client.cpp -o obj/socket_client.o -Isrc/cpp/dec
g++ -c src/cpp/impl/socket_server.cpp -o obj/socket_server.o -Isrc/cpp/dec 
g++ -std=c++11 src/main.cpp obj/socket_client.o obj/socket_server.o obj/cliente.o obj/cuenta.o obj/transaccion.o obj/db.o obj/usuario.o obj/sqlite3.o -o server.exe -lws2_32
g++ -std=c++11 src/cpp/impl/client_main.cpp obj/socket_client.o obj/db.o obj/cliente.o obj/cuenta.o obj/transaccion.o obj/usuario.o obj/sqlite3.o -o client.exe -lws2_32
```

# Servidor:
```
g++ -std=c++11 src/main.cpp obj/socket_client.o obj/socket_server.o obj/cliente.o obj/cuenta.o obj/transaccion.o obj/db.o obj/usuario.o obj/sqlite3.o -o server.exe -lws2_32
```

# Cliente:
```
g++ -std=c++11 src/cpp/impl/client_main.cpp obj/socket_client.o obj/db.o obj/cliente.o obj/cuenta.o obj/transaccion.o obj/usuario.o obj/sqlite3.o -o client.exe -lws2_32
```
# ServidorCpp:
```
g++ src/server.cpp src/cpp/impl/menu.cpp src/cpp/impl/Split.cpp src/c/impl/db.o lib/sqlite3.o src/c/impl/transaccion.o src/c/impl/cliente.o src/c/impl/cuenta.o src/c/impl/usuario.o -o servidor -lws2_32
```
# ClienteCpp:
```
g++ src/client.cpp src/cpp/impl/socket.cpp -o cliente -lws2_32
```

# Ejecución:
```
./server.exe
./client.exe
```

# Usuarios Test (usuario, contrasenya):

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

client.exe y server.exe sobre la cmd, o ./client ./server sobre el powershell. 

Ejecutar sobre el directorio raíz (./)


# Licencia
Este proyecto fue desarrollado siguiendo la metodología de la parte práctica de la asignatura Programacion IV. 

Universidad de Deusto, San Sebastián.
