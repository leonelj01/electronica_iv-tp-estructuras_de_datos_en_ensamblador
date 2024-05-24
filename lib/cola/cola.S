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
    // r0: descriptor, r1: base, r2: tamaño de memoria 
    str r1, [r0, #Cola_base]
    add r2, r1, r2, lsl #2
    str r2, [r0, #Cola_limite]
    str r1, [r0, #Cola_pEscritura]
    str r1, [r0, #Cola_pLectura]
    mov r3, #0
    str r3, [r0, #Cola_lleno]
    bx lr
endfun Cola_init

/**
 * @brief Pone un nuevo valor en cola.
 * 
 * @param descriptor Descriptor de cola
 * @param valor Valor a introducir
 * @return int 0 si la operación fue exitosa. -1 si la cola estaba llena.
 */
defun Cola_pone 
    //r0: descriptor, r1: valor
    ldr r2,[r0,#Cola_pEscritura]
    ldr r3,[r0,#Cola_pLectura]
    cmp r2,r3
    bne 1f

    ldr r3,[r0,#Cola_lleno]
    cbz r3, 1f
    
    mov r0,#-1
    b   0f

1:
    mov r0,#0
0:
    bx lr
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
    // r0: descriptor, r1: destino
    ldr r2,[r0,#Cola_pEscritura]
    ldr r3,[r0,#Cola_pLectura]
    cmp r2,r3
    bne 1f

    ldr r3,[r0,#Cola_lleno]
    cbnz r3, 1f
    
    mov r0,#-1
    b   0f

1:
    mov r0,#0

0:
    bx lr
endfun Cola_quita
