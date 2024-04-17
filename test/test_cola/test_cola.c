#include <unity.h>
#include "cola.h"

enum{TAMANO_COLA = 5};

static struct Datos{
    int32_t memoria[TAMANO_COLA+2];
    Cola descriptor;
}datos;

void setUp(void){
    datos = (struct Datos){
        .memoria = {0xBADBEEF,
                    0xBADBEEF,0xBADBEEF,0xBADBEEF,0xBADBEEF,0xBADBEEF,
                    0xBADBEEF},
        .descriptor = {}
    };
    Cola_init(&datos.descriptor,datos.memoria+1,TAMANO_COLA);
}

void tearDown(void){
    TEST_ASSERT_EQUAL_INT32(0xBADBEEF,datos.memoria[0]);
    TEST_ASSERT_EQUAL_INT32(0xBADBEEF,datos.memoria[TAMANO_COLA+1]);
}

static void test_pone_quita(void){
    Cola *const descriptor = &datos.descriptor;
    int32_t aux=0xBABAFAB0;
    TEST_ASSERT_FALSE(Cola_pone(descriptor,1));
    TEST_ASSERT_FALSE(Cola_pone(descriptor,2));
    TEST_ASSERT_FALSE(Cola_pone(descriptor,3));
    TEST_ASSERT_FALSE(Cola_pone(descriptor,4));
    TEST_ASSERT_FALSE(Cola_pone(descriptor,5));
    TEST_ASSERT_FALSE(Cola_quita(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(1,aux);
    TEST_ASSERT_FALSE(Cola_quita(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(2,aux);
    TEST_ASSERT_FALSE(Cola_quita(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(3,aux);
    TEST_ASSERT_FALSE(Cola_quita(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(4,aux);
    TEST_ASSERT_FALSE(Cola_quita(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(5,aux);
}

static void test_underflow(void){
    Cola *const descriptor = &datos.descriptor;
    int32_t aux = 0xBABAFAB0; 
    TEST_ASSERT_EQUAL_INT(-1,Cola_quita(descriptor,&aux));
    TEST_ASSERT_EQUAL_INT32(0xBABAFAB0,aux);
}

static void test_overflow(void){
    Cola *const descriptor = &datos.descriptor;
    for (int i=0;i<TAMANO_COLA;++i)
        TEST_ASSERT_FALSE(Cola_pone(descriptor,i));
    TEST_ASSERT_EQUAL_INT(-1,Cola_pone(descriptor,TAMANO_COLA));
}
int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_pone_quita);
    RUN_TEST(test_underflow);
    RUN_TEST(test_overflow);
    UNITY_END();
    return 0;
}