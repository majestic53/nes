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

#include "./cartridge_type.h"

#ifndef NDEBUG

const char *MAPPER[] = {
	"NROM", /* MAPPER_NROM */
	};

#endif /* NDEBUG */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_cartridge_load(
	__in const nes_t *configuration,
	__inout nes_cartridge_t *cartridge
	)
{
	uint8_t *data;
	size_t length = 0;
	int result = NES_OK;

	TRACE(LEVEL_VERBOSE, "%s", "Cartridge loading");

	if(configuration->rom.data.length < sizeof(*cartridge->header)) {
		result = ERROR(NES_ERR, "cartridge is too small -- expecting %.02f KB (%u bytes)", sizeof(*cartridge->header) / (float)BYTES_PER_KBYTE,
				sizeof(*cartridge->header));
		goto exit;
	}

	cartridge->header = (const nes_header_t *)configuration->rom.data.ptr;

	if(strncmp((char *)cartridge->header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC))) {
		result = ERROR(NES_ERR, "cartridge magic mismatch -- expecting \"%s\"", HEADER_MAGIC);
		goto exit;
	}

	switch((cartridge->mapper = (cartridge->header->flag_7.mapper_high << CHAR_BIT) | cartridge->header->flag_6.mapper_low)) {
		case MAPPER_NROM:
			break;
		default:
			result = ERROR(NES_ERR, "cartridge mapper unsupported -- %i", cartridge->mapper);
			goto exit;
	}

	data = (configuration->rom.data.ptr + sizeof(*cartridge->header));
	length += sizeof(*cartridge->header);

	if(cartridge->header->flag_6.trainer) {
		data += TRAINER_WIDTH;
		length += TRAINER_WIDTH;
	}

	if(!(cartridge->rom_count[ROM_PROGRAM] = cartridge->header->rom_program_count)) {
		result = ERROR(NES_ERR, "cartridge program rom counter invalid -- %zu", cartridge->rom_count[ROM_PROGRAM]);
		goto exit;
	}

	cartridge->rom[ROM_PROGRAM].length = cartridge->rom_count[ROM_PROGRAM] * ROM_PROGRAM_BANK_WIDTH;
	length += cartridge->rom[ROM_PROGRAM].length;

	if(!(cartridge->rom_count[ROM_CHARACTER] = cartridge->header->rom_character_count)) {
		result = ERROR(NES_ERR, "cartridge character rom counter invalid -- %zu", cartridge->rom_count[ROM_CHARACTER]);
		goto exit;
	}

	cartridge->rom[ROM_CHARACTER].length = cartridge->rom_count[ROM_CHARACTER] * ROM_CHARACTER_BANK_WIDTH;
	length += cartridge->rom[ROM_CHARACTER].length;

	if(length != configuration->rom.data.length) {
		result = ERROR(NES_ERR, "cartridge length mismatch -- expecting %.02f KB (%u bytes), found %.02f KB (%u bytes)", length / (float)BYTES_PER_KBYTE,
				length, cartridge->rom->length / (float)BYTES_PER_KBYTE, cartridge->rom->length);
		goto exit;
	}

	cartridge->rom[ROM_PROGRAM].ptr = data;
	data += cartridge->rom[ROM_PROGRAM].length;
	cartridge->rom[ROM_CHARACTER].ptr = data;
	cartridge->ram_count[RAM_PROGRAM] = cartridge->header->ram_program_count ? cartridge->header->ram_program_count : 1;

	if((result = nes_buffer_allocate(&cartridge->ram[RAM_PROGRAM], cartridge->ram_count[RAM_PROGRAM] * RAM_PROGRAM_BANK_WIDTH, RAM_PROGRAM_BANK_FILL))
			!= NES_OK) {
		goto exit;
	}

	cartridge->ram_count[RAM_CHARACTER] = 1;

	if((result = nes_buffer_allocate(&cartridge->ram[RAM_CHARACTER], cartridge->ram_count[RAM_CHARACTER] * RAM_CHARACTER_BANK_WIDTH, RAM_PROGRAM_BANK_FILL))
			!= NES_OK) {
		goto exit;
	}

	TRACE(LEVEL_VERBOSE, "%s", "Cartridge loaded");
	TRACE(LEVEL_VERBOSE, "Cartridge mapper: %i (%s)", cartridge->mapper, MAPPER[cartridge->mapper]);
	TRACE(LEVEL_VERBOSE, "Cartridge ROM-PRG: %u, %.02f KB (%u bytes)", cartridge->rom_count[ROM_PROGRAM],
		cartridge->rom[ROM_PROGRAM].length / (float)BYTES_PER_KBYTE, cartridge->rom[ROM_PROGRAM].length);
	TRACE(LEVEL_VERBOSE, "Cartridge RAM-PRG: %u, %.02f KB (%u bytes)", cartridge->ram_count[RAM_PROGRAM],
		cartridge->ram[RAM_PROGRAM].length / (float)BYTES_PER_KBYTE, cartridge->ram[RAM_PROGRAM].length);
	TRACE(LEVEL_VERBOSE, "Cartridge ROM-CHR: %u, %.02f KB (%u bytes)", cartridge->rom_count[ROM_CHARACTER],
		cartridge->rom[ROM_CHARACTER].length / (float)BYTES_PER_KBYTE, cartridge->rom[ROM_CHARACTER].length);
	TRACE(LEVEL_VERBOSE, "Cartridge RAM-CHR: %u, %.02f KB (%u bytes)", cartridge->ram_count[RAM_CHARACTER],
		cartridge->ram[RAM_CHARACTER].length / (float)BYTES_PER_KBYTE, cartridge->ram[RAM_CHARACTER].length);

exit:
	return result;
}

uint8_t
nes_cartridge_ram_read(
	__in const nes_cartridge_t *cartridge,
	__in int type,
	__in size_t address
	)
{
	return cartridge->ram[type].ptr[address];
}

void
nes_cartridge_ram_write(
	__inout nes_cartridge_t *cartridge,
	__in int type,
	__in size_t address,
	__in uint8_t data
	)
{
	cartridge->ram[type].ptr[address] = data;
}

uint8_t
nes_cartridge_rom_read(
	__in const nes_cartridge_t *cartridge,
	__in int type,
	__in size_t address
	)
{
	return cartridge->rom[type].ptr[address];
}

void
nes_cartridge_unload(
	__inout nes_cartridge_t *cartridge
	)
{
	TRACE(LEVEL_VERBOSE, "%s", "Cartridge unloading");

	for(uint32_t type = 0; type < RAM_MAX; ++type) {
		nes_buffer_free(&cartridge->ram[type]);
	}

	memset(cartridge, 0, sizeof(*cartridge));

	TRACE(LEVEL_VERBOSE, "%s", "Cartridge unloaded");
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
