#include <unity.h>
#include "pila.h"

enum {TAMANO_PILA=5};

static struct Datos{
    int32_t memoria[TAMANO_PILA+2];
    Pila descriptor;
}datos;

void setUp(void){
    datos = (struct Datos){
        .memoria={0xBADBEEF,
                  0xBADBEEF,0xBADBEEF,0xBADBEEF,0xBADBEEF,0xBADBEEF,
                  0xBADBEEF},
        .descriptor={}
    };
    Pila_init(&datos.descriptor,datos.memoria+1,TAMANO_PILA);
}

void tearDown(void){
    TEST_ASSERT_EQUAL_INT32(0xBADBEEF,datos.memoria[0]);
    TEST_ASSERT_EQUAL_INT32(0xBADBEEF,datos.memoria[TAMANO_PILA+1]);
}

static void test_pila_push_pop(void){
    Pila *const descriptor = &datos.descriptor;
    int32_t aux=0xBABAFAB0;
    TEST_ASSERT_FALSE(Pila_push(descriptor,1));
    TEST_ASSERT_FALSE(Pila_push(descriptor,2));
    TEST_ASSERT_FALSE(Pila_push(descriptor,3));
    TEST_ASSERT_FALSE(Pila_push(descriptor,4));
    TEST_ASSERT_FALSE(Pila_push(descriptor,5));
    TEST_ASSERT_FALSE(Pila_pop(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(5,aux);
    TEST_ASSERT_FALSE(Pila_pop(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(4,aux);
    TEST_ASSERT_FALSE(Pila_pop(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(3,aux);
    TEST_ASSERT_FALSE(Pila_pop(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(2,aux);
    TEST_ASSERT_FALSE(Pila_pop(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(1,aux);
    
}
static void test_pila_overflow(void){
    Pila *const descriptor = &datos.descriptor;

    for(int i=0;i<TAMANO_PILA;++i)
        Pila_push(descriptor,i);
    TEST_ASSERT_EQUAL_INT(-1,Pila_push(descriptor,TAMANO_PILA));
}

static void test_pila_underflow(void){
    Pila *const descriptor = &datos.descriptor;
    int32_t aux=0xBABAFAB0;
    TEST_ASSERT_EQUAL_INT(-1,Pila_pop(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(0xBABAFAB0,aux);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_pila_push_pop);
    RUN_TEST(test_pila_overflow);
    RUN_TEST(test_pila_underflow);
    UNITY_END();
    return 0;
}