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
    add r2, r1, r2, lsl #2
    str r1, [r0, #Cola_base]
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
    push {r4-r5,lr}
    ldr r2,  [r0,#Cola_limite]
    ldr r3,  [r0,#Cola_pEscritura]
    ldr r4,  [r0,#Cola_pLectura]
    ldr r5,  [r0,#Cola_base]
    ldr r12, [r0,#Cola_lleno]

    cmp r12,#0
    bne 1f
    str r1, [r3]
    add r3, #4
    cmp r3,r2
    beq 2f

    0:
        cmp r3,r4
        beq 3f
        str r3, [r0,#Cola_pEscritura]
        mov r0,#0
        b fin

    1: 
        mov r0,#-1
        b fin

    2:
        mov r3,r5
        b 0b

    3:
        mov r12,#1
        str r12, [r0,#Cola_lleno]
        str r3,  [r0,#Cola_pEscritura]
        mov r0,#0

    fin:
        pop {r4-r5,pc}
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
    push {r4-r6,lr}
    ldr r2,  [r0,#Cola_limite]
    ldr r3,  [r0,#Cola_pEscritura]
    ldr r4,  [r0,#Cola_pLectura]
    ldr r5,  [r0,#Cola_base]
    ldr r12, [r0,#Cola_lleno]

    cmp r12,#1
    bne 1f
    mov r12,#0
    str r12,[r0,#Cola_lleno]

0:
    ldr r6, [r4]
    str r6, [r1]
    add r4,#4
    cmp r4,r2
    beq 2f
    str r4, [r0,#Cola_pLectura]
    mov r0,#0
    b final

1:
    cmp r3,r4
    bne 0b
    mov r0,#-1
    b final

2:
    mov r4,r5
    str r4, [r0,#Cola_pLectura]
    mov r0,#0
    b final

final:
    pop {r4-r6,pc}
endfun Cola_quita
