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
#include "../include/service.h"

#define OBJECT_RAM_BEGIN 0x0000
#define OBJECT_RAM_END 0x00ff
#define OBJECT_RAM_FILL 0x00

#define PROCESSOR_RAM_BEGIN 0x0000
#define PROCESSOR_RAM_END 0x1fff
#define PROCESSOR_RAM_FILL 0xea
#define PROCESSOR_RAM_MIRROR 0x0800

#define PROCESSOR_ROM_0_BEGIN 0x8000
#define PROCESSOR_ROM_0_END 0xbfff

#define PROCESSOR_ROM_1_BEGIN 0xc000
#define PROCESSOR_ROM_1_END 0xffff

#define PROCESSOR_WORK_RAM_BEGIN 0x6000
#define PROCESSOR_WORK_RAM_END 0x7fff

#define VIDEO_PALETTE_RAM_BEGIN 0x3f00
#define VIDEO_PALETTE_RAM_END 0x3fff
#define VIDEO_PALETTE_RAM_FILL 0x3f
#define VIDEO_PALETTE_RAM_MIRROR 0x0020

#define VIDEO_RAM_BEGIN 0x2000
#define VIDEO_RAM_END 0x3eff
#define VIDEO_RAM_FILL 0x00
#define VIDEO_RAM_MIRROR 0x1000

#define VIDEO_ROM_BEGIN 0x0000
#define VIDEO_ROM_END 0x1fff

#define OBJECT_RAM_WIDTH \
        ADDRESS_WIDTH(OBJECT_RAM_BEGIN, OBJECT_RAM_END)

#define PROCESSOR_RAM_WIDTH \
        ADDRESS_WIDTH(PROCESSOR_RAM_BEGIN, PROCESSOR_RAM_BEGIN + PROCESSOR_RAM_MIRROR - 1)

#define VIDEO_RAM_WIDTH \
        ADDRESS_WIDTH(VIDEO_RAM_BEGIN, VIDEO_RAM_BEGIN + VIDEO_RAM_MIRROR - 1)

#define VIDEO_PALETTE_RAM_WIDTH \
        ADDRESS_WIDTH(VIDEO_PALETTE_RAM_BEGIN, VIDEO_PALETTE_RAM_BEGIN + VIDEO_PALETTE_RAM_MIRROR - 1)

typedef struct {
        nes_mapper_t mapper;
        nes_processor_t processor;
        nes_buffer_t ram_object;
        nes_buffer_t ram_processor;
        nes_buffer_t ram_video;
        nes_buffer_t ram_video_palette;

	/* TODO: ADD SUBSYSTEMS */

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
