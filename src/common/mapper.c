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

#include "./mapper_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_mapper_load(
	__in const nes_t *configuration,
	__inout nes_mapper_t *mapper
	)
{
	int result = NES_OK;

	TRACE(LEVEL_VERBOSE, "%s", "Mapper loading");

	if((result = nes_cartridge_load(configuration, &mapper->cartridge)) != NES_OK) {
		goto exit;
	}

	if((result = MAPPER_LOAD[mapper->cartridge.mapper](mapper)) != NES_OK) {
		goto exit;
	}

	TRACE(LEVEL_VERBOSE, "%s", "Mapper loaded");
	TRACE(LEVEL_VERBOSE, "Mapper ROM-PRG[0]: %zu", mapper->rom_program[ROM_BANK_0]);
	TRACE(LEVEL_VERBOSE, "Mapper ROM-PRG[1]: %zu", mapper->rom_program[ROM_BANK_1]);
	TRACE(LEVEL_VERBOSE, "Mapper RAM-PRG: %zu", mapper->ram_program);
	TRACE(LEVEL_VERBOSE, "Mapper ROM-CHR: %zu", mapper->rom_character);
	TRACE(LEVEL_VERBOSE, "Mapper RAM-CHR: %zu", mapper->ram_character);

exit:
	return result;
}

uint8_t
nes_mapper_ram_read(
	__in const nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address
	)
{
	return mapper->ram_read(mapper, type, address);
}

void
nes_mapper_ram_write(
	__inout nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address,
	__in uint8_t data
	)
{
	mapper->ram_write(mapper, type, address, data);
}

uint8_t
nes_mapper_rom_read(
	__in const nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address
	)
{
	return mapper->rom_read(mapper, type, address);
}

void
nes_mapper_rom_write(
	__inout nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address,
	__in uint8_t data
	)
{
	mapper->rom_write(mapper, type, address, data);
}

void
nes_mapper_unload(
	__inout nes_mapper_t *mapper
	)
{
	TRACE(LEVEL_VERBOSE, "%s", "Mapper unloading");

	nes_cartridge_unload(&mapper->cartridge);
	memset(mapper, 0, sizeof(*mapper));

	TRACE(LEVEL_VERBOSE, "%s", "Mapper unloaded");
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
