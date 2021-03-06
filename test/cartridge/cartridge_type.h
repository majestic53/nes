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

#ifndef NES_TEST_CARTRIDGE_TYPE_H_
#define NES_TEST_CARTRIDGE_TYPE_H_

#include "../../src/common/cartridge_type.h"
#include "../common.h"

typedef struct {
	nes_t configuration;
	nes_cartridge_t cartridge;
	nes_header_t header;
} nes_test_cartridge_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_test_cartridge_load(void);

int nes_test_cartridge_ram_read(void);

int nes_test_cartridge_ram_write(void);

int nes_test_cartridge_rom_read(void);

int nes_test_cartridge_unload(void);

void nes_test_initialize(void);

void nes_test_uninitialize(void);

static const nes_test TEST[] = {
	nes_test_cartridge_load,
	nes_test_cartridge_ram_read,
        nes_test_cartridge_ram_write,
	nes_test_cartridge_rom_read,
	nes_test_cartridge_unload,
	};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_TEST_CARTRIDGE_TYPE_H_ */
