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

#ifndef NES_CARTRIDGE_TYPE_H_
#define NES_CARTRIDGE_TYPE_H_

#include "../../include/common.h"

#define HEADER_MAGIC "NES\x1a\0"

#define RAM_CHARACTER_BANK_WIDTH (8 * BYTES_PER_KBYTE)

#define RAM_PROGRAM_BANK_FILL 0xea
#define RAM_PROGRAM_BANK_WIDTH (8 * BYTES_PER_KBYTE)

#define ROM_CHARACTER_BANK_WIDTH (8 * BYTES_PER_KBYTE)

#define ROM_PROGRAM_BANK_WIDTH (16 * BYTES_PER_KBYTE)

#define TRAINER_WIDTH 512

#endif /* NES_CARTRIDGE_TYPE_H_ */
