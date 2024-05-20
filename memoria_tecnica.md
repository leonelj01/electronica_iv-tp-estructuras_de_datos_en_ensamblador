# Memoria técnica - Estructuras de datos en ensamblador de un procesador ARMv7M

Electrónica IV - TP - Estructuras de datos en ensamblador de un procesador ARMv7M

### ***Juarez, Gustavo Leonel***

## Objetivos

> Aquí explica de forma resumida de que se trata el práctico y lista los
> ejercicios que vas a desarrollar.
>
> A partir de aquí documentarás para cada ejercicio tu proceso de diseño, lo que
> será de utilidad para razonar sobre como llegaste a un resultado, por qué
> se cometieron errores (en caso que haya ocurrido) y como podrías mejorarlo
> para llegar a buenos resultados de forma más eficiente. Si bien el contenido
> del trabajo irá evolucionando, es importante que cuando encuentres un problema
> o te "trabes" durante el proceso, escribas una nota.  
> *Sugiero conservar estas instrucciones durante el desarrollo, posiblemente
> en forma impresa, para poder referirte a ellas como guía mientras trabajas*
>

## Mueve memoria

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

### Análisis del problema

Para resolver el problema, primero hay que determinar como copiar los datos de origen en destino pues dependiendo de donde
se encuentre este se copiará desde izquierda a derecha o al reves, esto determina como se recorreran. Por ello si destino 
se encuentra antes que origen se debe copiar los datos de izquierda a derecha. En caso de que destino este ubicado despues
de origen entonces debo copiar los datos de derecha a izquierda.
![](figuras/fig-1.png)

Entonces para comenzar determinamos de que forma se copiaran los datos, si de izquierda a derecha o al reves. Para ello se 
utiliza el siguiente codigo:

```asm
    cmp     r0, r1                      // Compara los punteros de origen y destino
    beq     fin                         // Si ambos son iguales salta a "fin"
    blo     copia_desde_inicio          // Si r0 es menor que r1 entonces copio de izquierda a derecha
    bhi     copia_desde_final           // Si r0 es mayor que r1 entonces copio de derecha a izquierda
```

Donde las etiqueta de `copia_desde_inicio` y `copia_desde_final` representan saltos que determinarán como se realizaran las
diferentes forma de escritura.

Para poder copiar desde izquierda a derecha se recorre el registro, en este caso será r3, en dicha direccion. Para lograr
esto se suma un byte cada vez que se produce un salto a `copia_desde_inicio`:

```asm
copia_desde_inicio:
    ldrb    r3, [r1], #1                // Carga el byte desde el origen y actualiza el puntero
    strb    r3, [r0], #1                // Almacena el byte en el destino y actualiza el puntero
    subs    r2, #1                      // Decrementa la longitud
    bne     copia_desde_inicio
    b       fin
```

En cambio para poder copiar desde derecha a izquierda primero nos debemos ubicar en la posicion desde la cual queremos copiar,
esta estará dada por el tamaño de la palabra, entonces primero debemos ubicarnos al final de la palabra que queremos copiar
esto se realiza en `copia_desde_final`. Luego  debemos disminuir el tamaño en un byte cada vez que se produce un salto a 
`copia_desde_final_loop`:

```asm
copia_desde_final:
    add     r1, r2                      // Ajusta los punteros al final de las regiones
    add     r0, r2

copia_desde_final_loop:
    ldrb    r3, [r1, #-1]!              // Carga el byte desde el origen y actualiza el puntero
    strb    r3, [r0, #-1]!              // Almacena el byte en el destino y actualiza el puntero
    subs    r2, #1                      // Decrementa la longitud
    bne     copia_desde_final_loop
```
### Solución
``` asm
mueveMemoria:
    // r0 -> Destino 
    // r1 -> Origen
    // r2 -> Longitud de la palabra

    cmp     r0, r1                      // Compara los punteros de origen y destino
    beq     fin                         // Si ambos son iguales salta a "fin"
    blo     copia_desde_inicio          // Si r0 es menor que r1 entonces copio de izquierda a derecha
    bhi     copia_desde_final           // Si r0 es mayor que r1 entonces copio de derecha a izquierda

copia_desde_inicio:
    ldrb    r3, [r1], #1                // Carga el byte desde el origen y actualiza el puntero
    strb    r3, [r0], #1                // Almacena el byte en el destino y actualiza el puntero
    subs    r2, #1                      // Decrementa la longitud
    bne     copia_desde_inicio
    b       fin

copia_desde_final:
    add     r1, r2                      // Ajusta los punteros al final de las regiones
    add     r0, r2

copia_desde_final_loop:
    ldrb    r3, [r1, #-1]!              // Carga el byte desde el origen y actualiza el puntero
    strb    r3, [r0, #-1]!              // Almacena el byte en el destino y actualiza el puntero
    subs    r2, #1                      // Decrementa la longitud
    bne     copia_desde_final_loop

fin:
    bx lr
```
## Compara memoria

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

### Análisis del problema
Para lograr resolver el ejercicio se debe tener en cuenta que la comparacion de las regiones de memoria se realiza byte a byte,
es decir, voy comparando los byte mientras recorro los registros `Izquierda` y `derecha`. Luego la funcion deberá retornar un
valor segun lo siguiente:
* Si *`Izquierda`* es igual que *`derecha`* se retornará *0*
* Si *`Izquierda`* es menor que *`derecha`* se retornará *-1*
* Si *`Izquierda`* es mayor que *`derecha`* se retornará *1*
![](figuras/fig-2.png)

