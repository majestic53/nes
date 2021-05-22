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

#include "./define.h"

#define INES_VERSION_1 1
#define INES_VERSION INES_VERSION_1

enum {
	MAPPER_NROM = 0,
};

enum {
	ROM_PROGRAM = 0,
	ROM_CHARACTER,
	ROM_MAX,
};

typedef struct {
#if INES_VERSION >= INES_VERSION_1
	uint8_t magic[4];
	uint8_t rom_program_count;
	uint8_t rom_character_count;

	struct {
		uint8_t mirroring : 1;
		uint8_t battery : 1;
		uint8_t trainer : 1;
		uint8_t four_screen : 1;
		uint8_t mapper_low : 4;
	} flag_6;

	struct {
		uint8_t unused : 2;
		uint8_t version : 2;
		uint8_t mapper_high : 4;
	} flag_7;

	uint8_t ram_program_count;
#endif /* INES_VERSION >= INES_VERSION_1 */
#if INES_VERSION == INES_VERSION_1
	uint8_t unused[7];
#endif /* INES_VERSION == INES_VERSION_1 */
} nes_cartridge_header_t;

typedef struct {
	const nes_cartridge_header_t *header;
	int mapper;
	nes_buffer_t ram;
	size_t ram_count;
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
	__in size_t bank,
	__in uint16_t address
	);

uint8_t nes_cartridge_read_rom(
	__in const nes_cartridge_t *cartridge,
	__in int type,
	__in size_t bank,
	__in uint16_t address
	);

void nes_cartridge_unload(
	__inout nes_cartridge_t *cartridge
	);

void nes_cartridge_write_ram(
	__inout nes_cartridge_t *cartridge,
	__in size_t bank,
	__in uint16_t address,
	__in uint8_t data
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_CARTRIDGE_H_ */
