.syntax unified
.cpu cortex-m3
.thumb


.text

/**
 * @brief Copia los tamano valores desde origen a destino, si la memoria de
 * destino se solapa con la de origen al final destino tiene que tener la copia
 * completa de los datos (y en ese caso origen no la tendrá más)
 * 
 * @param destino puntero a base (menor dirección) de la región de destino
 * @param origen puntero a base de la región de origen
 * @param tamano tamano de las regiones
 */

    .type mueveMemoria, %function
    .global mueveMemoria
mueveMemoria:
    // r0 -> Origen
    // r1 -> Destino
    // r2 -> Longitud de la palabra

    cmp     r0, r1                 // Compara los punteros de origen y destino
    beq     fin
    blo     copia_desde_inicio
    bhi     copia_desde_final

copia_desde_inicio:
    ldrb    r3, [r1], #1          // Carga el byte desde el origen y actualiza el puntero
    strb    r3, [r0], #1          // Almacena el byte en el destino y actualiza el puntero
    subs    r2, #1                // Decrementa la longitud
    bne     copia_desde_inicio
    b       fin

copia_desde_final:
    add     r4, r1, r2             // Ajusta los punteros al final de las regiones
    add     r5, r0, r2

copia_desde_final_loop:
    ldrb    r3, [r4, #-1]!         // Carga el byte desde el origen y actualiza el puntero
    strb    r3, [r5, #-1]!        // Almacena el byte en el destino y actualiza el puntero
    subs    r2, #1                // Decrementa la longitud
    bne     copia_desde_final_loop

fin:
    bx lr
    .size mueveMemoria, . - mueveMemoria

