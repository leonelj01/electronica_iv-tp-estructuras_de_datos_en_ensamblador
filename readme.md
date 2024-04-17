# Electrónica IV - TP - Programación en ensamblador de un procesador ARMv7M

Este trabajo práctico debe realizarse en modalidad *individual*.

*Plazo*: **1 Semana**.

## Objetivos

Si completas este trabajo práctico con éxito lograrás:

1. Resolver mediante algoritmos problemas de computación
2. Entender y explicar el proceso que sigues al resolver cada problema, las
   fuentes que consultas y las dificultades que encuentras en el camino.
3. Implementar operaciones básicas para trabajar con con las estructuras de
   datos: arreglo, pila y cola.
4. Escribir programas en lenguaje ensamblador para la arquitectura ARMv7M que
   implementan los algoritmos encontrados o desarrollados.
5. Utilizar herramientas de desarrollo para compilar, cargar y ejecutar
   programas de prueba que ejercitan tus algoritmos y verifican sus respuestas
   en casos particulares.

## Metodología de trabajo

Este práctico debe resolverse siguiendo una disciplina de desarrollo estricta
para lograr capturar y documentar todos los detalles del proceso. No basta con
arribar a programas en ensamblador implementando algoritmos que resuelvan cada
problema sino que es fundamental documentar todo el proceso de desarrollo para
que tu respuesta sea válida.

Debes completar la memoria técnica en `memoria_tecnica.md` *mientras* realizas
tu trabajo, es decir que la memoria técnica es a la vez tu memoria de trabajo y
un registro de tu proceso para posterior análisis y reflexión. En la plantilla
de la memoria técnica se incluyen instrucciones detalladas de como realizar y
documentar tu desarrollo.

Para cada problema escribirás un archivo fuente en lenguaje ensamblador con tu
solución, que será compilado e integrado a un programa de pruebas unitarias que,
ejecutado en un simulador o en hardware compatibla, permitirá verificar el
correcto desempeño de tu solución.

## Problemas

En las distintas secciones de este capítulo se listan los problemas de cómputo
que debes resolver. Implementarás cada solución como una subrutina utilizable
como función de C, osea que respete el estándar de llamado a procedimiento de
la Interfaz Binaria de Aplicación (ABI) definida por ARM (ARM, 2023).

### Mueve memoria

Hay dos regiones contiguas de memoria, posiblemente solapadas, de igual cantidad
de bytes. Estas regiones las llamaremos *origen* y *destino*. Debes copiar los
valores de *origen* en *destino*. Si las regiones se solapan, *destino* debe
tener al terminar una copia íntegra de los datos (en ese caso *origen* ya no
la tendrá, por eso es mover y no copiar). Tu solución será en la forma de la
función `mueveMemoria` cuyo uso se ejemplifica en el Listado 1. Ubica tu
solución en `lib/ops_memoria/mueve.S`.

Listado 1: Prototipo de la funcion `mueveMemoria`  

```c
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
```

**Nota 1**: El prototipo de una función en C define los argumentos que acepta y
el valor que retorna.

**Nota 2**: Para entender el Listado 1 debes tener un entendimiento de punteros
y arreglos en lenguaje C. Como ayuda: `origen` es un arreglo en memoria de
dieciseis bytes interpretados como caracteres. Los primeros 15 bytes contienen
los caracteres 'H','o',...,'5' y el byte 16 tiene el valor cero (ascii `nul`).
En una expresión, el valor de `origen` es la posición de memoria del primer
caracter del arreglo (con tipo puntero a caracter), mientras que el valor de
`origen+5` corresponde a la posición en memoria del sexto caracter (inicialmente
está sería la 'M' de Mundo).

### Compara regiones de memoria

Dadas dos regiones de memoria de igual tamaño, que llamaremos *izquierda* y
*derecha*, compara sus contenidos byte a byte como valores *sin signo* y retorna
un valor entero. El valor retornado será $0$ si los contenidos de las regiones
son *iguales*, $-1$ si el primer byte distinto es *menor* en *izquierda* que en
*derecha* y $1$ si el primer byte distinto es *mayor* en *izquierda* que en
*derecha*. Tu solución será en la forma de la función `comparaMemoria` cuyo uso
se ejemplifica en el Listado 2. Ubica tu solución en `lib/ops_memoria/compara.S`

