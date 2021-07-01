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

#include "./bus_type.h"

static nes_test_bus_t g_test = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_mapper_load(
	__in const nes_t *configuration,
	__inout nes_mapper_t *mapper
	)
{
	return g_test.mapper_status;
}

uint8_t
nes_mapper_read_ram(
	__in const nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address
	)
{
	g_test.mapper_address = address;
	g_test.mapper_type = type;

	return g_test.mapper_data;
}

uint8_t
nes_mapper_read_rom(
	__in const nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address
	)
{
	g_test.mapper_address = address;
	g_test.mapper_type = type;

	return g_test.mapper_data;
}

void
nes_mapper_unload(
	__inout nes_mapper_t *mapper
	)
{
	g_test.mapper_unload = true;
}

void
nes_mapper_write_ram(
	__inout nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address,
	__in uint8_t data
	)
{
	g_test.mapper_address = address;
	g_test.mapper_data = data;
	g_test.mapper_type = type;
}

void
nes_mapper_write_rom(
	__inout nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address,
	__in uint8_t data
	)
{
	g_test.mapper_address = address;
	g_test.mapper_data = data;
	g_test.mapper_type = type;
}

void
nes_processor_reset(
        __inout nes_processor_t *processor
        )
{
	g_test.processor_reset = true;
}

int
nes_service_load(
	__in const nes_t *configuration
	)
{
	return NES_OK;
}

void
nes_service_unload(void)
{
	return;
}

const nes_version_t *
nes_version(void)
{
	return &g_test.version;
}

