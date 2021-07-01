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

#ifndef NES_CARTRIDGE_H_
#define NES_CARTRIDGE_H_

#include "./buffer.h"

enum {
	MAPPER_NROM = 0,
	MAPPER_MAX,
};

enum {
	RAM_PROGRAM = 0,
	RAM_CHARACTER,
	RAM_MAX,
};

enum {
	ROM_PROGRAM = 0,
	ROM_CHARACTER,
	ROM_MAX,
};

typedef struct {
	const nes_header_t *header;
	int mapper;
	nes_buffer_t ram[RAM_MAX];
	size_t ram_count[RAM_MAX];
	nes_buffer_t rom[ROM_MAX];
	size_t rom_count[ROM_MAX];
} nes_cartridge_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_cartridge_load(
	__in const nes_t *configuration,
	__inout nes_cartridge_t *cartridge
	);

uint8_t nes_cartridge_read_ram(
	__in const nes_cartridge_t *cartridge,
	__in int type,
	__in size_t address
	);

uint8_t nes_cartridge_read_rom(
	__in const nes_cartridge_t *cartridge,
	__in int type,
	__in size_t address
	);

void nes_cartridge_unload(
	__inout nes_cartridge_t *cartridge
	);

void nes_cartridge_write_ram(
	__inout nes_cartridge_t *cartridge,
	__in int type,
	__in size_t address,
	__in uint8_t data
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_CARTRIDGE_H_ */
