#include <unity.h>
#include "arreglo.h"

void setUp(void){}
void tearDown(void){}

static void test_lee(void){
    enum{TELEM=10,NELEM=3};
    char nombres[NELEM][TELEM]={"Emilia","Armando","Griselda"};
    char aux[TELEM]={};
    Arreglo d[1];
    int r;
    Arreglo_init(d,nombres,TELEM,NELEM);
    r = Arreglo_lee(d,-1,aux);
    TEST_ASSERT_EQUAL(-1,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){},aux,TELEM);
    r = Arreglo_lee(d,NELEM,aux);
    TEST_ASSERT_EQUAL(-1,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){},aux,TELEM);
    r = Arreglo_lee(d,0,aux);
    TEST_ASSERT_EQUAL(0,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char [TELEM]){"Emilia"},aux,TELEM);
    Arreglo_init(d,nombres,TELEM,NELEM);
    r = Arreglo_lee(d,1,aux);
    TEST_ASSERT_EQUAL(0,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char [TELEM]){"Armando"},aux,TELEM);

    r = Arreglo_lee(d,2,aux);
    TEST_ASSERT_EQUAL(0,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char [TELEM]){"Griselda"},aux,TELEM);

}

static void test_escribe(void){
    enum{TELEM=10,NELEM=3};
    struct Datos{
        char guarda1[TELEM];
        char nombres[NELEM][TELEM];
        char guarda2[TELEM];
    }datos = {};
    Arreglo d[1];
    int r;

    Arreglo_init(d,datos.nombres,TELEM,NELEM);
    
    r=Arreglo_escribe(d,-1,(char [TELEM]){"Gaston"});
    TEST_ASSERT_EQUAL(-1,r);
    r=Arreglo_escribe(d,NELEM,(char [TELEM]){"Oscar"});
    TEST_ASSERT_EQUAL(-1,r);
    TEST_ASSERT_EQUAL_MEMORY(&(struct Datos){},&datos,sizeof datos);

    r=Arreglo_escribe(d,0,(char[TELEM]){"Bridgitte"});
    TEST_ASSERT_EQUAL(0,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Bridgitte"},datos.nombres[0],TELEM);

    r=Arreglo_escribe(d,2,(char[TELEM]){"Sebastian"});
    TEST_ASSERT_EQUAL(0,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Sebastian"},datos.nombres[2],TELEM);

    r=Arreglo_escribe(d,1,(char[TELEM]){"Orfelia"});
    TEST_ASSERT_EQUAL(0,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Orfelia"},datos.nombres[1],TELEM);
}

static void test_apunta(void){
    enum {TELEM=7,NELEM=4};
    char datos[NELEM][TELEM]={};
    Arreglo d[1];
    char *p;

    Arreglo_init(d,datos,TELEM,NELEM);

    p = Arreglo_apunta(d,-1);
    TEST_ASSERT_NULL(p);

    p = Arreglo_apunta(d,2);
    TEST_ASSERT_EQUAL_PTR(datos[2],p);

    p = Arreglo_apunta(d,0);
    TEST_ASSERT_EQUAL_PTR(datos[0],p);
    
    p = Arreglo_apunta(d,3);
    TEST_ASSERT_EQUAL_PTR(datos[3],p);

    p = Arreglo_apunta(d,1);
    TEST_ASSERT_EQUAL_PTR(datos[1],p);

    p = Arreglo_apunta(d,4);
    TEST_ASSERT_NULL(p);
}

static void test_intercambia(void){
    enum{TELEM=10,NELEM=3};
    char datos[NELEM][TELEM]={"Daryl","Janette","Josefina"};
    Arreglo d[1];
    int r;
    Arreglo_init(d,datos,TELEM,NELEM);
    r=Arreglo_intercambia(d,-1,1);
    TEST_ASSERT_EQUAL(-1,r);
    r=Arreglo_intercambia(d,0,3);
    TEST_ASSERT_EQUAL(-1,r);
    r=Arreglo_intercambia(d,3,-1);
    TEST_ASSERT_EQUAL(-1,r);

    r=Arreglo_intercambia(d,0,1);
    TEST_ASSERT_EQUAL(0,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Janette"},datos[0],TELEM);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Daryl"},datos[1],TELEM);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Josefina"},datos[2],TELEM);

    r=Arreglo_intercambia(d,2,0);
    TEST_ASSERT_EQUAL(0,r);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Josefina"},datos[0],TELEM);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Daryl"},datos[1],TELEM);
    TEST_ASSERT_EQUAL_CHAR_ARRAY((char[TELEM]){"Janette"},datos[2],TELEM);
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