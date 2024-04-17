#include "serie.h"
#include "unity_config.h"
#include "timer_sistema.h"

void Unity_outputStart(void){
    TS_init();
    Serie_init(115200);
#ifdef ESPERA_PRUEBAS
    TS_espera(ESPERA_PRUEBAS);
#endif
}

void Unity_outputChar(int c){
    Serie_enviaCaracter(c);
}
void Unity_outputFlush(void){
    Serie_flush();
}
void Unity_outputComplete(void){
    Serie_deinit();
}
