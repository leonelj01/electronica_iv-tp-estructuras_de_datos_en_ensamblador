#include "listados.h"
#include <ctype.h>
#include "serie.h"

static int valor(const char *texto){
    int v=0;
    int negativo=0;
    for (const char *p=texto;*p;p++){
        const char c = *p;
        if (p==texto && c == '-'){
            negativo = 1;
        }else if(isdigit(c)){
            v = 10 * v + c - '0';
        }else{
            break;
        }
    }
    return negativo? -v : v;
}

int main(void){
    char nl[3];
    Serie_init(9600);
    for(;;){
        Serie_enviaNuevaLinea();
        Serie_enviaCadena("Ingrese el número de listado [1 a 5] y "
                          "luego espacio para continuar ");
        Serie_recibePalabra(nl,3);
        Serie_enviaNuevaLinea();

        switch(valor(nl)){
        case 1:
            listado_1();
        break; case 2:
            listado_2();
        break; case 3:
            listado_3();
        break; case 4:
            listado_4();
        break; case 5:
            listado_5();
        break; default:
            Serie_enviaCadena("Listado no reconocido\r\n");
        break;
        }

        Serie_enviaNuevaLinea();
    }
}