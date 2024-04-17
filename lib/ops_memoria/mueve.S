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
    bx lr
    .size mueveMemoria, . - mueveMemoria

