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

static nes_test_cartridge_t g_test = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_test_cartridge_load(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header), 0x00)) != NES_OK) {
		goto exit;
	}

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header), 0x00)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_MAX;

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header), 0x00)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00))
			!= NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = true;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00))
			!= NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memset(header->magic, 0, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 2;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00))
			!= NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 2;
	header->rom_program_count = 0;
	header->rom_character_count = 1;

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00))
			!= NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 2;
	header->rom_program_count = 2;
	header->rom_character_count = 0;

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00))
			!= NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = false;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 2;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	if(ASSERT((nes_cartridge_load(&g_test.configuration, &g_test.cartridge) == NES_OK)
			&& (g_test.cartridge.ram[RAM_PROGRAM].length == (2 * RAM_PROGRAM_BANK_WIDTH)))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data,
				sizeof(g_test.header) + TRAINER_WIDTH + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = true;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	if(ASSERT((nes_cartridge_load(&g_test.configuration, &g_test.cartridge) == NES_OK)
			&& (g_test.cartridge.ram[RAM_PROGRAM].length == RAM_PROGRAM_BANK_WIDTH))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_cartridge_read_ram(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00))
			!= NES_OK) {
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

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	memset(g_test.cartridge.ram[RAM_PROGRAM].ptr, 0xaa, RAM_PROGRAM_BANK_WIDTH);

	for(uint16_t address = 0; address < RAM_PROGRAM_BANK_WIDTH; ++address) {

		if(ASSERT(nes_cartridge_read_ram(&g_test.cartridge, RAM_PROGRAM, address) == 0xaa)) {
			result = NES_ERR;
			goto exit;
		}
	}

	memset(g_test.cartridge.ram[RAM_CHARACTER].ptr, 0xbb, RAM_CHARACTER_BANK_WIDTH);

	for(uint16_t address = 0; address < RAM_CHARACTER_BANK_WIDTH; ++address) {

		if(ASSERT(nes_cartridge_read_ram(&g_test.cartridge, RAM_CHARACTER, address) == 0xbb)) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_cartridge_read_rom(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data, sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00))
			!= NES_OK) {
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

	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header), 0xaa, ROM_PROGRAM_BANK_WIDTH);
	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header) + ROM_PROGRAM_BANK_WIDTH, 0xbb, ROM_PROGRAM_BANK_WIDTH);
	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH), 0xcc, ROM_CHARACTER_BANK_WIDTH);

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < ROM_PROGRAM_BANK_WIDTH; ++address) {

		if(ASSERT((nes_cartridge_read_rom(&g_test.cartridge, ROM_PROGRAM, address) == 0xaa)
				&& (nes_cartridge_read_rom(&g_test.cartridge, ROM_PROGRAM, ROM_PROGRAM_BANK_WIDTH + address) == 0xbb))) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(uint16_t address = 0; address < ROM_CHARACTER_BANK_WIDTH; ++address) {

		if(ASSERT(nes_cartridge_read_rom(&g_test.cartridge, ROM_CHARACTER, address) == 0xcc)) {
			result = NES_ERR;
			goto exit;
		}
	}

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data,
			sizeof(g_test.header) + TRAINER_WIDTH + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = true;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header), 0xff, TRAINER_WIDTH);
	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header) + TRAINER_WIDTH, 0xaa, ROM_PROGRAM_BANK_WIDTH);
	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header) + ROM_PROGRAM_BANK_WIDTH + TRAINER_WIDTH, 0xbb, ROM_PROGRAM_BANK_WIDTH);
	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + TRAINER_WIDTH, 0xcc, ROM_CHARACTER_BANK_WIDTH);

	if(ASSERT((nes_cartridge_load(&g_test.configuration, &g_test.cartridge) == NES_OK)
			&& (g_test.cartridge.ram[RAM_PROGRAM].length == RAM_PROGRAM_BANK_WIDTH))) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < ROM_PROGRAM_BANK_WIDTH; ++address) {

		if(ASSERT((nes_cartridge_read_rom(&g_test.cartridge, ROM_PROGRAM, address) == 0xaa)
				&& (nes_cartridge_read_rom(&g_test.cartridge, ROM_PROGRAM, ROM_PROGRAM_BANK_WIDTH + address) == 0xbb))) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(uint16_t address = 0; address < ROM_CHARACTER_BANK_WIDTH; ++address) {

		if(ASSERT(nes_cartridge_read_rom(&g_test.cartridge, ROM_CHARACTER, address) == 0xcc)) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_cartridge_unload(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data,
			sizeof(g_test.header) + TRAINER_WIDTH + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00)) != NES_OK) {
		goto exit;
	}

	header = (nes_cartridge_header_t *)g_test.configuration.rom.data.ptr;
	memcpy(header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC));
	header->flag_6.mapper_low = MAPPER_NROM;
	header->flag_6.trainer = true;
	header->flag_7.mapper_high = 0;
	header->ram_program_count = 0;
	header->rom_program_count = 2;
	header->rom_character_count = 1;

	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header), 0xff, TRAINER_WIDTH);
	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header) + TRAINER_WIDTH, 0xaa, ROM_PROGRAM_BANK_WIDTH);
	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header) + ROM_PROGRAM_BANK_WIDTH + TRAINER_WIDTH, 0xbb, ROM_PROGRAM_BANK_WIDTH);
	memset(g_test.configuration.rom.data.ptr + sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + TRAINER_WIDTH, 0xcc, ROM_CHARACTER_BANK_WIDTH);

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_cartridge_unload(&g_test.cartridge);

	if(ASSERT(!g_test.cartridge.header
			&& !g_test.cartridge.ram[RAM_PROGRAM].ptr
			&& !g_test.cartridge.ram[RAM_PROGRAM].length
			&& !g_test.cartridge.ram[RAM_CHARACTER].ptr
			&& !g_test.cartridge.ram[RAM_CHARACTER].length
			&& !g_test.cartridge.ram_count[RAM_PROGRAM]
			&& !g_test.cartridge.ram_count[RAM_CHARACTER]
			&& !g_test.cartridge.rom[ROM_PROGRAM].ptr
			&& !g_test.cartridge.rom[ROM_PROGRAM].length
			&& !g_test.cartridge.rom[ROM_CHARACTER].ptr
			&& !g_test.cartridge.rom[ROM_CHARACTER].length
			&& !g_test.cartridge.rom_count[ROM_PROGRAM]
			&& !g_test.cartridge.rom_count[ROM_CHARACTER])) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_cartridge_write_ram(void)
{
	int result = NES_OK;
	nes_cartridge_header_t *header;

	nes_test_initialize();

	if((result = nes_buffer_allocate(&g_test.configuration.rom.data,
			sizeof(g_test.header) + (2 * ROM_PROGRAM_BANK_WIDTH) + ROM_CHARACTER_BANK_WIDTH, 0x00)) != NES_OK) {
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

	if(ASSERT(nes_cartridge_load(&g_test.configuration, &g_test.cartridge) == NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	for(uint16_t address = 0; address < RAM_PROGRAM_BANK_WIDTH; ++address) {
		nes_cartridge_write_ram(&g_test.cartridge, RAM_PROGRAM, address, 0xaa);

		if(ASSERT(g_test.cartridge.ram[RAM_PROGRAM].ptr[address] == 0xaa)) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(uint16_t address = 0; address < RAM_CHARACTER_BANK_WIDTH; ++address) {
		nes_cartridge_write_ram(&g_test.cartridge, RAM_CHARACTER, address, 0xbb);

		if(ASSERT(g_test.cartridge.ram[RAM_CHARACTER].ptr[address] == 0xbb)) {
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
	nes_cartridge_unload(&g_test.cartridge);
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
