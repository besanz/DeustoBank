#include "cpp/dec/socket.h"
#include <iostream>
#include <cstdlib>

using namespace sockets;
using namespace std;

int main() {
    
    Socket *s = new Socket();

    
    char* opcion = new char[50];
    
    do{
        
        cin>>(char*)opcion;
        cin.ignore();
        s->sendRequest((char*)opcion);
        s->printBuffer();
        delete[] opcion;
        opcion = new char[50];
        
    } while( opcion[0] != 'q');
    delete s;
    return 0;
}