Listado 2:

```c
#include <stdint.h>
#include "serie.h"

#include "ops_memoria.h"

/* Prototipo en ops_memoria.h
int comparaMemoria(const void *izquierda,const void *derecha, size_t tamano);
*/

uint8_t a1[]={0,1,2,3};
uint8_t a2[]={0,1,3,5};
uint8_t a3[]={0,1,2,3};
size_t tamano = 4;

void listado_2(void){
    const int comp1 = comparaMemoria(a1,a3,tamano);
    const int comp2 = comparaMemoria(a1,a2,tamano);
    const int comp3 = comparaMemoria(a2,a3,tamano);
    Serie_enviaEntero(comp1); // envia "0"
    Serie_enviaNuevaLinea();
    Serie_enviaEntero(comp2); // envia "-1"
    Serie_enviaNuevaLinea();
    Serie_enviaEntero(comp3); // envia "1"
    Serie_enviaNuevaLinea();
}
```

### Arreglo general descrito por metadatos

En este problema nos ocuparemos de representar un arreglo mediante un tipo de
datos estructurado o registro (struct de lenguaje C) que contiene la información
necesaria para operar sobre el mismo. El elemento que describe al arreglo se
llamará *descriptor*. Definiremos funciones que implementan una interfaz
uniforme de acceso al arreglo en base a un descriptor. Dicha interfaz contiene
las siguientes operaciones:

- Inicializar un descriptor de arreglo
- Obtener un puntero a un elemento del arreglo
- Leer un elemento del arreglo
- Escribir un elemento del arreglo
- Intercambiar los valores de dos elementos del arreglo

En el Listado 3 verás un sencillo ejemplo de uso. Tu implementación irá en
`lib/arreglo/arreglo.S`

Listado 3. Ejemplo de uso de funciones de manipulación de arreglo de propósito
general

```c
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


static char nombres[20][4]={"Valeria","Alejandro","Hernan","Florencia"};
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
```

***NOTA 1:*** Los struct de C son representados en memoria como una concatenación
de los datos que los componen en el orden en que fueron declarados. El primer
miembro estará en la dirección base de la estructura. Cada miembro adicional se
ubicará en la primer posición de memoria luego del miembro anterior que
satisface los requisitos de alineación de su tipo (ver NOTA 2).

***NOTA 2:*** Los datos se almacenan en memoria respetando reglas de alineación
para conseguir un acceso eficiente a los mismos. La alineación se refiere a la
divisibilidad de la posición de memoria inicial. Un dato de un byte puede
ubicarse en cualquier posición de memoria. Un número de dos bytes (media
palabra) debe ubicarse a partir de una posición de memoria múltiplo de dos. Un
número de 32 bit (palabra) debe ubicarse en una dirección de memoria múltiplo
de 4. Un entero de 64 bit (doble palabra) debe comenzar en una posición de
memoria múltiplo de 8. En las arquitecturas ARM de 32 bit los punteros son
números de 32 bit.

## Pila

Una pila es una estructura de datos que permite guardar y recuperar información
de modo que el primer dato guardado es el último recuperado (por su sigla en
ingles LIFO, Last In First Out). Una pila implementa las operaciones *push* y
*pop*. La operación *push* ingresa un dato en la pila, mientras que la operación
*pop* quita el último dato ingresado de la pila y retorna su valor.
En este problema implementaremos una pila de enteros de 32 bit. La información
sobre la memoria reservada y el estado de la pila se mantendrán en un
descriptor. El Listado 4 muestra un ejemplo de uso. Ubica tu solución en
`lib/pila/pila.S`

Listado 4. Ejemplo de uso de pila de enteros de 32 bit.

