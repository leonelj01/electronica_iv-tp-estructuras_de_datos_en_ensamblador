#ifndef TIMER_SISTEMA_H
#define TIMER_SISTEMA_H

#include <stdint.h>

void TS_init(void);

uint32_t TS_milis(void);

void TS_espera(uint32_t milis);

#endif
