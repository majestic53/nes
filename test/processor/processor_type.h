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

typedef struct {
        nes_buffer_t memory;
        nes_processor_t processor;
} nes_test_processor_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_test_processor_execute_break(void);

int nes_test_processor_execute_clear(void);

int nes_test_processor_execute_decrement(void);

int nes_test_processor_execute_illegal(void);

int nes_test_processor_execute_increment(void);

int nes_test_processor_execute_no_operation(void);

int nes_test_processor_execute_pull(void);

int nes_test_processor_execute_push(void);

int nes_test_processor_execute_return(void);

int nes_test_processor_execute_set(void);

int nes_test_processor_execute_transfer(void);

int nes_test_processor_interrupt_maskable(void);

int nes_test_processor_interrupt_non_maskable(void);

int nes_test_processor_reset(void);

void nes_test_initialize(void);

void nes_test_uninitialize(void);

static const nes_test TEST[] = {
        nes_test_processor_execute_break,
        nes_test_processor_execute_clear,
        nes_test_processor_execute_decrement,
        nes_test_processor_execute_illegal,
        nes_test_processor_execute_increment,
        nes_test_processor_execute_no_operation,
        nes_test_processor_execute_pull,
        nes_test_processor_execute_push,
        nes_test_processor_execute_return,
        nes_test_processor_execute_set,
        nes_test_processor_execute_transfer,
        nes_test_processor_interrupt_maskable,
        nes_test_processor_interrupt_non_maskable,
	nes_test_processor_reset,
	};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_TEST_PROCESSOR_TYPE_H_ */
