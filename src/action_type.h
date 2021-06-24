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

#ifndef NES_ACTION_TYPE_H_
#define NES_ACTION_TYPE_H_

#include "./bus_type.h"

typedef int (*nes_action_hdlr)(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        );

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_action_bus_read(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        );

int nes_action_bus_write(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        );

int nes_action_processor_read(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        );

int nes_action_processor_write(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        );

int nes_action_run(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        );

int nes_action_step(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        );

static const nes_action_hdlr ACTION_HDLR[] = {
        nes_action_run, /* NES_ACTION_RUN */
        nes_action_step, /* NES_ACTION_STEP */
        nes_action_bus_read, /* NES_ACTION_BUS_READ */
        nes_action_bus_write, /* NES_ACTION_BUS_WRITE */
        nes_action_processor_read, /* NES_ACTION_PROCESSOR_READ */
        nes_action_processor_write, /* NES_ACTION_PROCESSOR_WRITE */
        };

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_ACTION_TYPE_H_ */
