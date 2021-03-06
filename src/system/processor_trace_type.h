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

#ifndef NES_PROCESSOR_TRACE_TYPE_H_
#define NES_PROCESSOR_TRACE_TYPE_H_

#include "./processor_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_PROCESSOR_TRACE_TYPE_H_ */
