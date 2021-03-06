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
        nes_bus_t *bus;

        if(!request) {
                result = ERROR(NES_ERR, "invalid request -- %p", request);
                goto exit;
        }

        if(request->type >= NES_ACTION_MAX) {
                result = ERROR(NES_ERR, "invalid request type -- %i", request->type);
                goto exit;
        }

        if(!(bus = nes_bus())->loaded) {
                result = ERROR(NES_ERR, "%s", "invalid bus state");
                goto exit;
        }

        result = ACTION_HDLR[request->type](bus, request, response);

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
        response->address.word = request->address.word;
        response->data.low = nes_bus_read(BUS_PROCESSOR, response->address.word);
        TRACE(LEVEL_VERBOSE, "Bus read [%04X]->%02X", response->address.word, response->data.low);

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

        nes_bus_write(BUS_PROCESSOR, request->address.word, request->data.low);
        TRACE(LEVEL_VERBOSE, "Bus write [%04X]<-%02X", request->address.word, nes_bus_read(BUS_PROCESSOR, request->address.word));

        return result;
}

int
nes_action_cartridge_header(
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

        response->ptr = bus->mapper.cartridge.header;
        response->type = request->type;
        TRACE(LEVEL_VERBOSE, "Cartridge header %p", response->ptr);

exit:
        return result;
}

int
nes_action_mapper_read(
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
        response->address.word = request->address.word;

        switch(response->address.word) {
                case NES_MAPPER_PROGRAM_ROM_0:
                        response->data.dword = bus->mapper.rom_program[ROM_BANK_0];
                        TRACE(LEVEL_VERBOSE, "Mapper read [PRG-ROM0]->%u(%08X)", response->data.dword, response->data.dword);
                        break;
                case NES_MAPPER_PROGRAM_ROM_1:
                        response->data.dword = bus->mapper.rom_program[ROM_BANK_1];
                        TRACE(LEVEL_VERBOSE, "Mapper read [PRG-ROM1]->%u(%08X)", response->data.dword, response->data.dword);
                        break;
                case NES_MAPPER_PROGRAM_RAM:
                        response->data.dword = bus->mapper.ram_program;
                        TRACE(LEVEL_VERBOSE, "Mapper read [PRG-RAM]->%u(%08X)", response->data.dword, response->data.dword);
                        break;
                case NES_MAPPER_CHARACTER_ROM:
                        response->data.dword = bus->mapper.rom_character;
                        TRACE(LEVEL_VERBOSE, "Mapper read [CHR-ROM]->%u(%08X)", response->data.dword, response->data.dword);
                        break;
                case NES_MAPPER_CHARACTER_RAM:
                        response->data.dword = bus->mapper.ram_character;
                        TRACE(LEVEL_VERBOSE, "Mapper read [CHR-RAM]->%u(%08X)", response->data.dword, response->data.dword);
                        break;
                default:
                        result = ERROR(NES_ERR, "invalid mapper register read -- %i", response->address.word);
                        goto exit;
        }

exit:
        return result;
}

