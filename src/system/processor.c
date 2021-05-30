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

#include "./processor_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void
nes_processor_interrupt(
        __inout nes_processor_t *processor,
        __in bool maskable
        )
{
        /* TODO */
}

void
nes_processor_reset(
        __inout nes_processor_t *processor
        )
{
        /* TODO */
}

void
nes_processor_step(
        __inout nes_processor_t *processor
        )
{
        /* TODO */
}

void
nes_processor_transfer(
        __inout nes_processor_t *processor,
        __in uint8_t page
        )
{
        /* TODO */
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
