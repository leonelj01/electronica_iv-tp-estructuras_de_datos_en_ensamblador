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
        ldrb    r3, [r0], #1
        ldrb    r4, [r1], #1

        cmp     r3, r4
        blo     izquierda
        bhi     derecha
        subs    r2, #1
        bne     compara

        mov     r0, #0
        b       fin

izquierda:
        mov     r0, #-1
        b       fin

derecha:
        mov     r0, #1
        b       fin

fin:
        bx      lr
    .size comparaMemoria, . - comparaMemoria
