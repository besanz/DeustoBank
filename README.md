DeustoBank

DeustoBank es un sistema de banca por linea de comandos que permite a los usuarios realizar transacciones bancarias de manera facil, rapida y segura. El proyecto esta desarrollado en C y C++ y utiliza SQLite3 para el almacenamiento de datos.

Caracteristicas

Creacion de cuentas bancarias
Consulta de informacion de cuentas bancarias
Depositos y retiros de dinero
Transferencias entre cuentas bancarias
Eliminacion de cuentas bancarias
Gestion de clientes y administradores

Requisitos
Para ejecutar DeustoBank, necesitaras lo siguiente:

Compilador de C y C++ (por ejemplo, GCC)

SQLite3

Estructura del proyecto

DeustoBank/
├── src/
│ ├── main.c
│ ├── cliente.c
│ ├── cuenta.c
│ ├── transaccion.c
│ ├── cliente.h
│ ├── cuenta.h
│ ├── transaccion.h
├── db/
│ ├── db.cpp
│ ├── db.h
├── network/
│ ├── connect.cpp
│ ├── connect.h
│ ├── getData.cpp
│ ├── getData.h
│ ├── postData.cpp
│ ├── postData.h
└── struct.h

Compilacion y ejecucion
Para compilar y ejecutar DeustoBank, sigue estos pasos:

Abre una terminal y navega hasta la carpeta del proyecto.
Compila el proyecto con el siguiente comando:
css
Copy code
gcc src/main.c src/cliente.c src/cuenta.c src/transaccion.c -o deustobank -lsqlite3
Ejecuta el programa compilado con el siguiente comando:
bash

./deustobank

Contribuciones
Las contribuciones al proyecto son bienvenidas. Por favor, asegurate de seguir las directrices de contribucion y el codigo de conducta.

Licencia
Este proyecto esta licenciado en la Univ. Deusto