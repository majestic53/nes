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

#include "./action_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_action(
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result;

        if(!request) {
                result = ERROR(NES_ERR, "invalid request -- %p", request);
                goto exit;
        }

        if(request->type >= NES_ACTION_MAX) {
                result = ERROR(NES_ERR, "invalid request type -- %i", request->type);
                goto exit;
        }

        result = HANDLER[request->type](nes_bus(), request, response);

exit:
        return result;
}

int
nes_action_bus_read(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result = NES_OK;

        if(!response) {
                result = ERROR(NES_ERR, "invalid response -- %p", response);
                goto exit;
        }

        response->type = request->type;
        response->address = request->address;
        response->data = nes_bus_read(BUS_PROCESSOR, response->address);
        TRACE(LEVEL_VERBOSE, "Bus read [%04x]->%02x", response->address, response->data & UINT8_MAX);

exit:
        return result;
}

int
nes_action_bus_write(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result = NES_OK;

        nes_bus_write(BUS_PROCESSOR, request->address, request->data);
        TRACE(LEVEL_VERBOSE, "Bus write [%04x]<-%02x", request->address, request->data & UINT8_MAX);

        return result;
}

int
nes_action_processor_read(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result = NES_OK;

        if(!response) {
                result = ERROR(NES_ERR, "invalid response -- %p", response);
                goto exit;
        }

        response->type = request->type;
        response->address = request->address;

        switch(response->address) {
                case NES_PROCESSOR_PROGRAM_COUNTER:
                        response->data = bus->processor.program_counter.word;
                        TRACE(LEVEL_VERBOSE, "Processor read [PC]->%04x", response->data);
                        break;
                case NES_PROCESSOR_STACK_POINTER:
                        response->data = bus->processor.stack_pointer.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [SP]->%02x", response->data & UINT8_MAX);
                        break;
                case NES_PROCESSOR_STATUS:
                        response->data = bus->processor.status.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [S]->%02x", response->data & UINT8_MAX);
                        break;
                case NES_PROCESSOR_ACCUMULATOR:
                        response->data = bus->processor.accumulator.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [A]->%02x", response->data & UINT8_MAX);
                        break;
                case NES_PROCESSOR_INDEX_X:
                        response->data = bus->processor.index_x.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [X]->%02x", response->data & UINT8_MAX);
                        break;
                case NES_PROCESSOR_INDEX_Y:
                        response->data = bus->processor.index_y.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [Y]->%02x", response->data & UINT8_MAX);
                        break;
                default:
                        result = ERROR(NES_ERR, "invalid address -- %i", response->address);
                        goto exit;
        }

exit:
        return result;
}

int
nes_action_processor_write(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result = NES_OK;

        switch(request->address) {
                case NES_PROCESSOR_PROGRAM_COUNTER:
                        bus->processor.program_counter.word = request->data;
                        TRACE(LEVEL_VERBOSE, "Processor write [PC]<-%04x", request->data);
                        break;
                case NES_PROCESSOR_STACK_POINTER:
                        bus->processor.stack_pointer.low = request->data;
                        TRACE(LEVEL_VERBOSE, "Processor write [SP]<-%02x", request->data & UINT8_MAX);
                        break;
                case NES_PROCESSOR_STATUS:
                        bus->processor.status.low = request->data;
                        TRACE(LEVEL_VERBOSE, "Processor write [S]<-%02x", request->data & UINT8_MAX);
                        break;
                case NES_PROCESSOR_ACCUMULATOR:
                        bus->processor.accumulator.low = request->data;
                        TRACE(LEVEL_VERBOSE, "Processor write [A]<-%02x", request->data & UINT8_MAX);
                        break;
                case NES_PROCESSOR_INDEX_X:
                        bus->processor.index_x.low = request->data;
                        TRACE(LEVEL_VERBOSE, "Processor write [X]<-%02x", request->data & UINT8_MAX);
                        break;
                case NES_PROCESSOR_INDEX_Y:
                        bus->processor.index_y.low = request->data;
                        TRACE(LEVEL_VERBOSE, "Processor write [Y]<-%02x", request->data & UINT8_MAX);
                        break;
                default:
                        result = ERROR(NES_ERR, "invalid address -- %i", request->address);
                        goto exit;
        }

exit:
        return result;
}

int
nes_action_run(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result = NES_OK;

        TRACE(LEVEL_INFORMATION, "%s", "Emulation running");

        for(;;) {
                uint16_t cycle = 0;

                if(nes_service_poll() != NES_OK) {
                        result = (result == NES_EVT) ? NES_OK : result;
                        break;
                }

                /* TODO: STEP SUBSYSTEMS */
                do {
                        nes_processor_step(&bus->processor);

                        /* TODO */

                        TRACE_STEP();
                } while(cycle++ < CYCLES_PER_FRAME);
                /* --- */

                if((result = nes_service_show()) != NES_OK) {
                        break;
                }
        }

        TRACE(LEVEL_INFORMATION, "%s", "Emulation paused");

        return result;
}

int
nes_action_step(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result = NES_OK;

        TRACE(LEVEL_INFORMATION, "%s", "Emulation stepping");

        if(nes_service_poll() != NES_OK) {
                result = (result == NES_EVT) ? NES_OK : result;
                goto exit;
        }

        nes_processor_step(&bus->processor);

        /* TODO: STEP SUBSYSTEMS */

        TRACE_STEP();

        if((result = nes_service_show()) != NES_OK) {
                goto exit;
        }

exit:
        TRACE(LEVEL_INFORMATION, "%s", "Emulation paused");

        return result;
}

#ifdef __cplusplus
extern }
#endif /* __cplusplus */
