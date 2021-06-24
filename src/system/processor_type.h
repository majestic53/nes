/**
 * NES
 * Copyright (C) 2021 David Jolly
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef NES_PROCESSOR_TYPE_H_
#define NES_PROCESSOR_TYPE_H_

#include "../../include/system/processor.h"

#define MASKABLE_ADDRESS 0xfffe
#define MASKABLE_CYCLES 7

#define NON_MASKABLE_ADDRESS 0xfffa
#define NON_MASKABLE_CYCLES 7

#define PAGE_WIDTH 0x0100

#define RESET_ADDRESS 0xfffc
#define RESET_CYCLES 7

#define STACK_ADDRESS 0x0100

#define TRANSFER_CYCLES 2

enum {
        BREAKPOINT_CLEAR = 2,
        BREAKPOINT_SET,
};

enum {
        OPCODE_ADC = 0,
        OPCODE_AND,
        OPCODE_ASL,
        OPCODE_BCC,
        OPCODE_BCS,
        OPCODE_BEQ,
        OPCODE_BIT,
        OPCODE_BMI,
        OPCODE_BNE,
        OPCODE_BPL,
        OPCODE_BRK,
        OPCODE_BVC,
        OPCODE_BVS,
        OPCODE_CLC,
        OPCODE_CLD,
        OPCODE_CLI,
        OPCODE_CLV,
        OPCODE_CMP,
        OPCODE_CPX,
        OPCODE_CPY,
        OPCODE_DEC,
        OPCODE_DEX,
        OPCODE_DEY,
        OPCODE_EOR,
        OPCODE_INC,
        OPCODE_INX,
        OPCODE_INY,
        OPCODE_JMP,
        OPCODE_JSR,
        OPCODE_LDA,
        OPCODE_LDX,
        OPCODE_LDY,
        OPCODE_LSR,
        OPCODE_NOP,
        OPCODE_ORA,
        OPCODE_PHA,
        OPCODE_PHP,
        OPCODE_PLA,
        OPCODE_PLP,
        OPCODE_ROL,
        OPCODE_ROR,
        OPCODE_RTI,
        OPCODE_RTS,
        OPCODE_SBC,
        OPCODE_SEC,
        OPCODE_SED,
        OPCODE_SEI,
        OPCODE_STA,
        OPCODE_STX,
        OPCODE_STY,
        OPCODE_TAX,
        OPCODE_TAY,
        OPCODE_TSX,
        OPCODE_TXA,
        OPCODE_TXS,
        OPCODE_TYA,
        OPCODE_XXX,
        OPCODE_MAX,
};

enum {
        MODE_ABSOLUTE = 0,
        MODE_ABSOLUTE_X,
        MODE_ABSOLUTE_Y,
        MODE_IMMEDIATE,
        MODE_IMPLIED,
        MODE_INDIRECT,
        MODE_INDIRECT_X,
        MODE_INDIRECT_Y,
        MODE_RELATIVE,
        MODE_ZEROPAGE,
        MODE_ZEROPAGE_X,
        MODE_ZEROPAGE_Y,
        MODE_MAX,
};

typedef struct {
        uint8_t cycles;
        int opcode;
        int mode;
} nes_processor_instruction_t;

typedef uint8_t (*nes_processor_instruction_hdlr)(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nes_processor_execute(
        __inout nes_processor_t *processor
        );

uint8_t nes_processor_execute_arithmetic(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_bit(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_bitwise(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_branch(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_breakpoint(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_clear(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_compare(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_decrement(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_illegal(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_increment(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_jump(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_load(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_no_operation(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_pull(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_push(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_return(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_rotate(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_set(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_shift(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_store(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_transfer(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

static const nes_processor_instruction_hdlr INSTRUCTION_HDLR[] = {
        /* 0x00 */
        nes_processor_execute_breakpoint,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        /* 0x08 */
        nes_processor_execute_push,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        /* 0x10 */
        nes_processor_execute_branch,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        /* 0x18 */
        nes_processor_execute_clear,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        /* 0x20 */
        nes_processor_execute_jump,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bit,
        nes_processor_execute_bitwise,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        /* 0x28 */
        nes_processor_execute_pull,
        nes_processor_execute_bitwise,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        nes_processor_execute_bit,
        nes_processor_execute_bitwise,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        /* 0x30 */
        nes_processor_execute_branch,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        /* 0x38 */
        nes_processor_execute_set,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        /* 0x40 */
        nes_processor_execute_return,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        /* 0x48 */
        nes_processor_execute_push,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        nes_processor_execute_jump,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        /* 0x50 */
        nes_processor_execute_branch,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        /* 0x58 */
        nes_processor_execute_clear,
        nes_processor_execute_bitwise,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bitwise,
        nes_processor_execute_shift,
        nes_processor_execute_illegal,
        /* 0x60 */
        nes_processor_execute_return,
        nes_processor_execute_arithmetic,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_arithmetic,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        /* 0x68 */
        nes_processor_execute_pull,
        nes_processor_execute_arithmetic,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        nes_processor_execute_jump,
        nes_processor_execute_arithmetic,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        /* 0x70 */
        nes_processor_execute_branch,
        nes_processor_execute_arithmetic,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_arithmetic,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        /* 0x78 */
        nes_processor_execute_set,
        nes_processor_execute_arithmetic,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_arithmetic,
        nes_processor_execute_rotate,
        nes_processor_execute_illegal,
        /* 0x80 */
        nes_processor_execute_illegal,
        nes_processor_execute_store,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_store,
        nes_processor_execute_store,
        nes_processor_execute_store,
        nes_processor_execute_illegal,
        /* 0x88 */
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        nes_processor_execute_transfer,
        nes_processor_execute_illegal,
        nes_processor_execute_store,
        nes_processor_execute_store,
        nes_processor_execute_store,
        nes_processor_execute_illegal,
        /* 0x90 */
        nes_processor_execute_branch,
        nes_processor_execute_store,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_store,
        nes_processor_execute_store,
        nes_processor_execute_store,
        nes_processor_execute_illegal,
        /* 0x98 */
        nes_processor_execute_transfer,
        nes_processor_execute_store,
        nes_processor_execute_transfer,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_store,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        /* 0xa0 */
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_illegal,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_illegal,
        /* 0xa8 */
        nes_processor_execute_transfer,
        nes_processor_execute_load,
        nes_processor_execute_transfer,
        nes_processor_execute_illegal,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_illegal,
        /* 0xb0 */
        nes_processor_execute_branch,
        nes_processor_execute_load,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_illegal,
        /* 0xb8 */
        nes_processor_execute_clear,
        nes_processor_execute_load,
        nes_processor_execute_transfer,
        nes_processor_execute_illegal,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_load,
        nes_processor_execute_illegal,
        /* 0xc0 */
        nes_processor_execute_compare,
        nes_processor_execute_compare,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_compare,
        nes_processor_execute_compare,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        /* 0xc8 */
        nes_processor_execute_increment,
        nes_processor_execute_compare,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        nes_processor_execute_compare,
        nes_processor_execute_compare,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        /* 0xd0 */
        nes_processor_execute_branch,
        nes_processor_execute_compare,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_compare,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        /* 0xd8 */
        nes_processor_execute_clear,
        nes_processor_execute_compare,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_compare,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        /* 0xe0 */
        nes_processor_execute_compare,
        nes_processor_execute_arithmetic,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_compare,
        nes_processor_execute_arithmetic,
        nes_processor_execute_increment,
        nes_processor_execute_illegal,
        /* 0xe8 */
        nes_processor_execute_increment,
        nes_processor_execute_arithmetic,
        nes_processor_execute_no_operation,
        nes_processor_execute_illegal,
        nes_processor_execute_compare,
        nes_processor_execute_arithmetic,
        nes_processor_execute_increment,
        nes_processor_execute_illegal,
        /* 0xf0 */
        nes_processor_execute_branch,
        nes_processor_execute_arithmetic,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_arithmetic,
        nes_processor_execute_increment,
        nes_processor_execute_illegal,
        /* 0xf8 */
        nes_processor_execute_set,
        nes_processor_execute_arithmetic,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_arithmetic,
        nes_processor_execute_increment,
        nes_processor_execute_illegal,
        };

