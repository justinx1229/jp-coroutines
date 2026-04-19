#include "memory.h"
#include "cpu.h"

// pc, sp
uint16_t pc = 0;
uint16_t sp = 0;

// regs
uint16_t af = 0;
uint16_t bc = 0;
uint16_t de = 0;
uint16_t hl = 0;

uint8_t get_reg8(uint8_t r) {
    switch (r) {
        case 0:
            // b
            return (bc >> 8);
        case 1:
            // c
            return bc & LO_8;
        case 2: 
            // d
            return (de >> 8);
        case 3: 
            // e
            return de & LO_8;
        case 4:
            // h
            return (hl >> 8);
        case 5:
            // l
            return hl & LO_8;
        case 6:
            // [hl]
            return read_byte(hl);
        case 7:
            // a
            return (af >> 8);
    }

    return 0;
}

uint16 get_16mem(uint8_t r) {
    switch (r) {
        case 0: 
            return bc;
        case 1:
            return de;
        case 2:
            return hl++;
        case 3:
            return hl--;
    }
    
    return 0;
}

void set_reg16(uint8_t r, uint16_t val) {
    switch (r) {
        case 0: 
            bc = val;
            break;
        case 1:
            de = val;
            break;
        case 2: 
            hl = val;
            break;
        case 3: 
            sp = val;
            break;
    }
}

void run_cb() {

}

void run00(uint8_t byte) {
    switch (byte & LO_4) {
        case 1: {
            uint16_t imm16 = (((uint16_t)read_byte(++pc)) << 8) | read_byte(++pc);
            set_reg16((byte >> 4) & LO_2, imm16);
            break;
        }
        case 2:
            write16(get_16mem((byte >> 4) & LO_2), get_reg8(7));
            break;
        case 3: 
            set_reg8(7, read_byte(get_16mem((byte >> 4) & LO_2)));
            
    }
}

void run() {
    uint8_t byte = read_byte(pc);   
    pc++;
    switch (byte) {
        case 0:
            return;
            break;
        case CB: 
            run_cb();
            break;
        default:
            // casework on first 2 bits
            switch (byte >> 6) {
                case 0:
                    run00(byte);
                    break;
                case 1:
                    // run01(byte); 
                    break;
                case 2: 
                    // run10(byte);
                    break;
                case 3:
                    // run11(byte);
                    break;
            }
            break;
    }
}