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

#ifndef NES_BUS_TYPE_H_
#define NES_BUS_TYPE_H_

#include "../include/system/processor.h"
#include "../include/system/video.h"
#include "../include/service.h"

#define OBJECT_RAM_WIDTH \
        ADDRESS_WIDTH(OBJECT_RAM_BEGIN, OBJECT_RAM_END)

#define PROCESSOR_RAM_WIDTH \
        ADDRESS_WIDTH(PROCESSOR_RAM_BEGIN, PROCESSOR_RAM_BEGIN + PROCESSOR_RAM_MIRROR - 1)

#define VIDEO_RAM_WIDTH \
        ADDRESS_WIDTH(VIDEO_RAM_BEGIN, VIDEO_RAM_BEGIN + VIDEO_RAM_MIRROR - 1)

#define VIDEO_PALETTE_RAM_WIDTH \
        ADDRESS_WIDTH(VIDEO_PALETTE_RAM_BEGIN, VIDEO_PALETTE_RAM_BEGIN + VIDEO_PALETTE_RAM_MIRROR - 1)

typedef struct {
        uint8_t data;
        bool loaded;
        nes_mapper_t mapper;
        nes_processor_t processor;
        nes_buffer_t ram_object;
        nes_buffer_t ram_processor;
        nes_buffer_t ram_video;
        nes_buffer_t ram_video_palette;
        nes_video_t video;
} nes_bus_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

nes_bus_t *nes_bus(void);

int nes_bus_load(
	__in const nes_t *configuration
	);

void nes_bus_unload(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_BUS_TYPE_H_ */
