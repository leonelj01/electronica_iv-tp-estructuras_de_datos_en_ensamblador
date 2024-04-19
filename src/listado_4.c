#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "serie.h"

#include "pila.h"

/* Prototipos en pila.h
void Pila_init(Pila *descriptor, int32_t *memoria, size_t tamano);

int Pila_push(Pila *descriptor, int32_t valor);

int Pila_pop(Pila *descriptor, int32_t *destino);
*/

#define TAMANO_PILA 20
static int32_t memoria[TAMANO_PILA];

#define TAMANO_BUFFER 20
static char buffer[TAMANO_BUFFER];


typedef enum {MSG_INSUFICIENTE,MSG_DESBORDE} TipoError;
static const char *const mensajes[]={
    "\r\nERROR: Datos insuficietes en pila!\r\n",
    "\r\nERROR: Desborde de pila\r\n"
};
static void msgError(TipoError err){
    Serie_enviaCadena(mensajes[err]);
}

static void ingresaNumero(Pila *pila,const char *cadena){
    const int32_t A = atol(cadena);
    const int err = Pila_push(pila,A);
    if (err){
        msgError(MSG_DESBORDE);
    }
}

void listado_4(void){
    Pila pila;
    Pila_init(&pila,memoria,TAMANO_PILA);
    Serie_enviaCadena(
        "Calculadora RPN. Ingrese números enteros y operadores"
        "separados por espacio en blanco o nueva línea.Operadores soportados:\r\n"
        "+ suma (a b -- a+b)\r\n"
        "- resta (a b -- a-b)\r\n"
        "* producto (a b -- a*b)\r\n"
        "/ cociente (a b -- a/b)\r\n"
        ". quita de pila y muestra el último resultado (r -- )\r\n"
        "bye finaliza el programa\r\n");
    int continuar = 1;
    while(continuar){
        int32_t A,B,err;
        const int tamano = Serie_recibePalabra(buffer,TAMANO_BUFFER);
        if (tamano == 1){
            switch(buffer[0]){
            case '+':
                err = Pila_pop(&pila,&B);
                err |= Pila_pop(&pila,&A);
                if (err){
                    msgError(MSG_INSUFICIENTE);
                }else{
                    Pila_push(&pila,A+B);
                }
            break; case '-':
                err = Pila_pop(&pila,&B);
                err |= Pila_pop(&pila,&A);
                if (err){
                    msgError(MSG_INSUFICIENTE);
                }else{
                    Pila_push(&pila,A-B);
                }
            break; case '*':
                err = Pila_pop(&pila,&B);
                err |= Pila_pop(&pila,&A);
                if (err){
                    msgError(MSG_INSUFICIENTE);
                }else{
                    Pila_push(&pila,A*B);
                }
            break; case '/':
                err = Pila_pop(&pila,&B);
                err |= Pila_pop(&pila,&A);
                if (err){
                    msgError(MSG_INSUFICIENTE);
                }else{
                    Pila_push(&pila,A/B);
                }
            break; case '.':
                err = Pila_pop(&pila,&A);
                if (err){
                    msgError(MSG_INSUFICIENTE);
                }else{
                    Serie_enviaCadena("Resultado: ");
                    Serie_enviaEntero(A);
                    Serie_enviaNuevaLinea();
                }
            break; default:
                ingresaNumero(&pila,buffer);
            }
        }else{
            if (!strcmp(buffer,"bye")){
                continuar = 0;
            }else{
                ingresaNumero(&pila,buffer);
            }
        }
    }
}
