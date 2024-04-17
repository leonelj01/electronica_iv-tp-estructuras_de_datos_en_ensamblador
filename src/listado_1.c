#include "serie.h"

#include "ops_memoria.h"

/* En ops_memoria.h
void mueveMemoria(void *destino,const void *origen, size_t tamano);
*/ 

static char origen[] = "Hola Mundo12345";

void listado_1(void){
    const size_t tamano = 4+1+5; // Cantidad de caracteres en "Hola Mundo"
    mueveMemoria(origen+5,origen,tamano);
    Serie_enviaCadena(origen); // Transmite "Hola Hola Mundo"
    Serie_enviaNuevaLinea();
    mueveMemoria(origen,origen+5,tamano);
    Serie_enviaCadena(origen); // Transmite "Hola MundoMundo"
    Serie_enviaNuevaLinea();
}