uint8_t nes_processor_fetch(
        __inout nes_processor_t *processor
        );

void nes_processor_fetch_operand(
        __inout nes_processor_t *processor,
        __in int mode
        );

uint16_t nes_processor_fetch_word(
        __inout nes_processor_t *processor
        );

const nes_processor_instruction_t *nes_processor_instruction(
        __in uint8_t opcode
        );

void nes_processor_interrupt_maskable(
        __inout nes_processor_t *processor
        );

void nes_processor_interrupt_non_maskable(
        __inout nes_processor_t *processor
        );

uint8_t nes_processor_pull(
        __inout nes_processor_t *processor
        );

uint16_t nes_processor_pull_word(
        __inout nes_processor_t *processor
        );

void nes_processor_push(
        __inout nes_processor_t *processor,
        __in uint8_t data
        );

void nes_processor_push_word(
        __inout nes_processor_t *processor,
        __in uint16_t data
        );

uint8_t nes_processor_read(
        __in const nes_processor_t *processor,
        __in uint16_t address
        );

uint16_t nes_processor_read_word(
        __in const nes_processor_t *processor,
        __in uint16_t address
        );

void nes_processor_transfer_byte(
        __inout nes_processor_t *processor
        );

void nes_processor_write(
        __inout nes_processor_t *processor,
        __in uint16_t address,
        __in uint8_t data
        );

void nes_processor_write_word(
        __inout nes_processor_t *processor,
        __in uint16_t address,
        __in uint16_t data
        );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_PROCESSOR_H_ */