int
nes_test_bus_load(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.mapper_status = NES_ERR;

	if(ASSERT((nes_bus_load(&g_test.configuration) == NES_OK)
			|| !g_test.processor_reset)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	if(ASSERT((nes_bus_load(&g_test.configuration) != NES_OK)
			|| g_test.processor_reset)) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_bus_read(void)
{
	uint32_t address;
	int result = NES_OK;

	nes_test_initialize();
	nes_bus_load(&g_test.configuration);

	for(address = OBJECT_RAM_BEGIN; address <= OBJECT_RAM_END; ++address) {

		if(ASSERT(nes_bus_read(BUS_OBJECT, address) == OBJECT_RAM_FILL)) {
			result = NES_ERR;
			goto exit;
		}
	}

	if(ASSERT(nes_bus_read(BUS_OBJECT, address) == 0x00)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus_unload();
	nes_test_initialize();
	nes_bus_load(&g_test.configuration);

	for(address = PROCESSOR_RAM_BEGIN; address <= PROCESSOR_RAM_END; ++address) {

		if(ASSERT(nes_bus_read(BUS_PROCESSOR, address) == PROCESSOR_RAM_FILL)) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(address = PROCESSOR_WORK_RAM_BEGIN; address <= PROCESSOR_WORK_RAM_END; ++address) {
		g_test.mapper_data = rand();

		if(ASSERT((nes_bus_read(BUS_PROCESSOR, address) == g_test.mapper_data)
				&& (g_test.mapper_address == address - PROCESSOR_WORK_RAM_BEGIN)
				&& (g_test.mapper_type == RAM_PROGRAM))) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(address = PROCESSOR_ROM_0_BEGIN; address <= PROCESSOR_ROM_1_END; ++address) {
		g_test.mapper_data = rand();

		if(ASSERT((nes_bus_read(BUS_PROCESSOR, address) == g_test.mapper_data)
				&& (g_test.mapper_address == address - PROCESSOR_ROM_0_BEGIN)
				&& (g_test.mapper_type == ROM_PROGRAM))) {
			result = NES_ERR;
			goto exit;
		}
	}

	nes_bus_unload();
	nes_test_initialize();
	nes_bus_load(&g_test.configuration);

	for(address = VIDEO_ROM_BEGIN; address <= VIDEO_ROM_END; ++address) {
		g_test.mapper_data = rand();

		if(ASSERT((nes_bus_read(BUS_VIDEO, address) == g_test.mapper_data)
				&& (g_test.mapper_address == address - VIDEO_ROM_BEGIN)
				&& (g_test.mapper_type == ROM_CHARACTER))) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(address = VIDEO_RAM_BEGIN; address <= VIDEO_RAM_END; ++address) {

		if(ASSERT(nes_bus_read(BUS_VIDEO, address) == VIDEO_RAM_FILL)) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(address = VIDEO_PALETTE_RAM_BEGIN; address <= VIDEO_PALETTE_RAM_END; ++address) {

		if(ASSERT(nes_bus_read(BUS_VIDEO, address) == VIDEO_PALETTE_RAM_FILL)) {
			result = NES_ERR;
			goto exit;
		}
	}

	if(ASSERT(nes_bus_read(BUS_VIDEO, address) == 0x00)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus_unload();

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_bus_unload(void)
{
	int result = NES_OK;

	nes_test_initialize();
	nes_bus_load(&g_test.configuration);
	nes_bus_unload();

	if(ASSERT(g_test.mapper_unload)) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_bus_write(void)
{
	uint8_t data;
	uint32_t address;
	int result = NES_OK;

	nes_test_initialize();
	nes_bus_load(&g_test.configuration);

	for(address = OBJECT_RAM_BEGIN; address <= OBJECT_RAM_END; ++address) {
		nes_bus_write(BUS_OBJECT, address, data = rand());

		if(ASSERT(nes_bus_read(BUS_OBJECT, address) == data)) {
			result = NES_ERR;
			goto exit;
		}
	}

	nes_bus_unload();
	nes_test_initialize();
	nes_bus_load(&g_test.configuration);

	for(address = PROCESSOR_RAM_BEGIN; address <= PROCESSOR_RAM_END; ++address) {
		nes_bus_write(BUS_PROCESSOR, address, data = rand());

		if(ASSERT(nes_bus_read(BUS_PROCESSOR, address) == data)) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(address = PROCESSOR_WORK_RAM_BEGIN; address <= PROCESSOR_WORK_RAM_END; ++address) {
		g_test.mapper_data = rand();

		if(ASSERT((nes_bus_read(BUS_PROCESSOR, address) == g_test.mapper_data)
				&& (g_test.mapper_address == address - PROCESSOR_WORK_RAM_BEGIN)
				&& (g_test.mapper_type == RAM_PROGRAM))) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(address = PROCESSOR_ROM_0_BEGIN; address <= PROCESSOR_ROM_1_END; ++address) {
		g_test.mapper_data = rand();

		if(ASSERT((nes_bus_read(BUS_PROCESSOR, address) == g_test.mapper_data)
				&& (g_test.mapper_address == address - PROCESSOR_ROM_0_BEGIN)
				&& (g_test.mapper_type == ROM_PROGRAM))) {
			result = NES_ERR;
			goto exit;
		}
	}

	nes_bus_unload();
	nes_test_initialize();
	nes_bus_load(&g_test.configuration);

	for(address = VIDEO_ROM_BEGIN; address <= VIDEO_ROM_END; ++address) {
		g_test.mapper_data = rand();

		if(ASSERT((nes_bus_read(BUS_VIDEO, address) == g_test.mapper_data)
				&& (g_test.mapper_address == address - VIDEO_ROM_BEGIN)
				&& (g_test.mapper_type == ROM_CHARACTER))) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(address = VIDEO_RAM_BEGIN; address <= VIDEO_RAM_END; ++address) {
		nes_bus_write(BUS_VIDEO, address, data = rand());

		if(ASSERT(nes_bus_read(BUS_VIDEO, address) == data)) {
			result = NES_ERR;
			goto exit;
		}
	}

	for(address = VIDEO_PALETTE_RAM_BEGIN; address <= VIDEO_PALETTE_RAM_END; ++address) {
		nes_bus_write(BUS_VIDEO, address, data = rand());

		if(ASSERT(nes_bus_read(BUS_VIDEO, address) == data)) {
			result = NES_ERR;
			goto exit;
		}
	}

	if(ASSERT(nes_bus_read(BUS_VIDEO, address) == 0x00)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus_unload();

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
	nes_bus_unload();
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
