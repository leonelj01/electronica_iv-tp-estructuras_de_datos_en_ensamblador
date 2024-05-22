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

.equ Cola_base,0 
.equ Cola_limite,4
.equ Cola_pEscritura,8
.equ Cola_pLectura,12
.equ Cola_lleno,16

/**
 * @brief Inicializa un descriptor de cola
 * 
 * @param descriptor Descriptor
 * @param base Dirección base de memoria
 * @param tamano Tamano de memoria para la cola
 */
defun Cola_init
    push {r4-r5,lr}
    str r1, [r0, #Cola_base]
    str r2, [r0, #Cola_limite]
    str r3, [r0, #Cola_pEscritura]
    str r4, [r0, #Cola_pLectura]
    str r5, [r0, #Cola_lleno]
    pop {r4-r5,pc}
endfun Cola_init

/**
 * @brief Pone un nuevo valor en cola.
 * 
 * @param descriptor Descriptor de cola
 * @param valor Valor a introducir
 * @return int 0 si la operación fue exitosa. -1 si la cola estaba llena.
 */
defun Cola_pone //r0: descriptor, r1: valor

endfun Cola_pone
/**
 * @brief Quita el elemento más antiguo de la cola y escribe su valor en 
 * destino.
 * 
 * @param descriptor Descriptor de cola
 * @param destino Puntero a variable donde guardar el valor quitado
 * @return int 0 si la operación tuvo éxito. -1 si la cola estaba vacía.
 */
defun Cola_quita
    mov r0,-1
    bx lr
endfun Cola_quita
