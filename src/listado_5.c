#include <stdint.h>
#include <stdbool.h>
#include "serie.h"

#include "cola.h"

/* Prototipos en cola.h
void Cola_init(Cola *descriptor, int32_t *memoria,int tamano);

int Cola_pone(Cola *descriptor, int32_t valor);

int Cola_quita(Cola *descriptor, int32_t *destino);
*/

#define TAMANO_COLA 20
static int32_t memoria[TAMANO_COLA];

void listado_5(void){
    Cola cola;
    Cola_init(&cola,memoria,TAMANO_COLA);

    Cola_pone(&cola,1);
    Cola_pone(&cola,2);
    Cola_pone(&cola,3);
    Cola_pone(&cola,4);
    // Envía: "1 2 3 4"
    for(int i=0;i<4;++i){
        int32_t aux;
        Cola_quita(&cola,&aux);
        Serie_enviaEntero(aux);
        Serie_enviaCaracter(' ');
    }
    Serie_enviaNuevaLinea();
}