### Solución

```asm
comparaMemoria:
        /*
        * r0 -> Izquierda
        * r1 -> Derecha
        * r2 -> Tamaño
        */
compara:
        ldrb    r3,  [r0], #1           // Cargo el byte desde Izquierda y actualizo el puntero.
        ldrb    r12, [r1], #1           // Cargo el byte desde Derecha y actualizo el puntero.

        cmp     r3, r12                 // Comparo los punteros de Izquierda y Derecha.
        blo     izquierda               // Si Izquierda es menor que derecha entonces salta a la etiqueta.
        bhi     derecha                 // Si Izquierda es mayor que derecha entonces salta a la etiqueta.
        subs    r2, #1                  // Decremento la longitud de la palabra
        bne     compara                 // Mientras r2 sea distinto de cero salto a compara y continuo el loop

        mov     r0, #0                  // Retorna 0
        b       fin

izquierda:
        mov     r0, #-1                 // Retorna -1
        b       fin

derecha:
        mov     r0, #1                  // Retorna 1

fin:
        bx      lr
```

## Arreglo
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

```c
typedef struct Arreglo{
    void *base;
    int32_t tamanoElemento;
    int32_t numElementos;
}Arreglo;

/**
 * @brief Inicializa un descriptor que describe un arreglo con una direccion de
 * memoria base, tamaño de elemento y número de elementos dados
 * 
 * @param descriptor Puntero al descriptor a inicializar 
 * @param base Dirección base del arreglo
 * @param tamanoElemento Tamaño del elemento
 * @param numElementos Número de elementos
 */
void Arreglo_init(Arreglo *descriptor, void *base,
                  int tamanoElemento, int numElementos);

/**
 * @brief Retorna un puntero a un elemento del arreglo descrito
 * 
 * @param descriptor Descriptor de arreglo inicializado con Arreglo_init
 * @param indice
 * @return void* Puntero al elemento indicado por indice, NULL si no existe el
 * elemento (NULL es la dirección 0)
 */
void *Arreglo_apunta(Arreglo *descriptor, int indice);

/**
 * @brief Lee el valor de un elemento del arreglo descrito, copiándolo en el
 * espacio de memoria proporcionado. Supone que la región de memoria tiene
 * tamaño suficiente
 * 
 * @param descriptor Descriptor del arreglo
 * @param indice Índice del elemento a leer
 * @param destino Puntero a la regió de memoria donde copiar el valor
 * @return int 0 si la operación pudo realizarse, -1 si el elemento no existe
 */
int Arreglo_lee(const Arreglo *descriptor, int indice, void *destino);

/**
 * @brief Escribe el valor proporcionado en el arreglo descrito
 * 
 * @param descriptor Descriptor de arreglo
 * @param indice Índice del elemento a escribir
 * @param origen Puntero a región conteniendo los datos a escribir
 * @return int 0 si efectuó la operación, -1 si no existe el elemento en el
 * arreglo
 */
int Arreglo_escribe(Arreglo *descriptor, int indice, const void *origen);

/**
 * @brief Intercambia los valores de dos elementos del arreglo
 * 
 * @param descriptor Descriptor de arreglo
 * @param indice_1 Primer índice
 * @param indice_2 Segundo índice
 * @return int 0 si tuvo exito, -1 si algún elemento no existe
 */
int Arreglo_intercambia(Arreglo *descriptor, int indice_1, int indice_2);
```

### Análisis del problema

### 1. Arreglo_Init:

Antes de comenzar a realizar las funciones debemos establecer el descriptor del arreglo, el cual definirá los parametros
del mismo. Estos son:
- Base: es la dirección a la que se encuentra el primer elemento.
- Tamaño de elementos: es la cantidad maxima de elementos que puedo almacenar.
- Numero de elementos: es la cantidad de elementos que se tiene.

"figura hecha a mano INIT"

### 2. apunta


### Solución

## Pila
### Análisis del problema
### Solución

## Cola
### Análisis del problema
### Solución

> ### Notas
>
> Incluye aquí notas describiendo tu experiencia en la solución del ejercicio.
> En casos simples puede que no tengas nada que añadir y en esos casos puedes
> omoitirlo. Debe figurar aquí una nota por cada problema o dificultad que te
> sorprendió o causó que te "trabes", busques ayuda o dediques más de 5' de
> razonamiento a entender como y por qué ocurre algo distinto a lo esperado.
>
> ### Referencias
>
> Incluye aquí referencias a los recursos que utilizaste durante tu proceso de
> diseño, las mismas deben haber sido *citadas* en estilo *APA* en el punto
> del desarrollo donde las utilizaste.
> *NOTA* Es prácticamente imposible que hayas completado un ejercicio sin
> necesidad de consultar ninguna referencia (libro, video, ejemplo en internet,
> etc.) y es importante que las incluyas aquí en el momento mismo que las
> consultes. Si una referencia resulto guiarte de forma erronea, inclúyela y
> citala en la correspondiente nota que explique lo ocurrido.
>
