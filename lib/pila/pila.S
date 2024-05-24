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

.equ Pila_base,0
.equ Pila_limite,4
.equ Pila_puntero,8
/**
 * @brief Inicializa un descriptor de pila. Inicialmente la pila está vacía
 * 
 * @param descriptor Descriptor a inicializar
 * @param base Dirección base (menor dirección) de la pila
 * @param capacidad Capacidad de la pila (en elementos)
 */
defun Pila_init
    // r0: descriptor, r1: base, r2: capacidad
    add r2, r1, r2, lsl #2
    str r1, [r0,#Pila_base]
    str r2, [r0,#Pila_limite]
    str r3, [r0,#Pila_puntero]
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
    // r0: descriptor, r1: valor a ingresar
    ldr r2, [r0,#Pila_puntero]
    ldr r3, [r0,#Pila_limite]
    cmp r3,r2
    bhs 1f


    ldrb r3,[r1]
    strb r3,[r2],#4
    mov r0, #0
 
0:
    bx lr

1:
    mov r0, #-1
    b   0b
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
    // r0: descriptor, r1: destino
    ldr r2,[r0,#Pila_puntero]
    ldr r3,[r0,#Pila_base]
    cmp r2,r3
    ble vacia


    ldrb r3,[r2],#-4
    strb r3,[r1]
    mov  r0,#0

salida_1:
    bx lr

vacia:
    mov r0,#-1
    b   salida_1
endfun Pila_pop
