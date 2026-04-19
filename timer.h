
#pragma once

#include "consts.h"

void reset_timer();
void tick_timer(uint8_t m_cycles);
void timer_write(uint16_t address, uint8_t byte);
