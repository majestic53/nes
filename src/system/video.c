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
nes_video_port_read(
        __inout nes_video_t *video,
        __in uint16_t address
        )
{
        uint8_t result = 0;

        switch(address) {

                /* TODO: READ BYTE FROM VIDEO PORT */

                default:
                        TRACE(LEVEL_WARNING, "Invalid video port address: %04X", address);
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

                /* TODO: WRITE BYTE TO VIDEO PORT */

                default:
                        TRACE(LEVEL_WARNING, "Invalid video port address: %04X", address);
                        break;
        }
}

void
nes_video_reset(
        __inout nes_video_t *video
        )
{
        TRACE(LEVEL_VERBOSE, "%s", "Video reset");
        memset(video, 0, sizeof(*video));

        /* TODO: RESET VIDEO */

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

#ifdef __cplusplus
}
#endif /* __cplusplus */
