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

#include "./processor_type.h"

static nes_test_processor_t g_test = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t
nes_bus_read(
	__in int bus,
	__in uint16_t address
	)
{
	uint8_t result = 0;

	switch(bus) {
		case BUS_PROCESSOR:
			result = g_test.memory.data[address];
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
			g_test.memory.data[address] = data;
			break;
		default:
			break;
	}
}

int
nes_test_processor_interrupt_maskable(void)
{
	int result = NES_OK;
	nes_processor_register_t address = { .word = rand() };

	nes_test_initialize();
	nes_processor_write_word(&g_test.processor, MASKABLE_ADDRESS, address.word + 2);
	nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
	nes_processor_reset(&g_test.processor);
	nes_processor_interrupt(&g_test.processor, true);

	if(ASSERT((g_test.processor.cycles == RESET_CYCLES)
			&& (g_test.processor.program_counter.word == address.word)
			&& (g_test.processor.stack_pointer.low == 0xfd)
			&& (g_test.processor.status.low == 0x34)
			&& (g_test.processor.accumulator.low == 0)
			&& (g_test.processor.index_x.low == 0)
			&& (g_test.processor.index_y.low == 0)
			&& (g_test.processor.pending.maskable == true))) {
		result = NES_ERR;
		goto exit;
	}

	while(g_test.processor.cycles > 0) {
		nes_processor_step(&g_test.processor);
	}

	nes_processor_step(&g_test.processor);

	if(ASSERT((g_test.processor.cycles == 1)
			&& (g_test.processor.program_counter.word == address.word + 1)
			&& (g_test.processor.stack_pointer.low == 0xfd)
			&& (g_test.processor.status.low == 0x34)
			&& (g_test.processor.accumulator.low == 0)
			&& (g_test.processor.index_x.low == 0)
			&& (g_test.processor.index_y.low == 0)
			&& (g_test.processor.pending.maskable == true))) {
		result = NES_ERR;
		goto exit;
	}

	g_test.processor.status.interrupt_disabled = false;

	while(g_test.processor.cycles > 0) {
		nes_processor_step(&g_test.processor);
	}

	nes_processor_step(&g_test.processor);

	if(ASSERT((g_test.processor.cycles == MASKABLE_CYCLES - 1)
			&& (g_test.processor.program_counter.word == address.word + 2)
			&& (g_test.processor.stack_pointer.low == 0xfd - 3)
			&& (g_test.processor.status.low == 0x34)
			&& (g_test.processor.accumulator.low == 0)
			&& (g_test.processor.index_x.low == 0)
			&& (g_test.processor.index_y.low == 0)
			&& (g_test.processor.pending.maskable == false)
			&& (nes_processor_pull(&g_test.processor) == 0x20)
			&& (nes_processor_pull_word(&g_test.processor) == address.word + 1))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_interrupt_non_maskable(void)
{
	int result = NES_OK;
	nes_processor_register_t address = { .word = rand() };

	nes_test_initialize();
	nes_processor_write_word(&g_test.processor, NON_MASKABLE_ADDRESS, address.word + 2);
	nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
	nes_processor_reset(&g_test.processor);
	nes_processor_interrupt(&g_test.processor, false);

	if(ASSERT((g_test.processor.cycles == RESET_CYCLES)
			&& (g_test.processor.program_counter.word == address.word)
			&& (g_test.processor.stack_pointer.low == 0xfd)
			&& (g_test.processor.status.low == 0x34)
			&& (g_test.processor.accumulator.low == 0)
			&& (g_test.processor.index_x.low == 0)
			&& (g_test.processor.index_y.low == 0)
			&& (g_test.processor.pending.non_maskable == true))) {
		result = NES_ERR;
		goto exit;
	}

	while(g_test.processor.cycles > 0) {
		nes_processor_step(&g_test.processor);
	}

	nes_processor_step(&g_test.processor);

	if(ASSERT((g_test.processor.cycles == NON_MASKABLE_CYCLES - 1)
			&& (g_test.processor.program_counter.word == address.word + 2)
			&& (g_test.processor.stack_pointer.low == 0xfd - 3)
			&& (g_test.processor.status.low == 0x34)
			&& (g_test.processor.accumulator.low == 0)
			&& (g_test.processor.index_x.low == 0)
			&& (g_test.processor.index_y.low == 0)
			&& (g_test.processor.pending.non_maskable == false)
			&& (nes_processor_pull(&g_test.processor) == 0x24)
			&& (nes_processor_pull_word(&g_test.processor) == address.word))) {
		result = NES_ERR;
		goto exit;
	}

	g_test.processor.status.interrupt_disabled = false;
	nes_processor_interrupt(&g_test.processor, false);
	nes_processor_write_word(&g_test.processor, NON_MASKABLE_ADDRESS, address.word + 4);

	while(g_test.processor.cycles > 0) {
		nes_processor_step(&g_test.processor);
	}

	nes_processor_step(&g_test.processor);

	if(ASSERT((g_test.processor.cycles == NON_MASKABLE_CYCLES - 1)
			&& (g_test.processor.program_counter.word == address.word + 4)
			&& (g_test.processor.stack_pointer.low == 0xfd - 3)
			&& (g_test.processor.status.low == 0x34)
			&& (g_test.processor.accumulator.low == 0)
			&& (g_test.processor.index_x.low == 0)
			&& (g_test.processor.index_y.low == 0)
			&& (g_test.processor.pending.non_maskable == false)
			&& (nes_processor_pull(&g_test.processor) == 0x20)
			&& (nes_processor_pull_word(&g_test.processor) == address.word + 2))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_reset(void)
{
	int result = NES_OK;
	nes_processor_register_t address = { .word = rand() };

	nes_test_initialize();
	nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
	nes_processor_reset(&g_test.processor);

	if(ASSERT((g_test.processor.cycles == RESET_CYCLES)
			&& (g_test.processor.program_counter.word == address.word)
			&& (g_test.processor.stack_pointer.low == 0xfd)
			&& (g_test.processor.status.low == 0x34)
			&& (g_test.processor.accumulator.low == 0)
			&& (g_test.processor.index_x.low == 0)
			&& (g_test.processor.index_y.low == 0))) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TEST_TRACE(result);

	return result;
}

void
nes_test_initialize(void)
{
	memset(g_test.memory.data, 0xea, g_test.memory.length);
	memset(&g_test.processor, 0, sizeof(g_test.processor));
}

int
main(
	__in int argc,
	__in char *argv[]
	)
{
	int result;

	srand(time(NULL));

	if((result = nes_buffer_allocate(&g_test.memory, UINT16_MAX + 1)) != NES_OK) {
		goto exit;
	}

	for(size_t test = 0; test < TEST_COUNT(TEST); ++test) {

		if(TEST[test]() != NES_OK) {
			result = NES_ERR;
		}
	}

exit:
	nes_buffer_free(&g_test.memory);

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
