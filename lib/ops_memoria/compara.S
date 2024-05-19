.syntax unified
.cpu cortex-m3
.thumb

.text

/**
 * @brief Compara dos regiones de memoria byte a byte como valores *sin signo*. 
 * Retorna 0 si son iguales, -1 si en la primera diferencia el valor de
 * izquierda es el menor y 1 si en la primera diferencia el valor de la
 * izquierda es mayor
 * 
 * @param izquierda Dirección base de la primera región
 * @param derecha Dirección base de la segunda región
 * @param tamano tamaño de las regiones
 * @return int 
 */

    .type comparaMemoria, %function
    .global comparaMemoria
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
    .size comparaMemoria, . - comparaMemoria
