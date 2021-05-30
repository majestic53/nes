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
        BREAKPOINT_INTERRUPT = 2,
        BREAKPOINT_INSTRUCTION,
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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

#define TRACE_PROCESSOR(_LEVEL_, _PROCESSOR_) \
        if((_LEVEL_) <= (LEVEL)) nes_processor_trace(_LEVEL_, _PROCESSOR_)
#else
#define TRACE_PROCESSOR(_LEVEL_, _PROCESSOR_)
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
