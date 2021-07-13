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

#include "./action_type.h"

static nes_test_action_t g_test = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

nes_bus_t *
nes_bus(void)
{
	return &g_test.bus;
}

uint8_t
nes_bus_read(
	__in int bus,
	__in uint16_t address
	)
{
	uint8_t result = 0;

	switch(bus) {
		case BUS_PROCESSOR:
			g_test.address.word = address;
			result = g_test.data.low;
			break;
		default:
			break;
	}

	return result;
}

void
nes_bus_write(
	__in int bus,
	__in uint16_t address,
	__in uint8_t data
	)
{

	switch(bus) {
		case BUS_PROCESSOR:
			g_test.address.word = address;
			g_test.data.low = data;
			break;
		default:
			break;
	}
}

void
nes_processor_step(
        __inout nes_processor_t *processor
        )
{
	return;
}

int
nes_service_poll(void)
{
	return NES_OK;
}

int
nes_service_show(void)
{
	return NES_OK;
}

bool
nes_video_step(
        __inout nes_video_t *video
        )
{
	return true;
}

int
nes_test_action_bus_read(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.data.low = rand();
	g_test.request.type = NES_ACTION_BUS_READ;
	g_test.request.address.word = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_BUS_READ)
			&& (g_test.response.address.word == g_test.request.address.word)
			&& (g_test.response.data.low == g_test.data.low)
			&& (g_test.address.word == g_test.request.address.word))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_action_bus_write(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.request.type = NES_ACTION_BUS_WRITE;
	g_test.request.address.word = rand();
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.address.word == g_test.request.address.word)
			&& (g_test.data.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_action_cartridge_header(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.request.type = NES_ACTION_CARTRIDGE_HEADER;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;
	nes_bus()->mapper.cartridge.header = (const nes_header_t *)1;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_CARTRIDGE_HEADER)
			&& (g_test.response.ptr == nes_bus()->mapper.cartridge.header))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_action_mapper_read(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.bus.mapper.rom_program[ROM_BANK_0] = rand();
	g_test.request.type = NES_ACTION_MAPPER_READ;
	g_test.request.address.word = NES_MAPPER_PROGRAM_ROM_0;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_MAPPER_READ)
			&& (g_test.response.address.word == NES_MAPPER_PROGRAM_ROM_0)
			&& (g_test.response.data.dword == g_test.bus.mapper.rom_program[ROM_BANK_0]))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.mapper.rom_program[ROM_BANK_1] = rand();
	g_test.request.type = NES_ACTION_MAPPER_READ;
	g_test.request.address.word = NES_MAPPER_PROGRAM_ROM_1;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_MAPPER_READ)
			&& (g_test.response.address.word == NES_MAPPER_PROGRAM_ROM_1)
			&& (g_test.response.data.dword == g_test.bus.mapper.rom_program[ROM_BANK_1]))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.mapper.ram_program = rand();
	g_test.request.type = NES_ACTION_MAPPER_READ;
	g_test.request.address.word = NES_MAPPER_PROGRAM_RAM;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_MAPPER_READ)
			&& (g_test.response.address.word == NES_MAPPER_PROGRAM_RAM)
			&& (g_test.response.data.dword == g_test.bus.mapper.ram_program))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.mapper.rom_character = rand();
	g_test.request.type = NES_ACTION_MAPPER_READ;
	g_test.request.address.word = NES_MAPPER_CHARACTER_ROM;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_MAPPER_READ)
			&& (g_test.response.address.word == NES_MAPPER_CHARACTER_ROM)
			&& (g_test.response.data.dword == g_test.bus.mapper.rom_character))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.mapper.ram_character = rand();
	g_test.request.type = NES_ACTION_MAPPER_READ;
	g_test.request.address.word = NES_MAPPER_CHARACTER_RAM;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_MAPPER_READ)
			&& (g_test.response.address.word == NES_MAPPER_CHARACTER_RAM)
			&& (g_test.response.data.dword == g_test.bus.mapper.ram_character))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_action_mapper_write(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.request.type = NES_ACTION_MAPPER_WRITE;
	g_test.request.address.word = NES_MAPPER_PROGRAM_ROM_0;
	g_test.request.data.dword = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.mapper.rom_program[ROM_BANK_0] == g_test.request.data.dword))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_MAPPER_WRITE;
	g_test.request.address.word = NES_MAPPER_PROGRAM_ROM_1;
	g_test.request.data.dword = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.mapper.rom_program[ROM_BANK_1] == g_test.request.data.dword))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_MAPPER_WRITE;
	g_test.request.address.word = NES_MAPPER_PROGRAM_RAM;
	g_test.request.data.dword = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.mapper.ram_program == g_test.request.data.dword))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_MAPPER_WRITE;
	g_test.request.address.word = NES_MAPPER_CHARACTER_ROM;
	g_test.request.data.dword = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.mapper.rom_character == g_test.request.data.dword))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_MAPPER_WRITE;
	g_test.request.address.word = NES_MAPPER_CHARACTER_RAM;
	g_test.request.data.dword = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.mapper.ram_character == g_test.request.data.dword))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_action_processor_read(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.bus.processor.program_counter.word = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address.word = NES_PROCESSOR_PROGRAM_COUNTER;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address.word == NES_PROCESSOR_PROGRAM_COUNTER)
			&& (g_test.response.data.word == g_test.bus.processor.program_counter.word))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.stack_pointer.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address.word = NES_PROCESSOR_STACK_POINTER;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address.word == NES_PROCESSOR_STACK_POINTER)
			&& (g_test.response.data.word == (STACK_ADDRESS | g_test.bus.processor.stack_pointer.low)))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.status.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address.word = NES_PROCESSOR_STATUS;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address.word == NES_PROCESSOR_STATUS)
			&& (g_test.response.data.low == g_test.bus.processor.status.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.status.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address.word = NES_PROCESSOR_PENDING;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address.word == NES_PROCESSOR_PENDING)
			&& (g_test.response.data.low == g_test.bus.processor.pending.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.accumulator.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address.word = NES_PROCESSOR_ACCUMULATOR;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address.word == NES_PROCESSOR_ACCUMULATOR)
			&& (g_test.response.data.low == g_test.bus.processor.accumulator.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.index_x.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address.word = NES_PROCESSOR_INDEX_X;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address.word == NES_PROCESSOR_INDEX_X)
			&& (g_test.response.data.low == g_test.bus.processor.index_x.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.index_y.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address.word = NES_PROCESSOR_INDEX_Y;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address.word == NES_PROCESSOR_INDEX_Y)
			&& (g_test.response.data.low == g_test.bus.processor.index_y.low))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_action_processor_write(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address.word = NES_PROCESSOR_PROGRAM_COUNTER;
	g_test.request.data.word = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.program_counter.word == g_test.request.data.word))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address.word = NES_PROCESSOR_STACK_POINTER;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.stack_pointer.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address.word = NES_PROCESSOR_STATUS;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.status.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address.word = NES_PROCESSOR_PENDING;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.pending.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address.word = NES_PROCESSOR_ACCUMULATOR;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.accumulator.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address.word = NES_PROCESSOR_INDEX_X;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.index_x.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address.word = NES_PROCESSOR_INDEX_Y;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.index_y.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_action_video_read(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.bus.video.control.raw = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_CONTROL;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_CONTROL)
			&& (g_test.response.data.low == g_test.bus.video.control.raw))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.video.mask.raw = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_MASK;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_MASK)
			&& (g_test.response.data.low == g_test.bus.video.mask.raw))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.video.status.raw = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_STATUS;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_STATUS)
			&& (g_test.response.data.low == g_test.bus.video.status.raw))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.video.object_address.word = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_OBJECT_ADDRESS;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_OBJECT_ADDRESS)
			&& (g_test.response.data.word == g_test.bus.video.object_address.word))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.video.object_data.low = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_OBJECT_DATA;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_OBJECT_DATA)
			&& (g_test.response.data.low == g_test.bus.video.object_data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.video.scroll_x.low = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_SCROLL_X;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_SCROLL_X)
			&& (g_test.response.data.low == g_test.bus.video.scroll_x.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.video.scroll_y.low = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_SCROLL_Y;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_SCROLL_Y)
			&& (g_test.response.data.low == g_test.bus.video.scroll_y.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.video.address.word = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_ADDRESS;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_ADDRESS)
			&& (g_test.response.data.word == g_test.bus.video.address.word))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.video.data.low = rand();
	g_test.request.type = NES_ACTION_VIDEO_READ;
	g_test.request.address.word = NES_VIDEO_DATA;
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_VIDEO_READ)
			&& (g_test.response.address.word == NES_VIDEO_DATA)
			&& (g_test.response.data.low == g_test.bus.video.data.low))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_action_video_write(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_CONTROL;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.control.raw == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_MASK;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.mask.raw == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_STATUS;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.status.raw == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_OBJECT_ADDRESS;
	g_test.request.data.word = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.object_address.word == g_test.request.data.word))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_OBJECT_DATA;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.object_data.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_SCROLL_X;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.scroll_x.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_SCROLL_Y;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.scroll_y.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_ADDRESS;
	g_test.request.data.word = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.address.word == g_test.request.data.word))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_VIDEO_WRITE;
	g_test.request.address.word = NES_VIDEO_DATA;
	g_test.request.data.low = rand();
	nes_bus()->loaded = false;

	if(ASSERT(nes_action(&g_test.request, &g_test.response) != NES_OK)) {
		result = NES_ERR;
		goto exit;
	}

	nes_bus()->loaded = true;

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.video.data.low == g_test.request.data.low))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
}

void
nes_test_initialize(void)
{
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

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
