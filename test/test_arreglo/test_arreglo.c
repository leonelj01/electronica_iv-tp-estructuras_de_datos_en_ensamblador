#include <unity.h>
#include "arreglo.h"

enum{TAMANO_ELEMENTO = 20,NUM_ELEMENTOS=4};

static struct Datos{
    char nombres[NUM_ELEMENTOS][TAMANO_ELEMENTO];
    char auxiliar[TAMANO_ELEMENTO];
    Arreglo descriptor;
}datos;

#define S0 "Valeria"
#define S1 "Alejandro"
#define S2 "Hernan"
#define S3 "Florencia"
#define Sx "Monica"
void setUp(void){
    datos = (struct Datos){
        .nombres={S0,S1,S2,S3},
        .auxiliar={},
        .descriptor={}
    };
    Arreglo_init(
        &datos.descriptor,&datos.nombres,TAMANO_ELEMENTO,NUM_ELEMENTOS);
}
void tearDown(void){}

static void test_lee(void){
    Arreglo *const descriptor = &datos.descriptor;
    char *const aux = datos.auxiliar;
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,0,aux));
    TEST_ASSERT_EQUAL_STRING(S0,aux);
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,3,aux));
    TEST_ASSERT_EQUAL_STRING(S3,aux);
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,1,aux));
    TEST_ASSERT_EQUAL_STRING(S1,aux);
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,2,aux));
    TEST_ASSERT_EQUAL_STRING(S2,aux);
}

static void test_escribe(void){
    Arreglo *const descriptor = &datos.descriptor;
    char *const aux = datos.auxiliar;
    TEST_ASSERT_FALSE(Arreglo_escribe(descriptor,0,Sx));
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,0,aux));
    TEST_ASSERT_EQUAL_STRING(Sx,aux);
    TEST_ASSERT_FALSE(Arreglo_escribe(descriptor,1,S3));
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,1,aux));
    TEST_ASSERT_EQUAL_STRING(S3,aux);
    TEST_ASSERT_FALSE(Arreglo_escribe(descriptor,2,S1));
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,2,aux));
    TEST_ASSERT_EQUAL_STRING(S1,aux);
    TEST_ASSERT_FALSE(Arreglo_escribe(descriptor,3,S0));
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,3,aux));
    TEST_ASSERT_EQUAL_STRING(S0,aux);
}

static void test_apunta(void){
    Arreglo *const descriptor = &datos.descriptor;
    char *const aux = datos.auxiliar;

    char *const p0 = Arreglo_apunta(descriptor,0);
    char *const p3 = Arreglo_apunta(descriptor,3);

    TEST_ASSERT_NOT_NULL(p0);
    TEST_ASSERT_NOT_NULL(p3);

    TEST_ASSERT_EQUAL_STRING(S0,p0);
    TEST_ASSERT_EQUAL_STRING(S3,p3);

    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,1,p0));
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,0,aux));
    TEST_ASSERT_EQUAL_STRING(S1,aux);

    TEST_ASSERT_FALSE(Arreglo_escribe(descriptor,3,Sx));
    TEST_ASSERT_EQUAL_STRING(Sx,p3);
}

static void test_intercambia(void){
    Arreglo *const descriptor = &datos.descriptor;
    char *const aux = datos.auxiliar;

    TEST_ASSERT_FALSE(Arreglo_intercambia(descriptor,0,3));
    TEST_ASSERT_FALSE(Arreglo_intercambia(descriptor,1,2));

    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,0,aux));
    TEST_ASSERT_EQUAL_STRING(S3,aux);
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,1,aux));
    TEST_ASSERT_EQUAL_STRING(S2,aux);
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,2,aux));
    TEST_ASSERT_EQUAL_STRING(S1,aux);
    TEST_ASSERT_FALSE(Arreglo_lee(descriptor,3,aux));
    TEST_ASSERT_EQUAL_STRING(S0,aux);

}
int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_lee);
    RUN_TEST(test_escribe);
    RUN_TEST(test_apunta);
    RUN_TEST(test_intercambia);
    UNITY_END();
    return 0;
}