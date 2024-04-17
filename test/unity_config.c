#include "serie.h"
#include "unity_config.h"

void Unity_outputStart(void){
    Serie_init(115200);
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
