
#pragma once

#include "consts.h"

void reset_interrupt();
bool is_ime();
bool is_halt();
void set_halt(bool value);
void disable_interrupts();
void set_sime();
void sime_to_ime();
uint8_t handle_interrupt(uint16_t &pc, uint16_t &sp);
