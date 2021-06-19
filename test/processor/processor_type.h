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

#ifndef NES_TEST_PROCESSOR_TYPE_H_
#define NES_TEST_PROCESSOR_TYPE_H_

#include "../../src/system/processor_type.h"
#include "../common.h"

#define TRIALS 1

static const uint8_t ILLEGAL_OPCODE[] = {
        0x02, 0x03, 0x04, 0x07, 0x0b, 0x0c, 0x0f,
        0x12, 0x13, 0x14, 0x17, 0x1a, 0x1b, 0x1c, 0x1f,
        0x22, 0x23, 0x27, 0x2b, 0x2f,
        0x32, 0x33, 0x34, 0x37, 0x3a, 0x3b, 0x3c, 0x3f,
        0x42, 0x43, 0x44, 0x47, 0x4b, 0x4f,
        0x52, 0x53, 0x54, 0x57, 0x5a, 0x5b, 0x5c, 0x5f,
        0x62, 0x63, 0x64, 0x67, 0x6b, 0x6f,
        0x72, 0x73, 0x74, 0x77, 0x7a, 0x7b, 0x7c, 0x7f,
        0x82, 0x83, 0x87, 0x89, 0x8b, 0x8f,
        0x92, 0x93, 0x97, 0x9b, 0x9c, 0x9e, 0x9f,
        0xa3, 0xa7, 0xab, 0xaf,
        0xb2, 0xb3, 0xb7, 0xbb, 0xbf,
        0xc2, 0xc3, 0xc7, 0xcb, 0xcf,
        0xd2, 0xd3, 0xd4, 0xd7, 0xda, 0xdb, 0xdc, 0xdf,
        0xe2, 0xe3, 0xe7, 0xeb, 0xef,
        0xf2, 0xf3, 0xf4, 0xf7, 0xfa, 0xfb, 0xfc, 0xff,
        };

typedef struct {
        nes_buffer_t memory;
        nes_processor_t processor;
} nes_test_processor_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_test_processor_execute_arithmetic(void);

int nes_test_processor_execute_bit(void);

int nes_test_processor_execute_bitwise(void);

int nes_test_processor_execute_branch(void);

int nes_test_processor_execute_breakpoint(void);

int nes_test_processor_execute_clear(void);

int nes_test_processor_execute_compare(void);

int nes_test_processor_execute_decrement(void);

int nes_test_processor_execute_illegal(void);

int nes_test_processor_execute_increment(void);

int nes_test_processor_execute_jump(void);

int nes_test_processor_execute_load(void);

int nes_test_processor_execute_no_operation(void);

int nes_test_processor_execute_pull(void);

int nes_test_processor_execute_push(void);

int nes_test_processor_execute_return(void);

int nes_test_processor_execute_rotate(void);

int nes_test_processor_execute_set(void);

int nes_test_processor_execute_shift(void);

int nes_test_processor_execute_store(void);

int nes_test_processor_execute_transfer(void);

int nes_test_processor_fetch_absolute(void);

int nes_test_processor_fetch_absolute_x(void);

int nes_test_processor_fetch_absolute_y(void);

int nes_test_processor_fetch_immediate(void);

int nes_test_processor_fetch_implied(void);

int nes_test_processor_fetch_indirect(void);

int nes_test_processor_fetch_indirect_x(void);

int nes_test_processor_fetch_indirect_y(void);

int nes_test_processor_fetch_relative(void);

int nes_test_processor_fetch_zeropage(void);

int nes_test_processor_fetch_zeropage_x(void);

int nes_test_processor_fetch_zeropage_y(void);

int nes_test_processor_interrupt_maskable(void);

int nes_test_processor_interrupt_non_maskable(void);

int nes_test_processor_reset(void);

int nes_test_processor_transfer(void);

void nes_test_initialize(void);

static const nes_test TEST[] = {
        nes_test_processor_execute_arithmetic,
        nes_test_processor_execute_bit,
        nes_test_processor_execute_bitwise,
        nes_test_processor_execute_branch,
        nes_test_processor_execute_breakpoint,
        nes_test_processor_execute_clear,
        nes_test_processor_execute_compare,
        nes_test_processor_execute_decrement,
        nes_test_processor_execute_illegal,
        nes_test_processor_execute_increment,
        nes_test_processor_execute_jump,
        nes_test_processor_execute_load,
        nes_test_processor_execute_no_operation,
        nes_test_processor_execute_pull,
        nes_test_processor_execute_push,
        nes_test_processor_execute_return,
        nes_test_processor_execute_rotate,
        nes_test_processor_execute_set,
        nes_test_processor_execute_shift,
        nes_test_processor_execute_store,
        nes_test_processor_execute_transfer,
        nes_test_processor_fetch_absolute,
        nes_test_processor_fetch_absolute_x,
        nes_test_processor_fetch_absolute_y,
        nes_test_processor_fetch_immediate,
        nes_test_processor_fetch_implied,
        nes_test_processor_fetch_indirect,
        nes_test_processor_fetch_indirect_x,
        nes_test_processor_fetch_indirect_y,
        nes_test_processor_fetch_relative,
        nes_test_processor_fetch_zeropage,
        nes_test_processor_fetch_zeropage_x,
        nes_test_processor_fetch_zeropage_y,
        nes_test_processor_interrupt_maskable,
        nes_test_processor_interrupt_non_maskable,
	nes_test_processor_reset,
        nes_test_processor_transfer,
	};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_TEST_PROCESSOR_TYPE_H_ */
