#include "ppu.h"

uint32_t dots = 0;

enum class Mode {
    OAM,
    DRAW,
    HBLANK,
    VBLANK
};

Mode mode = Mode::OAM;

void f_lyc() {
    if (ly == lyc) {
        write_byte(0xFF41, read_byte(0xFF41) | (1 << 2));
        if (read_byte(0xFF41) & (1 << 6)) {
            write_byte(0xFF0F, read_byte(0xFF0F) | (1 << 1));
        }
    }   
}

void compute_mode() {
    if (ly < 144) {
        if (dots <= 80) {
            mode = Mode::OAM;
        }
        else if (dots <= 252) {
            mode = Mode::DRAW;
        }
        else {
            mode = Mode::HBLANK;
        }
    }

    if (ly == 144) {
        mode = Mode::VBLANK;
        // vblank interrupt
        write_byte(0xFF0F, read_byte(0xFF0F) | 1);
    }
    else if (ly > 153) {
        mode = Mode::OAM;
        ly = 0;
        f_lyc();
    }
}

void run_ppu(uint32_t m_cycles) {
    uint32_t t_cycles = 4 * m_cycles;
    // add dots
    dots += t_cycles;
    
    // if dots is too big, start over. 
    if (dots >= 456) {
        dots -= 456;
        ly++;
        f_lyc(); 
    }

    compute_mode();

    uint8_t value;
    switch (mode) {
        case Mode::OAM: {
            value = 2;
            break;
        }
        case Mode::DRAW: {
            value = 3;
            break;
        }
        case Mode::HBLANK: {
            value = 0;
            break;
        }
        case Mode::VBLANK: {
            value = 1;
            break;
        }
    }
    
    // update STAT
    write_byte(0xFF41, (read_byte(0xFF41) & (LO_8 ^ LO_2)) | value);

}