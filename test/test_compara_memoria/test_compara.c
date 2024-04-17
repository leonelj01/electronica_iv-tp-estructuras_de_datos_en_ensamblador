#include "ops_memoria.h"
#include <unity.h>

void setUp(void){}
void tearDown(void){}

static const char A[]="aaxxx";
static const char B[]="abcde";
static const char C[]="axaaa";

static void test_comparaMemoria_0(void){
    TEST_ASSERT_EQUAL(comparaMemoria(A,A,sizeof(A)),0);
    TEST_ASSERT_EQUAL(comparaMemoria(B,B,sizeof(B)),0);
    TEST_ASSERT_EQUAL(comparaMemoria(C,C,sizeof(C)),0);
}

static void test_comparaMemoria_1(void){
    TEST_ASSERT_EQUAL(comparaMemoria(A,B,sizeof(A)),-1);
    TEST_ASSERT_EQUAL(comparaMemoria(B,C,sizeof(B)),-1);
    TEST_ASSERT_EQUAL(comparaMemoria(A,C,sizeof(B)),-1);
}

static void test_comparaMemoria_2(void){
    TEST_ASSERT_EQUAL(comparaMemoria(B,A,sizeof(A)),1);
    TEST_ASSERT_EQUAL(comparaMemoria(C,B,sizeof(B)),1);
    TEST_ASSERT_EQUAL(comparaMemoria(C,A,sizeof(B)),1);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_comparaMemoria_0);
    RUN_TEST(test_comparaMemoria_1);
    RUN_TEST(test_comparaMemoria_2);
    UNITY_END();
    for(;;){}
}
