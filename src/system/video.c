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

#include "./video_trace_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t
nes_video_object_read(
        __inout nes_video_t *video
        )
{
        return nes_bus_read(BUS_OBJECT, video->object_address.low);
}

void
nes_video_object_write(
        __inout nes_video_t *video,
        __in uint8_t data
        )
{
        nes_bus_write(BUS_OBJECT, video->object_address.low, data);
}

uint8_t
nes_video_port_read(
        __inout nes_video_t *video,
        __in uint16_t address
        )
{
        uint8_t result = 0;

        switch(address) {
                case VIDEO_PORT_STATUS: /* 0x2002 */
                        result = video->status.raw;
                        video->status.vblank = false;
                        video->address_latch = false;
                        break;
                case VIDEO_PORT_OBJECT_DATA: /* 0x2004 */
                        result = video->object_data.low = nes_video_object_read(video);
                        break;
                case VIDEO_PORT_DATA: /* 0x2007 */

                        switch(video->address.word) {
                                case VIDEO_PALETTE_RAM_BEGIN ... VIDEO_PALETTE_RAM_END: /* 0x3f00 - 0x3fff */
                                        result = video->data.low = nes_video_read(video, video->address.word);
                                        break;
                                default: /* 0x0000 - 0x3eff */
                                        result = video->data.low;
                                        video->data.low = nes_video_read(video, video->address.word);
                                        break;
                        }

                        video->address.word += VIDEO_INCREMENT[video->control.increment];
                        video->address.word %= VIDEO_ADDRESS_MIRROR;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid video port read: [%04X]", address);
                        goto exit;
        }

exit:
        return result;
}

void
nes_video_port_write(
        __inout nes_video_t *video,
        __in uint16_t address,
        __in uint8_t data
        )
{

        switch(address) {
               case VIDEO_PORT_CONTROL: /* 0x2000 */
                        video->control.raw = data;
                        break;
                case VIDEO_PORT_MASK: /* 0x2001 */
                        video->mask.raw = data;
                        break;
                case VIDEO_PORT_OBJECT_ADDRESS: /* 0x2003 */
                        video->object_address.low = data;
                        break;
                case VIDEO_PORT_OBJECT_DATA: /* 0x2004 */
                        nes_video_object_write(video, data);
                        video->object_data.low = data;
                        ++video->object_address.low;
                        break;
                case VIDEO_PORT_SCROLL: /* 0x2005 */

                        if(!video->address_latch) {
                                video->scroll_x.low = data;
                        } else {
                                video->scroll_y.low = data;
                        }

                        video->address_latch = !video->address_latch;
                        break;
                case VIDEO_PORT_ADDRESS: /* 0x2006 */

                        if(!video->address_latch) {
                                video->address.high = data;
                        } else {
                                video->address.low = data;
                        }

                        video->address.word %= VIDEO_ADDRESS_MIRROR;
                        video->address_latch = !video->address_latch;
                        break;
                case VIDEO_PORT_DATA: /* 0x2007 */
                        nes_video_write(video, video->address.word, data);
                        video->address.word += VIDEO_INCREMENT[video->control.increment];
                        video->address.word %= VIDEO_ADDRESS_MIRROR;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid video port write: [%04X]<-%02X", address, data);
                        break;
        }
}

uint8_t
nes_video_read(
        __inout nes_video_t *video,
        __in uint16_t address
        )
{
        return nes_bus_read(BUS_VIDEO, address);
}

void
nes_video_reset(
        __inout nes_video_t *video
        )
{
        TRACE(LEVEL_VERBOSE, "%s", "Video reset");
        memset(video, 0, sizeof(*video));
        TRACE_VIDEO(LEVEL_VERBOSE, video);
}

bool
nes_video_step(
        __inout nes_video_t *video
        )
{
        bool result = false;

        if(!video->cycles) {
                video->cycles = VIDEO_CYCLES;

                /* TODO: STEP VIDEO */

                TRACE_VIDEO(LEVEL_VERBOSE, video);
        }

        --video->cycles;

        /* TODO: DEBUG */
        if((result = (video->cycles_frame++ >= CYCLES_PER_FRAME))) {
                video->cycles_frame = 0;
        }
        /* --- */

        return result;
}

void
nes_video_write(
        __inout nes_video_t *video,
        __in uint16_t address,
        __in uint8_t data
        )
{
        nes_bus_write(BUS_VIDEO, address, data);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
