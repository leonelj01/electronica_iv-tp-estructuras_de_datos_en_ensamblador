#ifndef PILA_H
#define PILA_H
#include <stddef.h>
#include <stdint.h>

typedef struct Pila{
    int32_t *base; // Primera posición de la memoria reservada
    int32_t *limite; // Dirección siguiente a la última posición reservada
    int32_t *puntero; // Dirección del último dato insertado con push
}Pila;

/**
 * @brief Inicializa un descriptor de pila. Inicialmente la pila está vacía
 * 
 * @param descriptor Descriptor a inicializar
 * @param base Dirección base (menor dirección) de la pila
 * @param capacidad Capacidad de la pila (en elementos)
 */
void Pila_init(Pila *descriptor, int32_t *base, size_t capacidad);

/**
 * @brief Ingresa un valor en la pila descrita
 * 
 * @param descriptor Descriptor de pila
 * @param valor Valor a ingresar
 * @return int 0 si la operación se realizó, -1 si la pila estaba llena
 */
int Pila_push(Pila *descriptor, int32_t valor);

/**
 * @brief Quita el último elemento de la pila descrita y copia su valor en
 * destino
 * 
 * @param descriptor Descriptor de pila
 * @param destino Dirección de la variable de destino
 * @return int 0 si la operación se realizó, -1 si la pila estaba vacía
 */
int Pila_pop(Pila *descriptor, int32_t *destino);

#endif