int
nes_action_mapper_write(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result = NES_OK;

        switch(request->address.word) {
                case NES_MAPPER_PROGRAM_ROM_0:
                        bus->mapper.rom_program[ROM_BANK_0] = request->data.dword;
                        TRACE(LEVEL_VERBOSE, "Mapper write [PRG-ROM0]<-%u(%08X)", bus->mapper.rom_program[ROM_BANK_0], bus->mapper.rom_program[ROM_BANK_0]);
                        break;
                case NES_MAPPER_PROGRAM_ROM_1:
                        bus->mapper.rom_program[ROM_BANK_1] = request->data.dword;
                        TRACE(LEVEL_VERBOSE, "Mapper write [PRG-ROM1]<-%u(%08X)", bus->mapper.rom_program[ROM_BANK_1], bus->mapper.rom_program[ROM_BANK_1]);
                        break;
                case NES_MAPPER_PROGRAM_RAM:
                        bus->mapper.ram_program = request->data.dword;
                        TRACE(LEVEL_VERBOSE, "Mapper write [PRG-RAM]<-%u(%08X)", bus->mapper.ram_program, bus->mapper.ram_program);
                        break;
                case NES_MAPPER_CHARACTER_ROM:
                        bus->mapper.rom_character = request->data.dword;
                        TRACE(LEVEL_VERBOSE, "Mapper write [CHR-ROM]<-%u(%08X)", bus->mapper.rom_character, bus->mapper.rom_character);
                        break;
                case NES_MAPPER_CHARACTER_RAM:
                        bus->mapper.ram_character = request->data.dword;
                        TRACE(LEVEL_VERBOSE, "Mapper write [CHR-RAM]<-%u(%08X)", bus->mapper.ram_character, bus->mapper.ram_character);
                        break;
                default:
                        result = ERROR(NES_ERR, "invalid mapper register write -- %i", request->address.word);
                        goto exit;
        }

exit:
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
        response->address.word = request->address.word;

        switch(response->address.word) {
                case NES_PROCESSOR_PROGRAM_COUNTER:
                        response->data.word = bus->processor.program_counter.word;
                        TRACE(LEVEL_VERBOSE, "Processor read [PC]->%04X", response->data.word);
                        break;
                case NES_PROCESSOR_STACK_POINTER:
                        response->data.word = STACK_ADDRESS | bus->processor.stack_pointer.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [SP]->%04X", response->data.word);
                        break;
                case NES_PROCESSOR_STATUS:
                        response->data.low = bus->processor.status.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [S]->%02X", response->data.low);
                        break;
                case NES_PROCESSOR_PENDING:
                        response->data.low = bus->processor.pending.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [P]->%02X", response->data.low);
                        break;
                case NES_PROCESSOR_ACCUMULATOR:
                        response->data.low = bus->processor.accumulator.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [A]->%02X", response->data.low);
                        break;
                case NES_PROCESSOR_INDEX_X:
                        response->data.low = bus->processor.index_x.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [X]->%02X", response->data.low);
                        break;
                case NES_PROCESSOR_INDEX_Y:
                        response->data.low = bus->processor.index_y.low;
                        TRACE(LEVEL_VERBOSE, "Processor read [Y]->%02X", response->data.low);
                        break;
                default:
                        result = ERROR(NES_ERR, "invalid processor register read -- %i", response->address.word);
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

        switch(request->address.word) {
                case NES_PROCESSOR_PROGRAM_COUNTER:
                        bus->processor.program_counter.word = request->data.word;
                        TRACE(LEVEL_VERBOSE, "Processor write [PC]<-%04X", bus->processor.program_counter.word);
                        break;
                case NES_PROCESSOR_STACK_POINTER:
                        bus->processor.stack_pointer.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Processor write [SP]<-%04X", STACK_ADDRESS | bus->processor.stack_pointer.low);
                        break;
                case NES_PROCESSOR_STATUS:
                        bus->processor.status.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Processor write [S]<-%02X", bus->processor.status.low);
                        break;
                case NES_PROCESSOR_PENDING:
                        bus->processor.pending.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Processor write [P]<-%02X", bus->processor.pending.low);
                        break;
                case NES_PROCESSOR_ACCUMULATOR:
                        bus->processor.accumulator.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Processor write [A]<-%02X", bus->processor.accumulator.low);
                        break;
                case NES_PROCESSOR_INDEX_X:
                        bus->processor.index_x.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Processor write [X]<-%02X", bus->processor.index_x.low);
                        break;
                case NES_PROCESSOR_INDEX_Y:
                        bus->processor.index_y.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Processor write [Y]<-%02X", bus->processor.index_y.low);
                        break;
                default:
                        result = ERROR(NES_ERR, "invalid processor register write -- %i", request->address.word);
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
                bool complete = false;

                if(nes_service_poll() != NES_OK) {
                        result = (result == NES_EVT) ? NES_OK : result;
                        break;
                }

                do {
                        nes_processor_step(&bus->processor);

                        /* TODO: STEP SUBSYSTEMS */

                        complete = nes_video_step(&bus->video);
                        TRACE_STEP();
                } while(!complete);

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

        do {
                nes_processor_step(&bus->processor);

                /* TODO: STEP SUBSYSTEMS */

                nes_video_step(&bus->video);
                TRACE_STEP();

                if((result = nes_service_show()) != NES_OK) {
                        goto exit;
                }
        } while(bus->processor.cycles);

        nes_processor_step(&bus->processor);

        /* TODO: STEP SUBSYSTEMS */

        nes_video_step(&bus->video);
        TRACE_STEP();

        if((result = nes_service_show()) != NES_OK) {
                goto exit;
        }

exit:
        TRACE(LEVEL_INFORMATION, "%s", "Emulation paused");

        return result;
}

int
nes_action_video_read(
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
        response->address.word = request->address.word;

        switch(response->address.word) {
                case NES_VIDEO_CONTROL:
                        response->data.low = bus->video.control.raw;
                        TRACE(LEVEL_VERBOSE, "Video read [CONTROL]->%02X", response->data.low);
                        break;
                case NES_VIDEO_MASK:
                        response->data.low = bus->video.mask.raw;
                        TRACE(LEVEL_VERBOSE, "Video read [MASK]->%02X", response->data.low);
                        break;
                case NES_VIDEO_STATUS:
                        response->data.low = bus->video.status.raw;
                        TRACE(LEVEL_VERBOSE, "Video read [STATUS]->%02X", response->data.low);
                        break;
                case NES_VIDEO_OBJECT_ADDRESS:
                        response->data.word = bus->video.object_address.word;
                        TRACE(LEVEL_VERBOSE, "Video read [OAM-ADDR]->%04X", response->data.word);
                        break;
                case NES_VIDEO_OBJECT_DATA:
                        response->data.low = bus->video.object_data.low;
                        TRACE(LEVEL_VERBOSE, "Video read [OAM-DATA]->%02X", response->data.low);
                        break;
                case NES_VIDEO_SCROLL_X:
                        response->data.low = bus->video.scroll_x.low;
                        TRACE(LEVEL_VERBOSE, "Video read [SCROLL-X]->%02X", response->data.low);
                        break;
                case NES_VIDEO_SCROLL_Y:
                        response->data.low = bus->video.scroll_y.low;
                        TRACE(LEVEL_VERBOSE, "Video read [SCROLL-Y]->%02X", response->data.low);
                        break;
                case NES_VIDEO_ADDRESS:
                        response->data.word = bus->video.address.word;
                        TRACE(LEVEL_VERBOSE, "Video read [ADDRESS]->%04X", response->data.word);
                        break;
                case NES_VIDEO_DATA:
                        response->data.low = bus->video.data.low;
                        TRACE(LEVEL_VERBOSE, "Video read [DATA]->%02X", response->data.low);
                        break;
                default:
                        result = ERROR(NES_ERR, "invalid video register read -- %i", response->address.word);
                        goto exit;
        }

exit:
        return result;
}

int
nes_action_video_write(
        __in nes_bus_t *bus,
        __in const nes_action_t *request,
        __inout nes_action_t *response
        )
{
        int result = NES_OK;

        switch(request->address.word) {
                case NES_VIDEO_CONTROL:
                        bus->video.control.raw = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Video write [CONTROL]<-%02X", bus->video.control.raw);
                        break;
                case NES_VIDEO_MASK:
                        bus->video.mask.raw = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Video write [MASK]<-%02X", bus->video.mask.raw);
                        break;
                case NES_VIDEO_STATUS:
                        bus->video.status.raw = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Video write [STATUS]<-%02X", bus->video.status.raw);
                        break;
                case NES_VIDEO_OBJECT_ADDRESS:
                        bus->video.object_address.word = request->data.word;
                        TRACE(LEVEL_VERBOSE, "Video write [OAM-ADDR]<-%04X", bus->video.object_address.word);
                        break;
                case NES_VIDEO_OBJECT_DATA:
                        bus->video.object_data.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Video write [OAM-DATA]<-%02X", bus->video.object_data.low);
                        break;
                case NES_VIDEO_SCROLL_X:
                        bus->video.scroll_x.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Video write [SCROLL-X]<-%02X", bus->video.scroll_x.low);
                        break;
                case NES_VIDEO_SCROLL_Y:
                        bus->video.scroll_y.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Video write [SCROLL-Y]<-%02X", bus->video.scroll_y.low);
                        break;
                case NES_VIDEO_ADDRESS:
                        bus->video.address.word = request->data.word;
                        TRACE(LEVEL_VERBOSE, "Video write [ADDRESS]<-%04X", bus->video.address.word);
                        break;
                case NES_VIDEO_DATA:
                        bus->video.data.low = request->data.low;
                        TRACE(LEVEL_VERBOSE, "Video write [DATA]<-%04X", bus->video.data.low);
                        break;
                default:
                        result = ERROR(NES_ERR, "invalid video register write -- %i", request->address.word);
                        goto exit;
        }

exit:
        return result;
}

#ifdef __cplusplus
extern }
#endif /* __cplusplus */
