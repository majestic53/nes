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

#ifndef NES_TEST_VIDEO_TYPE_H_
#define NES_TEST_VIDEO_TYPE_H_

#include "../../src/system/video_type.h"
#include "../common.h"

typedef struct {
        nes_buffer_t memory;
        nes_buffer_t object;
        nes_video_t video;
} nes_test_video_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_test_video_port_read(void);

int nes_test_video_port_write(void);

int nes_test_video_reset(void);

void nes_test_initialize(void);

static const nes_test TEST[] = {
        nes_test_video_port_read,
        nes_test_video_port_write,
        nes_test_video_reset,
	};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_TEST_VIDEO_TYPE_H_ */
