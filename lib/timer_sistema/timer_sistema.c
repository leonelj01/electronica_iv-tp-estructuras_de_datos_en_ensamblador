#include "timer_sistema.h"
#include <stm32f1xx.h>

static volatile uint32_t ticks;

void SysTick_Handler(void){
    ticks++;
}


void TS_init(void){
    SystemCoreClockUpdate();
    const uint32_t ticks = (SystemCoreClock+500)/1000; 
    SysTick_Config(ticks);
}

uint32_t TS_milis(void){
    return ticks;
}

void TS_espera(uint32_t milis){
    const uint32_t inicial = TS_milis();
    while(TS_milis()-inicial < milis);
}