```c
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
    "\nERROR: Datos insuficietes en pila!\n",
    "\nERROR: Desborde de pila\n"
};
static void msgError(TipoError err){
    Serie_enviaCadena(mensajes[err]);
}

static void ingresaNumero(Pila *pila,const char *cadena){
    const int32_t A = atol(cadena);
    const int err = Pila_push(&pila,A);
    if (err){
        msgError(MSG_DESBORDE);
    }
}

void listado_4(void){
    Pila pila;
    Pila_init(&pila,memoria,TAMANO_PILA);
    Serie_enviaCadena(
        "Calculadora RPN. Ingrese números enteros y operadores"
        "separados por espacio en blanco o nueva línea.Operadores soportados:\n"
        "+ suma (a b -- a+b)\n"
        "- resta (a b -- a-b)\n"
        "* producto (a b -- a*b)\n"
        "/ cociente (a b -- a/b)\n"
        ". quita de pila y muestra el último resultado (r -- )\n"
        "bye finaliza el programa\n");
    int continuar = 1;
    while(continuar){
        int32_t A,B,err;
        const int tamano = Serie_recibePalabra(buffer,TAMANO_BUFFER);
        if (tamano == 1){
            switch(buffer[0]){
            case '+':
                err = Pila_pop(&pila,&A);
                err |= Pila_pop(&pila,&B);
                if (err){
                    msgError(MSG_INSUFICIENTE);
                }else{
                    Pila_push(&pila,A+B);
                }
            break; case '-':
                err = Pila_pop(&pila,&A);
                err |= Pila_pop(&pila,&B);
                if (err){
                    msgError(MSG_INSUFICIENTE);
                }else{
                    Pila_push(&pila,A-B);
                }
            break; case '*':
                err = Pila_pop(&pila,&A);
                err |= Pila_pop(&pila,&B);
                if (err){
                    msgError(MSG_INSUFICIENTE);
                }else{
                    Pila_push(&pila,A*B);
                }
            break; case '/':
                err = Pila_pop(&pila,&A);
                err |= Pila_pop(&pila,&B);
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
```

## Cola

Una cola es una estructura de datos que permite almacenar temporalmente
información para un posterior uso. A diferencia de una pila, en una cola el
primer dato en entrar será también el primero en salir (por su sigla en inglés
FIFO, First In First Out). Las colas son utilizadas para almacenar datos en
espera a ser procesados, de forma similar a cuando hacemos cola en una
ventanilla para hacer un trámite. En este problema implementarás una cola de
enteros de 32 bit que soporte las siguientes operaciones:

- *pone*: Pone un nuevo elemento en la cola
- *quita*: Quita el elemento más antiguo de la cola, retornando su valor

El Listado 5 muestra un ejemplo de uso de la cola. Ubica tu solución en
`lib/cola/cola.S`

Listado 5. Ejemplo de uso de cola de enteros de 32 bit.

```c
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
```

## Calificación

La calificación de este trabajo se realizará según los siguientes criterios:

1. El trabajo es original, individual y de autoría propia
2. Se han resuelto todos los ejercicios planteados.
3. La *memoria técnica* registra en forma detallada el proceso seguido al
   resolverlos.
4. El desarrollo en la memoria técnica sustenta el código escrito
5. El código escrito es correcto y pasa todas las pruebas
6. Se respeta el plazo de entrega pactado

*Notas:*

- El práctico no se desaprueba sino que es devuelto para corrección hasta su
  aprovación. El puntaje de aprobación será 10 salvo en caso que se detecte
  plagio o se entregue tarde.
- Si se detecta plagio debes rehacer el práctico y reduce la calificación del
  mismo a 4 una vez aprobado.
- La entrega tardía resta 2 puntos con demora hasta una semana, 3 puntos a las
  dos semanas y 6 puntos si demora tres semanas. Esto es, si demora 3 semanas
  en entregar aprobará con 4.

## Referencias

- ARM (2023). *Procedure Call Standard for the Arm® Architecture*. Alojado en GitHub.
  [https://github.com/ARM-software/abi-aa/blob/main/aapcs32/aapcs32.rst](https://github.com/ARM-software/abi-aa/blob/main/aapcs32/aapcs32.rst).
