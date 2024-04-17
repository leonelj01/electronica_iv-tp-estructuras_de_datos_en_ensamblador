#include "ops_memoria.h"
#include <unity.h>

void setUp(void){}
void tearDown(void){}

#define T1 "Hola "
#define T2 "Mundo"
#define T3 "12345"
#define CARACTERES_Ti 5

static void test_mueveMemoria_0(void){
    static const char esperado[] = T1 T2;
    char datos[sizeof(esperado)] ={};
    mueveMemoria(datos,esperado,sizeof(esperado));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(esperado,datos,sizeof(esperado));
}

static void test_mueveMemoria_1(void){    
    char datos[] = T1 T2 T3;
    static const char esperado[] = T1 T1 T2;
    mueveMemoria(datos+CARACTERES_Ti,datos,2*CARACTERES_Ti);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(esperado,datos,sizeof(esperado));
}

static void test_mueveMemoria_2(void){
    char datos[] = T3 T1 T2;
    static const char esperado[] = T1 T2 T2;
    mueveMemoria(datos,datos+CARACTERES_Ti,2*CARACTERES_Ti);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(esperado,datos,sizeof(esperado));
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_mueveMemoria_0);
    RUN_TEST(test_mueveMemoria_1);
    RUN_TEST(test_mueveMemoria_2);
    UNITY_END();
    for(;;){}
}
