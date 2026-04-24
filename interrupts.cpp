
#include "interrupts.h"
#include "memory.h"
#include "timer.h"

bool halt = false;
bool set_ime = false;
bool ime = false;

void reset_interrupt() {
    halt = false;
    set_ime = false;
    ime = false;
}

bool is_ime() {
    return ime;
}

bool is_halt() {
    return halt;
}

void set_halt(bool value) {
    halt = value;
}

void disable_interrupts() {
    ime = false;
    set_ime = false;
}

void set_sime() {
    set_ime = true;
}

void sime_to_ime() {
    if (set_ime) {
        ime = true;
        set_ime = false;
    }
}

uint8_t handle_interrupt(uint16_t &pc, uint16_t &sp) {
    // tick_timer(1);
    uint8_t ie_ = read_byte(0xFFFF);
    uint8_t if_ = read_byte(0xFF0F);

    if (ie_ & if_) {
        halt = false;
    }

    if (!ime) {
        return 0;
    }

    if (!(ie_ & if_)) {
        return 0;
    }

    for (int i = 0; i < 5; i++) {
        if (ie_ & if_ & (1 << i)) {
            ime = false;
            write_byte(--sp, pc >> 8);
            // tick_timer(1);
            write_byte(--sp, pc & LO_8);
            // tick_timer(1);
            pc = I_JUMPS[i];
            // tick_timer(1);
            write_byte(0xFF0F, read_byte(0xFF0F) ^ (1 << i));
            // tick_timer(1);
            return 5;
        }
    }

    return 0;
}
