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

#ifndef NES_VIDEO_H_
#define NES_VIDEO_H_

#include "../bus.h"

enum {
        VIDEO_INCREMENT_ACROSS = 0,
        VIDEO_INCREMENT_DOWN,
        VIDEO_INCREMENT_MAX,
};

typedef union {

        struct {
                uint8_t namespace : 2;
                uint8_t increment : 1;
                uint8_t sprite_table : 1;
                uint8_t background_table : 1;
                uint8_t sprite_size : 1;
                uint8_t select : 1;
                uint8_t interrupt : 1;
        };

        uint8_t raw;
} nes_video_control_t;

typedef union {

        struct {
                uint8_t grayscale : 1;
                uint8_t background_show_top : 1;
                uint8_t sprite_show_top : 1;
                uint8_t background_show : 1;
                uint8_t sprite_show : 1;
                uint8_t red_emphasis : 1;
                uint8_t green_emphasis : 1;
                uint8_t blue_emphasis : 1;
        };

        uint8_t raw;
} nes_video_mask_t;

typedef union {

        struct {
                uint8_t unused : 5;
                uint8_t sprite_overflow : 1;
                uint8_t sprite_0_hit : 1;
                uint8_t vblank : 1;
        };

        uint8_t raw;
} nes_video_status_t;

typedef struct {
        nes_register_t address;
        bool address_latch;
        nes_video_control_t control;
        uint8_t cycles;
        uint16_t cycles_frame;
        nes_register_t data;
        nes_video_mask_t mask;
        nes_register_t object_address;
        nes_register_t scroll_x;
        nes_register_t scroll_y;
        nes_video_status_t status;
} nes_video_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t nes_video_port_read(
        __inout nes_video_t *video,
        __in uint16_t address
        );

void nes_video_port_write(
        __inout nes_video_t *video,
        __in uint16_t address,
        __in uint8_t data
        );

void nes_video_reset(
        __inout nes_video_t *video
        );

bool nes_video_step(
        __inout nes_video_t *video
        );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_VIDEO_H_ */
