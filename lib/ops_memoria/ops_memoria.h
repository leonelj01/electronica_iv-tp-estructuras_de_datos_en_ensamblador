#ifndef OPS_MEMORIA_H
#define OPS_MEMORIA_H

#include <stddef.h>
/**
 * @brief Copia los tamano valores desde origen a destino, si la memoria de
 * destino se solapa con la de origen al final destino tiene que tener la copia
 * completa de los datos (y en ese caso origen no la tendrá más)
 * 
 * @param destino puntero a base (menor dirección) de la región de destino
 * @param origen puntero a base de la región de origen
 * @param tamano tamano de las regiones
 */
void mueveMemoria(void *destino,const void *origen, size_t tamano);

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
int comparaMemoria(const void *izquierda,const void *derecha, size_t tamano);

#endif