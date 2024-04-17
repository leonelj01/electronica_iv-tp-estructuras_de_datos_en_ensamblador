#ifndef COLA_H
#define COLA_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Cola {
    /**
     * @brief Dirección de la primera posición de la memoria reservada
     * 
     */
    int32_t *base;

    /**
     * @brief Dirección de la posición siguiente a la última posición reservada 
     * 
     */
    int32_t *limite;

    /**
     * @brief Dirección de la posición donde se va a escribir el proximo dato
     * 
     */
    int32_t *pEscritura;
    
    /**
     * @brief Dirección de la posición del dato más antiguo no leído 
     * 
     */
    int32_t *pLectura;

    /**
     * @brief Indica cola llena, cuando pEscritura == pLectura, 1 indica cola
     * llena y 0 indica cola vacía.
     * 
     * @note el tipo bool se implementa como entero de 8 bit pero solo puede
     * contener valores 0 y 1
     */
    bool llena;
}Cola;

/**
 * @brief Inicializa un descriptor de cola
 * 
 * @param descriptor Descriptor
 * @param base Dirección base de memoria
 * @param tamano Tamano de memoria para la cola
 */
void Cola_init(Cola *descriptor, int32_t *base,int tamano);

/**
 * @brief Pone un nuevo valor en cola.
 * 
 * @param descriptor Descriptor de cola
 * @param valor Valor a introducir
 * @return int 0 si la operación fue exitosa. -1 si la cola estaba llena.
 */
int Cola_pone(Cola *descriptor, int32_t valor);

/**
 * @brief Quita el elemento más antiguo de la cola y escribe su valor en 
 * destino.
 * 
 * @param descriptor Descriptor de cola
 * @param destino Puntero a variable donde guardar el valor quitado
 * @return int 0 si la operación tuvo éxito. -1 si la cola estaba vacía.
 */
int Cola_quita(Cola *descriptor, int32_t *destino);

#endif