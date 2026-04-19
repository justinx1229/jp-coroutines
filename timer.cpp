
#include "timer.h"
#include "memory.h"

extern uint8_t regs[SIZE_REGS];

uint16_t div_cycles = 0;
uint16_t tima_cycles = 0;

const uint8_t REG_DIV = 0x04;
const uint8_t REG_TIMA = 0x05;
const uint8_t REG_TMA = 0x06;
const uint8_t REG_TAC = 0x07;
const uint8_t REG_IF = 0x0F;

uint16_t timer_period(uint8_t tac) {
    switch (tac & LO_2) {
        case 0:
            return 256;
        case 1:
            return 4;
        case 2:
            return 16;
        case 3:
            return 64;
    }

    return 256;
}

void reset_timer() {
    div_cycles = 0;
    tima_cycles = 0;
}

void tick_timer(uint8_t m_cycles) {
    div_cycles += m_cycles;
    while (div_cycles >= 64) {
        div_cycles -= 64;
        regs[REG_DIV]++;
    }

    if (!(regs[REG_TAC] & 0b100)) {
        return;
    }

    tima_cycles += m_cycles;
    uint16_t period = timer_period(regs[REG_TAC]);

    while (tima_cycles >= period) {
        tima_cycles -= period;

        if (regs[REG_TIMA] == 0xFF) {
            regs[REG_TIMA] = regs[REG_TMA];
            regs[REG_IF] |= 0b100;
        }
        else {
            regs[REG_TIMA]++;
        }
    }
}

void timer_write(uint16_t address, uint8_t byte) {
    if (address == 0xFF04) {
        regs[REG_DIV] = 0;
        div_cycles = 0;
    }
    else if (address == 0xFF07) {
        regs[REG_TAC] = byte;
        tima_cycles = 0;
    }
}
