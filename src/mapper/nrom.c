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

#include "./nrom_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_mapper_nrom_load(
	__inout nes_mapper_t *mapper
	)
{
	int result = NES_OK;

	TRACE(LEVEL_VERBOSE, "%s", "NROM mapper loading");

	mapper->ram = 0;
	mapper->rom_program[ROM_BANK_0] = 0;
	mapper->rom_program[ROM_BANK_1] = (mapper->cartridge.rom_count[ROM_PROGRAM] > 1) ? 1 : 0;
	mapper->rom_character = 0;
	mapper->read_ram = nes_mapper_nrom_read_ram;
	mapper->read_rom = nes_mapper_nrom_read_rom;
	mapper->write_ram = nes_mapper_nrom_write_ram;
	mapper->write_rom = nes_mapper_nrom_write_rom;

	TRACE(LEVEL_VERBOSE, "%s", "NROM mapper loaded");

	return result;
}

uint8_t
nes_mapper_nrom_read_ram(
	__in const nes_mapper_t *mapper,
	__in uint16_t address
	)
{
	return nes_cartridge_read_ram(&mapper->cartridge, (mapper->ram * NROM_RAM_BANK_WIDTH) + (address % NROM_RAM_BANK_WIDTH));
}

uint8_t
nes_mapper_nrom_read_rom(
	__in const nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address
	)
{
	uint8_t result = 0;
	size_t bank = ROM_BANK_0;

	switch(type) {
		case ROM_CHARACTER:
			result = nes_cartridge_read_rom(&mapper->cartridge, type, (mapper->rom_character * NROM_ROM_CHARACTER_BANK_WIDTH)
					+ (address % NROM_ROM_CHARACTER_BANK_WIDTH));
			break;
		case ROM_PROGRAM:

			if(mapper->cartridge.rom_count[ROM_PROGRAM] > 1) {
				bank = (address >= NROM_ROM_PROGRAM_BANK_WIDTH) ? ROM_BANK_1 : ROM_BANK_0;
			}

			result = nes_cartridge_read_rom(&mapper->cartridge, type, (mapper->rom_program[bank] * NROM_ROM_PROGRAM_BANK_WIDTH)
					+ (address % NROM_ROM_PROGRAM_BANK_WIDTH));
			break;
		default:
			break;
	}

	return result;
}

void
nes_mapper_nrom_write_ram(
	__inout nes_mapper_t *mapper,
	__in uint16_t address,
	__in uint8_t data
	)
{
	nes_cartridge_write_ram(&mapper->cartridge, (mapper->ram * NROM_RAM_BANK_WIDTH) + (address % NROM_RAM_BANK_WIDTH), data);
}

void
nes_mapper_nrom_write_rom(
	__inout nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address,
	__in uint8_t data
	)
{
	return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
