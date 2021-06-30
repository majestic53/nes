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

#ifndef NES_DEFINE_H_
#define NES_DEFINE_H_

#include "../nes.h"

#ifndef __in
#define __in
#endif /* __in */

#ifndef __inout
#define __inout
#endif /* __inout */

#ifndef __out
#define __out
#endif /* __out */

#define ADDRESS_OBJECT_RAM_BEGIN 0x0000
#define ADDRESS_OBJECT_RAM_END 0x00ff

#define ADDRESS_PROCESSOR_RAM_BEGIN 0x0000
#define ADDRESS_PROCESSOR_RAM_END 0x1fff

#define ADDRESS_PROCESSOR_RAM_WORK_BEGIN 0x6000
#define ADDRESS_PROCESSOR_RAM_WORK_END 0x7fff

#define ADDRESS_PROCESSOR_ROM_0_BEGIN 0x8000
#define ADDRESS_PROCESSOR_ROM_0_END 0xbfff

#define ADDRESS_PROCESSOR_ROM_1_BEGIN 0xc000
#define ADDRESS_PROCESSOR_ROM_1_END 0xffff

#define ADDRESS_PROCESSOR_STACK 0x0100

#define ADDRESS_VIDEO_RAM_BEGIN 0x2000
#define ADDRESS_VIDEO_RAM_END 0x2fff

#define ADDRESS_VIDEO_RAM_MIRROR_BEGIN 0x3000
#define ADDRESS_VIDEO_RAM_MIRROR_END 0x3eff

#define ADDRESS_VIDEO_RAM_PALETTE_BEGIN 0x3f00
#define ADDRESS_VIDEO_RAM_PALETTE_END 0x3f1f

#define ADDRESS_VIDEO_RAM_PALETTE_MIRROR_BEGIN 0x3f20
#define ADDRESS_VIDEO_RAM_PALETTE_MIRROR_END 0x3fff

#define ADDRESS_VIDEO_ROM_BEGIN 0x0000
#define ADDRESS_VIDEO_ROM_END 0x1fff

#define BYTES_PER_KBYTE 1024

#define CYCLES_PER_FRAME 29781

#define FORMAT_MAX 512

#define FRAME_FREQUENCY (MILLISEC_PER_SEC / (float)FRAMES_PER_SEC)
#define FRAMES_PER_SEC 60

#define MILLISEC_PER_SEC 1000

#define NES "NES"

#define ADDRESS_WIDTH(_BEGIN_, _END_) \
        (((_END_) - (_BEGIN_)) + 1)

#endif /* NES_DEFINE_H_ */
