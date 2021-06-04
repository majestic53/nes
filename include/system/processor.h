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

#ifndef NES_PROCESSOR_H_
#define NES_PROCESSOR_H_

#include "../bus.h"

typedef union {

        struct {

                union {

                        struct {
                                uint8_t transfer : 1;
                                uint8_t non_maskable : 1;
                                uint8_t maskable : 1;
                        };

                        struct {
                                uint8_t carry : 1;
                                uint8_t zero : 1;
                                uint8_t interrupt_disabled : 1;
                                uint8_t decimal : 1;
                                uint8_t breakpoint : 2;
                                uint8_t overflow : 1;
                                uint8_t negative : 1;
                        };

                        uint8_t low;
                };

                uint8_t high;
        };

        uint16_t word;
} nes_processor_register_t;

typedef struct {
        nes_processor_register_t address;
        uint8_t opcode;

        struct {
                nes_processor_register_t address;
                nes_processor_register_t address_indirect;
                nes_processor_register_t data;
                bool page_boundary;
        } operand;
} nes_processor_fetch_t;

typedef struct {
        nes_processor_register_t source;
        nes_processor_register_t offset;
} nes_processor_transfer_t;

typedef struct {
        uint8_t cycles;
        nes_processor_fetch_t fetched;
        nes_processor_register_t pending;
        nes_processor_transfer_t transfer;
        nes_processor_register_t program_counter;
        nes_processor_register_t stack_pointer;
        nes_processor_register_t status;
        nes_processor_register_t accumulator;
        nes_processor_register_t index_x;
        nes_processor_register_t index_y;
} nes_processor_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nes_processor_interrupt(
        __inout nes_processor_t *processor,
        __in bool maskable
        );

void nes_processor_reset(
        __inout nes_processor_t *processor
        );

void nes_processor_step(
        __inout nes_processor_t *processor
        );

void nes_processor_transfer(
        __inout nes_processor_t *processor,
        __in uint8_t page
        );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_PROCESSOR_H_ */
