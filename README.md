DeustoBank

Compilacion:

gcc src/main.c src/impl/cliente.c src/impl/cuenta.c src/impl/transaccion.c src/impl/db.c src/impl/usuario.c lib/sqlite3.c -o startbank -Isrc -Isrc/dec -ILib -DSQLITE_THREADSAFE=0

Ejecucion:

startbank.exe

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

./deustobank

Contribuciones
Las contribuciones al proyecto son bienvenidas. Por favor, asegurate de seguir las directrices de contribucion y el codigo de conducta.

Licencia
Este proyecto esta licenciado en la Univ. Deusto