#include <stdint.h>
#include "serie.h"

#include "arreglo.h"

/* Prototipos en arreglo.h
void Arreglo_init(Arreglo *descriptor, void *base,
                  int tamanoElemento, int numElementos);

void *Arreglo_apunta(Arreglo *descriptor, int indice);

int Arreglo_lee(const Arreglo *descriptor, int indice, void *destino);

int Arreglo_escribe(Arreglo *descriptor, int indice, const void *origen);

int Arreglo_intercambia(Arreglo *descriptor, int indice_1, int indice_2);

*/


static char nombres[4][20]={"Valeria","Alejandro","Hernan","Florencia"};
static char auxiliar[20];

void listado_3(void){
    Arreglo descriptor;
    Arreglo_init(&descriptor,nombres,20,4);
    const char *p1;
    Arreglo_lee(&descriptor,3,auxiliar);
    Serie_enviaCadena(auxiliar); // envía "Florencia"
    Serie_enviaNuevaLinea();
    p1 = Arreglo_apunta(&descriptor,2);
    Serie_enviaCadena(p1); // envía "Hernan"
    Serie_enviaNuevaLinea();
    Arreglo_escribe(&descriptor,1,"Victor");
    Arreglo_lee(&descriptor,1,auxiliar);
    Serie_enviaCadena(auxiliar); // envía "Victor"
    Serie_enviaNuevaLinea();
    Arreglo_intercambia(&descriptor,0,1);
    Arreglo_lee(&descriptor,1,auxiliar);
    Serie_enviaCadena(auxiliar); // envía "Valeria"
    Serie_enviaNuevaLinea();
}
