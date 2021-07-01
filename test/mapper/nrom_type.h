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

#ifndef NES_TEST_MAPPER_NROM_TYPE_H_
#define NES_TEST_MAPPER_NROM_TYPE_H_

#include "../../src/common/cartridge_type.h"
#include "../../src/common/mapper_type.h"
#include "../common.h"

typedef struct {
        nes_t configuration;
        nes_header_t header;
        nes_mapper_t mapper;
} nes_test_mapper_nrom_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_test_mapper_nrom_load(void);

int nes_test_mapper_nrom_read_ram(void);

int nes_test_mapper_nrom_read_rom(void);

int nes_test_mapper_nrom_write_ram(void);

int nes_test_mapper_nrom_write_rom(void);

void nes_test_initialize(void);

void nes_test_uninitialize(void);

static const nes_test TEST[] = {
	nes_test_mapper_nrom_load,
	nes_test_mapper_nrom_read_ram,
	nes_test_mapper_nrom_read_rom,
	nes_test_mapper_nrom_write_ram,
        nes_test_mapper_nrom_write_rom,
	};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_TEST_MAPPER_NROM_TYPE_H_ */
