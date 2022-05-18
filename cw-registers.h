// 
// CW-REGISTERS.h
// Created: 1th April 2022
// Created by: Gijs Burghoorn (@coastalwhite)
//
// Provides functions to read, capture and send back the register values of the
// CWLITE-ARM. This attempts to have as little changes of the registers as
// possible while reading them.
// 

#ifndef __CW_REGISTERS_H__
#define __CW_REGISTERS_H__

#ifndef CW_REGISTERS_NUM_BUFFERS
/// The number of buffers available for registers
#define CW_REGISTERS_NUM_BUFFERS 2
#endif

/// The size of a register in bytes
#define REGSIZE sizeof(uint32_t)
/// The number of registers captured (per buffer)
#define CW_REGISTERS_NUM_REGS 14
// #define CW_REGISTERS_NUM_REGS 15

#define CW_DATA_SIZE (1+REGSIZE*CW_REGISTERS_NUM_REGS)

#include "simpleserial.h"

#include <stdint.h>
#include <stdlib.h>

// All the registers available on the Cortex M4
// --- General Purpose ---
register uint32_t CW_REG0 __asm("r0");
register uint32_t CW_REG1 __asm("r1");
register uint32_t CW_REG2 __asm("r2");
register uint32_t CW_REG3 __asm("r3");
register uint32_t CW_REG4 __asm("r4");
register uint32_t CW_REG5 __asm("r5");
register uint32_t CW_REG6 __asm("r6");
register uint32_t CW_REG7 __asm("r7");
register uint32_t CW_REG8 __asm("r8");
register uint32_t CW_REG9 __asm("r9");
register uint32_t CW_REG10 __asm("r10");
register uint32_t CW_REG11 __asm("r11");
register uint32_t CW_REG12 __asm("r12");
// --- End General Purpose ---
register uint32_t CW_REG13 __asm("r13"); // Stack Pointer
register uint32_t CW_REG14 __asm("r14"); // Link Register
// register int CW_REG15 __asm("r15"); // Program Counter

static uint32_t register_buffers[CW_REGISTERS_NUM_REGS * CW_REGISTERS_NUM_BUFFERS];

/// Capture all registers into `buffer_num`.
#define capture_registers(buffer_num)                                \
    register_buffers[0 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG0; \
    register_buffers[1 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG1; \
    register_buffers[2 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG2; \
    register_buffers[3 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG3; \
    register_buffers[4 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG4; \
    register_buffers[5 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG5; \
    register_buffers[6 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG6; \
    register_buffers[7 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG7; \
    register_buffers[8 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG8; \
    register_buffers[9 + buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG9; \
    register_buffers[10+ buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG10;\
    register_buffers[11+ buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG11;\
    register_buffers[12+ buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG12;\
    register_buffers[13+ buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG13;\
    register_buffers[14+ buffer_num * CW_REGISTERS_NUM_REGS] = CW_REG14;

/// Send the captured registers in buffer `buffer_num` back to the Scope.
/// 
/// This will send 1 byte with the buffer_num followed by
/// `CW_REGISTERS_NUM_REGS` number of register values in Big-Endian. Over the `r`
/// command.
void put_captured_registers(const uint8_t buffer_num) {
    uint8_t result_buffer[CW_DATA_SIZE];

	result_buffer[0] = buffer_num;

    // Loop over all registers
    for (uint8_t reg_i = 0; reg_i < CW_REGISTERS_NUM_REGS; reg_i++) {
        uint16_t src_index = reg_i + buffer_num * CW_REGISTERS_NUM_REGS;
        uint32_t src = register_buffers[src_index];

		// Put the register in the buffer in big-endian
        for (uint8_t byte_i = 0; byte_i < REGSIZE; byte_i++) {
            uint16_t dst_index = reg_i * REGSIZE + (REGSIZE - byte_i - 1);
            result_buffer[dst_index + 1] = (uint8_t) (src & 0xFF);
            src >>= 8;
        }
    }

    simpleserial_put('r', CW_DATA_SIZE, result_buffer);
}

#endif // __CW_REGISTERS_H__
