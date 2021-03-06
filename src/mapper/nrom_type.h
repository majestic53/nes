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

#ifndef NES_MAPPER_NROM_TYPE_H_
#define NES_MAPPER_NROM_TYPE_H_

#include "../common/cartridge_type.h"

#define NROM_RAM_CHARACTER_BANK_WIDTH RAM_CHARACTER_BANK_WIDTH
#define NROM_RAM_PROGRAM_BANK_WIDTH RAM_PROGRAM_BANK_WIDTH
#define NROM_ROM_CHARACTER_BANK_WIDTH ROM_CHARACTER_BANK_WIDTH
#define NROM_ROM_PROGRAM_BANK_WIDTH ROM_PROGRAM_BANK_WIDTH

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_mapper_nrom_load(
	__inout nes_mapper_t *mapper
	);

uint8_t nes_mapper_nrom_ram_read(
	__in const nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address
	);

void nes_mapper_nrom_ram_write(
	__inout nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address,
	__in uint8_t data
	);

uint8_t nes_mapper_nrom_rom_read(
	__in const nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address
	);

void nes_mapper_nrom_rom_write(
	__inout nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address,
	__in uint8_t data
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_MAPPER_NROM_TYPE_H_ */
