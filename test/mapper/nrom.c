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

static nes_test_mapper_nrom_t g_test = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_test_mapper_nrom_load(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + ROM_PROGRAM_BANK_WIDTH + ROM_CHARACTER_BANK_WIDTH)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 1;
	header->rom_character_count = 1;

	if(ASSERT(nes_mapper_load(&g_test.configuration, &g_test.mapper) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	if(ASSERT((g_test.mapper.ram == 0)
			&& (g_test.mapper.rom_program[ROM_BANK_0] == 0)
			&& (g_test.mapper.rom_program[ROM_BANK_1] == 0)
			&& (g_test.mapper.rom_character == 0)
			&& (g_test.mapper.read_ram == nes_mapper_nrom_read_ram)
			&& (g_test.mapper.read_rom == nes_mapper_nrom_read_rom)
			&& (g_test.mapper.write_ram == nes_mapper_nrom_write_ram)
			&& (g_test.mapper.write_rom == nes_mapper_nrom_write_rom)) != NES_OK) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	if(ASSERT(nes_mapper_load(&g_test.configuration, &g_test.mapper) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	if(ASSERT((g_test.mapper.ram == 0)
			&& (g_test.mapper.rom_program[ROM_BANK_0] == 0)
			&& (g_test.mapper.rom_program[ROM_BANK_1] == 1)
			&& (g_test.mapper.rom_character == 0)
			&& (g_test.mapper.read_ram == nes_mapper_nrom_read_ram)
			&& (g_test.mapper.read_rom == nes_mapper_nrom_read_rom)
			&& (g_test.mapper.write_ram == nes_mapper_nrom_write_ram)
			&& (g_test.mapper.write_rom == nes_mapper_nrom_write_rom)) != NES_OK) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_mapper_nrom_read_ram(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + ROM_PROGRAM_BANK_WIDTH + ROM_CHARACTER_BANK_WIDTH)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 1;
	header->rom_character_count = 1;

	if(ASSERT(nes_mapper_load(&g_test.configuration, &g_test.mapper) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < NROM_RAM_BANK_WIDTH; ++address) {
		g_test.mapper.cartridge.ram.ptr[address] = rand();
	}

	for(uint16_t address = 0; address < NROM_RAM_BANK_WIDTH; ++address) {

		if(ASSERT(nes_mapper_read_ram(&g_test.mapper, address) == g_test.mapper.cartridge.ram.ptr[address]) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_mapper_nrom_read_rom(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + ROM_PROGRAM_BANK_WIDTH + ROM_CHARACTER_BANK_WIDTH)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 1;
	header->rom_character_count = 1;

	if(ASSERT(nes_mapper_load(&g_test.configuration, &g_test.mapper) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < NROM_ROM_PROGRAM_BANK_WIDTH; ++address) {
		g_test.mapper.cartridge.rom[ROM_PROGRAM].ptr[address] = rand();
	}

	for(uint16_t address = 0; address < NROM_ROM_PROGRAM_BANK_WIDTH; ++address) {

		if(ASSERT(nes_mapper_read_rom(&g_test.mapper, ROM_PROGRAM, address) == g_test.mapper.cartridge.rom[ROM_PROGRAM].ptr[address]) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(uint16_t address = 0; address < NROM_ROM_CHARACTER_BANK_WIDTH; ++address) {
		g_test.mapper.cartridge.rom[ROM_CHARACTER].ptr[address] = rand();
	}

	for(uint16_t address = 0; address < NROM_ROM_CHARACTER_BANK_WIDTH; ++address) {

		if(ASSERT(nes_mapper_read_rom(&g_test.mapper, ROM_CHARACTER, address) == g_test.mapper.cartridge.rom[ROM_CHARACTER].ptr[address]) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	if(ASSERT(nes_mapper_load(&g_test.configuration, &g_test.mapper) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < (2 * NROM_ROM_PROGRAM_BANK_WIDTH); ++address) {
		g_test.mapper.cartridge.rom[ROM_PROGRAM].ptr[address] = rand();
	}

	for(uint16_t address = 0; address < (2 * NROM_ROM_PROGRAM_BANK_WIDTH); ++address) {

		if(ASSERT(nes_mapper_read_rom(&g_test.mapper, ROM_PROGRAM, address) == g_test.mapper.cartridge.rom[ROM_PROGRAM].ptr[address]) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(uint16_t address = 0; address < NROM_ROM_CHARACTER_BANK_WIDTH; ++address) {
		g_test.mapper.cartridge.rom[ROM_CHARACTER].ptr[address] = rand();
	}

	for(uint16_t address = 0; address < NROM_ROM_CHARACTER_BANK_WIDTH; ++address) {

		if(ASSERT(nes_mapper_read_rom(&g_test.mapper, ROM_CHARACTER, address) == g_test.mapper.cartridge.rom[ROM_CHARACTER].ptr[address]) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_mapper_nrom_write_ram(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + ROM_PROGRAM_BANK_WIDTH + ROM_CHARACTER_BANK_WIDTH)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 1;
	header->rom_character_count = 1;

	if(ASSERT(nes_mapper_load(&g_test.configuration, &g_test.mapper) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < NROM_RAM_BANK_WIDTH; ++address) {
		uint8_t data = rand();

		nes_mapper_write_ram(&g_test.mapper, address, data);

		if(ASSERT(g_test.mapper.cartridge.ram.ptr[address] == data) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_mapper_nrom_write_rom(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + ROM_PROGRAM_BANK_WIDTH + ROM_CHARACTER_BANK_WIDTH)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 1;
	header->rom_character_count = 1;

	if(ASSERT(nes_mapper_load(&g_test.configuration, &g_test.mapper) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < NROM_ROM_PROGRAM_BANK_WIDTH; ++address) {
		uint8_t data = rand(), data_1 = data + 1;

		g_test.mapper.cartridge.rom[ROM_PROGRAM].ptr[address] = data_1;
		nes_mapper_write_rom(&g_test.mapper, ROM_PROGRAM, address, data);

		if(ASSERT(g_test.mapper.cartridge.rom[ROM_PROGRAM].ptr[address] == data_1) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(uint16_t address = 0; address < NROM_ROM_CHARACTER_BANK_WIDTH; ++address) {
		uint8_t data = rand(), data_1 = data + 1;

		g_test.mapper.cartridge.rom[ROM_CHARACTER].ptr[address] = data_1;
		nes_mapper_write_rom(&g_test.mapper, ROM_CHARACTER, address, data);

		if(ASSERT(g_test.mapper.cartridge.rom[ROM_CHARACTER].ptr[address] == data_1) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	if(ASSERT(nes_mapper_load(&g_test.configuration, &g_test.mapper) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < (2 * NROM_ROM_PROGRAM_BANK_WIDTH); ++address) {
		uint8_t data = rand(), data_1 = data + 1;

		g_test.mapper.cartridge.rom[ROM_PROGRAM].ptr[address] = data_1;
		nes_mapper_write_rom(&g_test.mapper, ROM_PROGRAM, address, data);

		if(ASSERT(g_test.mapper.cartridge.rom[ROM_PROGRAM].ptr[address] == data_1) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(uint16_t address = 0; address < NROM_ROM_CHARACTER_BANK_WIDTH; ++address) {
		uint8_t data = rand(), data_1 = data + 1;

		g_test.mapper.cartridge.rom[ROM_CHARACTER].ptr[address] = data_1;
		nes_mapper_write_rom(&g_test.mapper, ROM_CHARACTER, address, data);

		if(ASSERT(g_test.mapper.cartridge.rom[ROM_CHARACTER].ptr[address] == data_1) != NES_OK) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TRACE_RESULT(result);

	return result;
}

void
nes_test_initialize(void)
{
	nes_test_uninitialize();
}

void
nes_test_uninitialize(void)
{
	nes_mapper_unload(&g_test.mapper);
	nes_buffer_free(&g_test.configuration.rom.data);
	memset(&g_test, 0, sizeof(g_test));
}

int
main(
	__in int argc,
	__in char *argv[]
	)
{
	int result = NES_OK, seed;

	if(argc > 1) {
		seed = strtol(argv[1], NULL, 16);
	} else {
		seed = time(NULL);
	}

	srand(seed);
	TRACE_SEED(seed);

	for(size_t test = 0; test < TEST_COUNT(TEST); ++test) {

		if(TEST[test]() != NES_OK) {
			result = NES_ERR;
		}
	}

	nes_test_uninitialize();

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
