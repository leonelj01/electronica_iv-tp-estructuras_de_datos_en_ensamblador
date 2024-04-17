.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.macro defun nombre
    .section .text.\nombre
    .global \nombre
    .type \nombre, %function
\nombre:
.endm

.macro endfun nombre
    .size \nombre, . - \nombre
.endm

/**
 * @brief Inicializa un descriptor de pila. Inicialmente la pila está vacía
 * 
 * @param descriptor Descriptor a inicializar
 * @param base Dirección base (menor dirección) de la pila
 * @param capacidad Capacidad de la pila (en elementos)
 */
defun Pila_init
    bx lr
endfun Pila_init

/**
 * @brief Ingresa un valor en la pila descrita
 * 
 * @param descriptor Descriptor de pila
 * @param valor Valor a ingresar
 * @return int 0 si la operación se realizó, -1 si la pila estaba llena
 */
defun Pila_push
    mov r0,#-1
    bx lr
endfun Pila_push

/**
 * @brief Quita el último elemento de la pila descrita y copia su valor en
 * destino
 * 
 * @param descriptor Descriptor de pila
 * @param destino Dirección de la variable de destino
 * @return int 0 si la operación se realizó, -1 si la pila estaba vacía
 */
defun Pila_pop
    mov r0,#-1
    bx lr
endfun Pila_pop
