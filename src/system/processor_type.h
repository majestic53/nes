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
};

typedef struct {
        uint8_t cycles;
        int opcode;
        int mode;
} nes_processor_instruction_t;

static const nes_processor_instruction_t INSTRUCTION[] = {
        /* 0x00 */
        { 7, OPCODE_BRK, MODE_IMPLIED },
        { 6, OPCODE_ORA, MODE_INDIRECT_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_ORA, MODE_ZEROPAGE },
        { 5, OPCODE_ASL, MODE_ZEROPAGE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x08 */
        { 3, OPCODE_PHP, MODE_IMPLIED },
        { 2, OPCODE_ORA, MODE_IMMEDIATE },
        { 2, OPCODE_ASL, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_ORA, MODE_ABSOLUTE },
        { 6, OPCODE_ASL, MODE_ABSOLUTE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x10 */
        { 2, OPCODE_BPL, MODE_RELATIVE },
        { 5, OPCODE_ORA, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_ORA, MODE_ZEROPAGE_X },
        { 6, OPCODE_ASL, MODE_ZEROPAGE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x18 */
        { 2, OPCODE_CLC, MODE_IMPLIED },
        { 4, OPCODE_ORA, MODE_ABSOLUTE_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_ORA, MODE_ABSOLUTE_X },
        { 7, OPCODE_ASL, MODE_ABSOLUTE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x20 */
        { 6, OPCODE_JSR, MODE_ABSOLUTE },
        { 6, OPCODE_AND, MODE_INDIRECT_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_BIT, MODE_ZEROPAGE },
        { 3, OPCODE_AND, MODE_ZEROPAGE },
        { 5, OPCODE_ROL, MODE_ZEROPAGE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x28 */
        { 4, OPCODE_PLP, MODE_IMPLIED },
        { 2, OPCODE_AND, MODE_IMMEDIATE },
        { 2, OPCODE_ROL, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_BIT, MODE_ABSOLUTE },
        { 4, OPCODE_AND, MODE_ABSOLUTE },
        { 6, OPCODE_ROL, MODE_ABSOLUTE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x30 */
        { 2, OPCODE_BMI, MODE_RELATIVE },
        { 5, OPCODE_AND, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_AND, MODE_ZEROPAGE_X },
        { 6, OPCODE_ROL, MODE_ZEROPAGE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x38 */
        { 2, OPCODE_SEC, MODE_IMPLIED },
        { 4, OPCODE_AND, MODE_ABSOLUTE_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_AND, MODE_ABSOLUTE_X },
        { 7, OPCODE_ROL, MODE_ABSOLUTE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x40 */
        { 6, OPCODE_RTI, MODE_IMPLIED },
        { 6, OPCODE_EOR, MODE_INDIRECT_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_EOR, MODE_ZEROPAGE },
        { 5, OPCODE_LSR, MODE_ZEROPAGE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x48 */
        { 3, OPCODE_PHA, MODE_IMPLIED },
        { 2, OPCODE_EOR, MODE_IMMEDIATE },
        { 2, OPCODE_LSR, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_JMP, MODE_ABSOLUTE },
        { 4, OPCODE_EOR, MODE_ABSOLUTE },
        { 6, OPCODE_LSR, MODE_ABSOLUTE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x50 */
        { 2, OPCODE_BVC, MODE_RELATIVE },
        { 5, OPCODE_EOR, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_EOR, MODE_ZEROPAGE_X },
        { 6, OPCODE_LSR, MODE_ZEROPAGE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x58 */
        { 2, OPCODE_CLI, MODE_IMPLIED },
        { 4, OPCODE_EOR, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_EOR, MODE_ABSOLUTE_X },
        { 7, OPCODE_LSR, MODE_ABSOLUTE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x60 */
        { 6, OPCODE_RTS, MODE_IMPLIED },
        { 6, OPCODE_ADC, MODE_INDIRECT_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_ADC, MODE_ZEROPAGE },
        { 5, OPCODE_ROR, MODE_ZEROPAGE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x68 */
        { 4, OPCODE_PLA, MODE_IMPLIED },
        { 2, OPCODE_ADC, MODE_IMMEDIATE },
        { 2, OPCODE_ROR, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 5, OPCODE_JMP, MODE_INDIRECT },
        { 4, OPCODE_ADC, MODE_ABSOLUTE },
        { 6, OPCODE_ROR, MODE_ABSOLUTE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x70 */
        { 2, OPCODE_BVS, MODE_RELATIVE },
        { 5, OPCODE_ADC, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_ADC, MODE_ZEROPAGE_X },
        { 6, OPCODE_ROR, MODE_ZEROPAGE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x78 */
        { 2, OPCODE_SEI, MODE_IMPLIED },
        { 4, OPCODE_ADC, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_ADC, MODE_ABSOLUTE_X },
        { 7, OPCODE_ROR, MODE_ABSOLUTE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x80 */
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 6, OPCODE_STA, MODE_INDIRECT_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_STY, MODE_ZEROPAGE },
        { 3, OPCODE_STA, MODE_ZEROPAGE },
        { 3, OPCODE_STX, MODE_ZEROPAGE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x88 */
        { 2, OPCODE_DEY, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_TXA, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_STY, MODE_ABSOLUTE },
        { 4, OPCODE_STA, MODE_ABSOLUTE },
        { 4, OPCODE_STX, MODE_ABSOLUTE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x90 */
        { 2, OPCODE_BCC, MODE_RELATIVE },
        { 6, OPCODE_STA, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_STY, MODE_ZEROPAGE_X },
        { 4, OPCODE_STA, MODE_ZEROPAGE_X },
        { 4, OPCODE_STX, MODE_ZEROPAGE_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0x98 */
        { 2, OPCODE_TYA, MODE_IMPLIED },
        { 5, OPCODE_STA, MODE_ABSOLUTE_Y },
        { 2, OPCODE_TXS, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 5, OPCODE_STA, MODE_ABSOLUTE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xa0 */
        { 2, OPCODE_LDY, MODE_IMMEDIATE },
        { 6, OPCODE_LDA, MODE_INDIRECT_X },
        { 2, OPCODE_LDX, MODE_IMMEDIATE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_LDY, MODE_ZEROPAGE },
        { 3, OPCODE_LDA, MODE_ZEROPAGE },
        { 3, OPCODE_LDX, MODE_ZEROPAGE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xa8 */
        { 2, OPCODE_TAY, MODE_IMPLIED },
        { 2, OPCODE_LDA, MODE_IMMEDIATE },
        { 2, OPCODE_TAX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_LDY, MODE_ABSOLUTE },
        { 4, OPCODE_LDA, MODE_ABSOLUTE },
        { 4, OPCODE_LDX, MODE_ABSOLUTE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xb0 */
        { 2, OPCODE_BCS, MODE_RELATIVE },
        { 5, OPCODE_LDA, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_LDY, MODE_ZEROPAGE_X },
        { 4, OPCODE_LDA, MODE_ZEROPAGE_X },
        { 4, OPCODE_LDX, MODE_ZEROPAGE_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xb8 */
        { 2, OPCODE_CLV, MODE_IMPLIED },
        { 4, OPCODE_LDA, MODE_ABSOLUTE_Y },
        { 2, OPCODE_TSX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_LDY, MODE_ABSOLUTE_X },
        { 4, OPCODE_LDA, MODE_ABSOLUTE_X },
        { 4, OPCODE_LDX, MODE_ABSOLUTE_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xc0 */
        { 2, OPCODE_CPY, MODE_IMMEDIATE },
        { 6, OPCODE_CMP, MODE_INDIRECT_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_CPY, MODE_ZEROPAGE },
        { 3, OPCODE_CMP, MODE_ZEROPAGE },
        { 3, OPCODE_DEC, MODE_ZEROPAGE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xc8 */
        { 2, OPCODE_INY, MODE_IMPLIED },
        { 2, OPCODE_CMP, MODE_IMMEDIATE },
        { 2, OPCODE_DEX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_CPY, MODE_ABSOLUTE },
        { 4, OPCODE_CMP, MODE_ABSOLUTE },
        { 6, OPCODE_DEC, MODE_ABSOLUTE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xd0 */
        { 2, OPCODE_BNE, MODE_RELATIVE },
        { 5, OPCODE_CMP, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_CMP, MODE_ZEROPAGE_X },
        { 6, OPCODE_DEC, MODE_ZEROPAGE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xd8 */
        { 2, OPCODE_CLD, MODE_IMPLIED },
        { 4, OPCODE_CMP, MODE_ABSOLUTE_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_CMP, MODE_ABSOLUTE_X },
        { 7, OPCODE_DEC, MODE_ABSOLUTE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xe0 */
        { 2, OPCODE_CPX, MODE_IMMEDIATE },
        { 6, OPCODE_SBC, MODE_INDIRECT_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 3, OPCODE_CPX, MODE_ZEROPAGE },
        { 3, OPCODE_SBC, MODE_ZEROPAGE },
        { 5, OPCODE_INC, MODE_ZEROPAGE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xe8 */
        { 2, OPCODE_INX, MODE_IMPLIED },
        { 2, OPCODE_SBC, MODE_IMMEDIATE },
        { 2, OPCODE_NOP, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_CPX, MODE_ABSOLUTE },
        { 4, OPCODE_SBC, MODE_ABSOLUTE },
        { 6, OPCODE_INC, MODE_ABSOLUTE },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xf0 */
        { 2, OPCODE_BEQ, MODE_RELATIVE },
        { 5, OPCODE_SBC, MODE_INDIRECT_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_SBC, MODE_ZEROPAGE_X },
        { 6, OPCODE_INC, MODE_ZEROPAGE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        /* 0xf8 */
        { 2, OPCODE_SED, MODE_IMPLIED },
        { 4, OPCODE_SBC, MODE_ABSOLUTE_Y },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        { 4, OPCODE_SBC, MODE_ABSOLUTE_X },
        { 7, OPCODE_INC, MODE_ABSOLUTE_X },
        { 2, OPCODE_XXX, MODE_IMPLIED },
        };

typedef uint8_t (*nes_processor_instruction_hdlr)(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t nes_processor_execute_binary(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_bit(
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

uint8_t nes_processor_execute_set(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

uint8_t nes_processor_execute_transfer(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

static const nes_processor_instruction_hdlr HANDLER[] = {
        /* 0x00 */
        nes_processor_execute_breakpoint,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x08 */
        nes_processor_execute_push,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x10 */
        nes_processor_execute_branch,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x18 */
        nes_processor_execute_clear,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x20 */
        nes_processor_execute_jump,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_bit,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x28 */
        nes_processor_execute_pull,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_bit,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x30 */
        nes_processor_execute_branch,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x38 */
        nes_processor_execute_set,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x40 */
        nes_processor_execute_return,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x48 */
        nes_processor_execute_push,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_jump,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x50 */
        nes_processor_execute_branch,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x58 */
        nes_processor_execute_clear,
        nes_processor_execute_binary,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_binary,
        NULL,
        nes_processor_execute_illegal,
        /* 0x60 */
        nes_processor_execute_return,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0x68 */
        nes_processor_execute_pull,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_jump,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0x70 */
        nes_processor_execute_branch,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0x78 */
        nes_processor_execute_set,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0x80 */
        nes_processor_execute_illegal,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0x88 */
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        nes_processor_execute_transfer,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0x90 */
        nes_processor_execute_branch,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0x98 */
        nes_processor_execute_transfer,
        NULL,
        nes_processor_execute_transfer,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        /* 0xa0 */
        NULL,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0xa8 */
        nes_processor_execute_transfer,
        NULL,
        nes_processor_execute_transfer,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0xb0 */
        nes_processor_execute_branch,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0xb8 */
        nes_processor_execute_clear,
        NULL,
        nes_processor_execute_transfer,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        NULL,
        nes_processor_execute_illegal,
        /* 0xc0 */
        NULL,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        /* 0xc8 */
        nes_processor_execute_increment,
        NULL,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        /* 0xd0 */
        nes_processor_execute_branch,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        /* 0xd8 */
        nes_processor_execute_clear,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        nes_processor_execute_decrement,
        nes_processor_execute_illegal,
        /* 0xe0 */
        NULL,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        nes_processor_execute_increment,
        nes_processor_execute_illegal,
        /* 0xe8 */
        nes_processor_execute_increment,
        NULL,
        nes_processor_execute_no_operation,
        nes_processor_execute_illegal,
        NULL,
        NULL,
        nes_processor_execute_increment,
        nes_processor_execute_illegal,
        /* 0xf0 */
        nes_processor_execute_branch,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
        nes_processor_execute_increment,
        nes_processor_execute_illegal,
        /* 0xf8 */
        nes_processor_execute_set,
        NULL,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        nes_processor_execute_illegal,
        NULL,
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

void nes_processor_instruction(
        __inout nes_processor_t *processor
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

#ifndef NDEBUG

void nes_processor_trace(
        __in int level,
        __in const nes_processor_t *processor
        );

void nes_processor_trace_instruction(
        __in int level,
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        );

#define TRACE_PROCESSOR(_LEVEL_, _PROCESSOR_) \
        if((_LEVEL_) <= (LEVEL)) nes_processor_trace(_LEVEL_, _PROCESSOR_)
#define TRACE_PROCESSOR_INSTRUCTION(_LEVEL_, _PROCESSOR_, _INSTRUCTION_) \
        if((_LEVEL_) <= (LEVEL)) nes_processor_trace_instruction(_LEVEL_, _PROCESSOR_, _INSTRUCTION_)
#else
#define TRACE_PROCESSOR(_LEVEL_, _PROCESSOR_)
#define TRACE_PROCESSOR_INSTRUCTION(_LEVEL_, _PROCESSOR_, _INSTRUCTION_)
#endif /* NDEBUG */

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
