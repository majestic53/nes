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
nes_test_processor_execute_bit(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t accumulator = { .low = rand() }, address = { .word = (rand() % 0x2000) + 512 },
			address_indirect = { .word = (rand() % 0x2000) + 0x2000 + 512 }, data = {}, status = {};

		if(ASSERT((INSTRUCTION[0x2c].opcode == OPCODE_BIT)
				&& (INSTRUCTION[0x2c].mode == MODE_ABSOLUTE)
				&& (INSTRUCTION[0x2c].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x2c);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		data.low = rand();
		nes_processor_write(&g_test.processor, address_indirect.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.overflow = data.overflow;
		status.zero = !(g_test.processor.accumulator.low & data.low);
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, address_indirect.word) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x24].opcode == OPCODE_BIT)
				&& (INSTRUCTION[0x24].mode == MODE_ZEROPAGE)
				&& (INSTRUCTION[0x24].cycles == 3))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x24);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		data.low = rand();
		nes_processor_write(&g_test.processor, address_indirect.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.overflow = data.overflow;
		status.zero = !(g_test.processor.accumulator.low & data.low);
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 2)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, address_indirect.low) == data.low))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_bitwise(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t accumulator = {}, address = { .word = (rand() % 0x2000) + 512 },
			address_absolute = { .word = (rand() % 0x2000) + 0x4000 + 512 },
			address_indirect = { .word = (rand() % 0x2000) + 0x2000 + 512 },
			address_offset = {}, data = {}, index = {}, status = {};

		if(ASSERT((INSTRUCTION[0x2d].opcode == OPCODE_AND)
				&& (INSTRUCTION[0x2d].mode == MODE_ABSOLUTE)
				&& (INSTRUCTION[0x2d].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x2d);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low &= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x3d].opcode == OPCODE_AND)
				&& (INSTRUCTION[0x3d].mode == MODE_ABSOLUTE_X)
				&& (INSTRUCTION[0x3d].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x3d);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low &= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_indirect.word + index.low;

		if(ASSERT((g_test.processor.cycles == 3 + ((address_indirect.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x39].opcode == OPCODE_AND)
				&& (INSTRUCTION[0x39].mode == MODE_ABSOLUTE_Y)
				&& (INSTRUCTION[0x39].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x39);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_y.low = index.low;
		accumulator.low &= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_indirect.word + index.low;

		if(ASSERT((g_test.processor.cycles == 3 + ((address_indirect.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == index.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x29].opcode == OPCODE_AND)
				&& (INSTRUCTION[0x29].mode == MODE_IMMEDIATE)
				&& (INSTRUCTION[0x29].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x29);
		nes_processor_write(&g_test.processor, address.word + 1, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low &= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x21].opcode == OPCODE_AND)
				&& (INSTRUCTION[0x21].mode == MODE_INDIRECT_X)
				&& (INSTRUCTION[0x21].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x21);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, address_absolute.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low + 1) & UINT8_MAX, address_absolute.high);
		nes_processor_write_word(&g_test.processor, address_absolute.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low &= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x31].opcode == OPCODE_AND)
				&& (INSTRUCTION[0x31].mode == MODE_INDIRECT_Y)
				&& (INSTRUCTION[0x31].cycles == 5))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x31);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, address_indirect.low, address_absolute.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + 1) & UINT8_MAX, address_absolute.high);
		nes_processor_write_word(&g_test.processor, address_absolute.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_y.low = index.low;
		accumulator.low &= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_absolute.word + index.low;

		if(ASSERT((g_test.processor.cycles == 4 + ((address_absolute.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == index.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x25].opcode == OPCODE_AND)
				&& (INSTRUCTION[0x25].mode == MODE_ZEROPAGE)
				&& (INSTRUCTION[0x25].cycles == 3))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x25);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, address_indirect.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low &= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 2)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x35].opcode == OPCODE_AND)
				&& (INSTRUCTION[0x35].mode == MODE_ZEROPAGE_X)
				&& (INSTRUCTION[0x35].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x35);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low &= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x4d].opcode == OPCODE_EOR)
				&& (INSTRUCTION[0x4d].mode == MODE_ABSOLUTE)
				&& (INSTRUCTION[0x4d].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x4d);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low ^= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x5d].opcode == OPCODE_EOR)
				&& (INSTRUCTION[0x5d].mode == MODE_ABSOLUTE_X)
				&& (INSTRUCTION[0x5d].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x5d);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low ^= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_indirect.word + index.low;

		if(ASSERT((g_test.processor.cycles == 3 + ((address_indirect.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x59].opcode == OPCODE_EOR)
				&& (INSTRUCTION[0x59].mode == MODE_ABSOLUTE_Y)
				&& (INSTRUCTION[0x59].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x59);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_y.low = index.low;
		accumulator.low ^= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_indirect.word + index.low;

		if(ASSERT((g_test.processor.cycles == 3 + ((address_indirect.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == index.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x49].opcode == OPCODE_EOR)
				&& (INSTRUCTION[0x49].mode == MODE_IMMEDIATE)
				&& (INSTRUCTION[0x49].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x49);
		nes_processor_write(&g_test.processor, address.word + 1, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low ^= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x41].opcode == OPCODE_EOR)
				&& (INSTRUCTION[0x41].mode == MODE_INDIRECT_X)
				&& (INSTRUCTION[0x41].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x41);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, address_absolute.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low + 1) & UINT8_MAX, address_absolute.high);
		nes_processor_write_word(&g_test.processor, address_absolute.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low ^= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x51].opcode == OPCODE_EOR)
				&& (INSTRUCTION[0x51].mode == MODE_INDIRECT_Y)
				&& (INSTRUCTION[0x51].cycles == 5))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x51);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, address_indirect.low, address_absolute.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + 1) & UINT8_MAX, address_absolute.high);
		nes_processor_write_word(&g_test.processor, address_absolute.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_y.low = index.low;
		accumulator.low ^= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_absolute.word + index.low;

		if(ASSERT((g_test.processor.cycles == 4 + ((address_absolute.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == index.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x45].opcode == OPCODE_EOR)
				&& (INSTRUCTION[0x45].mode == MODE_ZEROPAGE)
				&& (INSTRUCTION[0x45].cycles == 3))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x45);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, address_indirect.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low ^= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 2)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x55].opcode == OPCODE_EOR)
				&& (INSTRUCTION[0x55].mode == MODE_ZEROPAGE_X)
				&& (INSTRUCTION[0x55].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x55);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low ^= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x0d].opcode == OPCODE_ORA)
				&& (INSTRUCTION[0x0d].mode == MODE_ABSOLUTE)
				&& (INSTRUCTION[0x0d].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x0d);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low |= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x1d].opcode == OPCODE_ORA)
				&& (INSTRUCTION[0x1d].mode == MODE_ABSOLUTE_X)
				&& (INSTRUCTION[0x1d].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x1d);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low |= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_indirect.word + index.low;

		if(ASSERT((g_test.processor.cycles == 3 + ((address_indirect.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x19].opcode == OPCODE_ORA)
				&& (INSTRUCTION[0x19].mode == MODE_ABSOLUTE_Y)
				&& (INSTRUCTION[0x19].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x19);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_y.low = index.low;
		accumulator.low |= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_indirect.word + index.low;

		if(ASSERT((g_test.processor.cycles == 3 + ((address_indirect.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == index.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x09].opcode == OPCODE_ORA)
				&& (INSTRUCTION[0x09].mode == MODE_IMMEDIATE)
				&& (INSTRUCTION[0x09].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x09);
		nes_processor_write(&g_test.processor, address.word + 1, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low |= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x01].opcode == OPCODE_ORA)
				&& (INSTRUCTION[0x01].mode == MODE_INDIRECT_X)
				&& (INSTRUCTION[0x01].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x01);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, address_absolute.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low + 1) & UINT8_MAX, address_absolute.high);
		nes_processor_write_word(&g_test.processor, address_absolute.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low |= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x11].opcode == OPCODE_ORA)
				&& (INSTRUCTION[0x11].mode == MODE_INDIRECT_Y)
				&& (INSTRUCTION[0x11].cycles == 5))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x11);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, address_indirect.low, address_absolute.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + 1) & UINT8_MAX, address_absolute.high);
		nes_processor_write_word(&g_test.processor, address_absolute.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_y.low = index.low;
		accumulator.low |= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);
		address_offset.word = address_absolute.word + index.low;

		if(ASSERT((g_test.processor.cycles == 4 + ((address_absolute.high != address_offset.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == index.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x05].opcode == OPCODE_ORA)
				&& (INSTRUCTION[0x05].mode == MODE_ZEROPAGE)
				&& (INSTRUCTION[0x05].cycles == 3))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x05);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, address_indirect.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		accumulator.low |= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 2)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x15].opcode == OPCODE_ORA)
				&& (INSTRUCTION[0x15].mode == MODE_ZEROPAGE_X)
				&& (INSTRUCTION[0x15].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		accumulator.low = rand();
		data.low = rand();
		index.low = rand();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x15);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = accumulator.low;
		g_test.processor.index_x.low = index.low;
		accumulator.low |= data.low;
		status.low = 0x34;
		status.negative = accumulator.negative;
		status.zero = !accumulator.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == accumulator.low)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_branch(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x2000) + 512 }, address_base = {}, address_relative = {},
			data = { .low = rand() }, status = {};

		if(ASSERT((INSTRUCTION[0x90].opcode == OPCODE_BCC)
				&& (INSTRUCTION[0x90].mode == MODE_RELATIVE)
				&& (INSTRUCTION[0x90].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x90);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = 0x34;
		status.carry = true;
		g_test.processor.status.carry = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x90);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

                if(data.negative) {
                        data.high = UINT8_MAX;
                }

		status.low = 0x34;
		status.carry = false;
		g_test.processor.status.carry = false;
		nes_processor_step(&g_test.processor);
		address_base.word = address.word + 2;
		address_relative.word = address_base.word + data.word;

		if(ASSERT((g_test.processor.cycles == 2 + ((address_base.high != address_relative.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == address_relative.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xb0].opcode == OPCODE_BCS)
				&& (INSTRUCTION[0xb0].mode == MODE_RELATIVE)
				&& (INSTRUCTION[0xb0].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xb0);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = 0x34;
		status.carry = false;
		g_test.processor.status.carry = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xb0);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

                if(data.negative) {
                        data.high = UINT8_MAX;
                }

		status.low = 0x34;
		status.carry = true;
		g_test.processor.status.carry = true;
		nes_processor_step(&g_test.processor);
		address_base.word = address.word + 2;
		address_relative.word = address_base.word + data.word;

		if(ASSERT((g_test.processor.cycles == 2 + ((address_base.high != address_relative.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == address_relative.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xf0].opcode == OPCODE_BEQ)
				&& (INSTRUCTION[0xf0].mode == MODE_RELATIVE)
				&& (INSTRUCTION[0xf0].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xf0);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = 0x34;
		status.zero = false;
		g_test.processor.status.zero = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xf0);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

                if(data.negative) {
                        data.high = UINT8_MAX;
                }

		status.low = 0x34;
		status.zero = true;
		g_test.processor.status.zero = true;
		nes_processor_step(&g_test.processor);
		address_base.word = address.word + 2;
		address_relative.word = address_base.word + data.word;

		if(ASSERT((g_test.processor.cycles == 2 + ((address_base.high != address_relative.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == address_relative.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x30].opcode == OPCODE_BMI)
				&& (INSTRUCTION[0x30].mode == MODE_RELATIVE)
				&& (INSTRUCTION[0x30].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x30);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = 0x34;
		status.negative = false;
		g_test.processor.status.negative = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x30);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

                if(data.negative) {
                        data.high = UINT8_MAX;
                }

		status.low = 0x34;
		status.negative = true;
		g_test.processor.status.negative = true;
		nes_processor_step(&g_test.processor);
		address_base.word = address.word + 2;
		address_relative.word = address_base.word + data.word;

		if(ASSERT((g_test.processor.cycles == 2 + ((address_base.high != address_relative.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == address_relative.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xd0].opcode == OPCODE_BNE)
				&& (INSTRUCTION[0xd0].mode == MODE_RELATIVE)
				&& (INSTRUCTION[0xd0].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xd0);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = 0x34;
		status.zero = true;
		g_test.processor.status.zero = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xd0);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

                if(data.negative) {
                        data.high = UINT8_MAX;
                }

		status.low = 0x34;
		status.zero = false;
		g_test.processor.status.zero = false;
		nes_processor_step(&g_test.processor);
		address_base.word = address.word + 2;
		address_relative.word = address_base.word + data.word;

		if(ASSERT((g_test.processor.cycles == 2 + ((address_base.high != address_relative.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == address_relative.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x10].opcode == OPCODE_BPL)
				&& (INSTRUCTION[0x10].mode == MODE_RELATIVE)
				&& (INSTRUCTION[0x10].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x10);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = 0x34;
		status.negative = true;
		g_test.processor.status.negative = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x10);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

                if(data.negative) {
                        data.high = UINT8_MAX;
                }

		status.low = 0x34;
		status.negative = false;
		g_test.processor.status.negative = false;
		nes_processor_step(&g_test.processor);
		address_base.word = address.word + 2;
		address_relative.word = address_base.word + data.word;

		if(ASSERT((g_test.processor.cycles == 2 + ((address_base.high != address_relative.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == address_relative.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x50].opcode == OPCODE_BVC)
				&& (INSTRUCTION[0x50].mode == MODE_RELATIVE)
				&& (INSTRUCTION[0x50].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x50);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = 0x34;
		status.overflow = true;
		g_test.processor.status.overflow = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x50);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

                if(data.negative) {
                        data.high = UINT8_MAX;
                }

		status.low = 0x34;
		status.overflow = false;
		g_test.processor.status.overflow = false;
		nes_processor_step(&g_test.processor);
		address_base.word = address.word + 2;
		address_relative.word = address_base.word + data.word;

		if(ASSERT((g_test.processor.cycles == 2 + ((address_base.high != address_relative.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == address_relative.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x70].opcode == OPCODE_BVS)
				&& (INSTRUCTION[0x70].mode == MODE_RELATIVE)
				&& (INSTRUCTION[0x70].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x70);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = 0x34;
		status.overflow = false;
		g_test.processor.status.overflow = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x70);
		nes_processor_write(&g_test.processor, address.word + 1, data.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

                if(data.negative) {
                        data.high = UINT8_MAX;
                }

		status.low = 0x34;
		status.overflow = true;
		g_test.processor.status.overflow = true;
		nes_processor_step(&g_test.processor);
		address_base.word = address.word + 2;
		address_relative.word = address_base.word + data.word;

		if(ASSERT((g_test.processor.cycles == 2 + ((address_base.high != address_relative.high) ? 1 : 0))
				&& (g_test.processor.program_counter.word == address_relative.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_breakpoint(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

		if(ASSERT((INSTRUCTION[0x00].opcode == OPCODE_BRK)
				&& (INSTRUCTION[0x00].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x00].cycles == 7))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, MASKABLE_ADDRESS, address.word + 256);
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, 0x00);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == MASKABLE_CYCLES - 1)
				&& (g_test.processor.program_counter.word == ((address.word + 256) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd - 3)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_pull(&g_test.processor) == 0x34)
				&& (nes_processor_pull_word(&g_test.processor) == ((address.word + 1) & UINT16_MAX)))) {
			result = NES_ERR;
			goto exit;
		}

		nes_processor_write_word(&g_test.processor, MASKABLE_ADDRESS, address.word + 256);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.interrupt_disabled = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == MASKABLE_CYCLES - 1)
				&& (g_test.processor.program_counter.word == ((address.word + 256) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd - 3)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_pull(&g_test.processor) == 0x30)
				&& (nes_processor_pull_word(&g_test.processor) == ((address.word + 1) & UINT16_MAX)))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_clear(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

		if(ASSERT((INSTRUCTION[0x18].opcode == OPCODE_CLC)
				&& (INSTRUCTION[0x18].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x18].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x18);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.carry = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x18);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.carry = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xd8].opcode == OPCODE_CLD)
				&& (INSTRUCTION[0xd8].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xd8].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xd8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.decimal = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xd8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.decimal = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x58].opcode == OPCODE_CLI)
				&& (INSTRUCTION[0x58].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x58].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x58);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.interrupt_disabled = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x30)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x58);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.interrupt_disabled = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x30)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xb8].opcode == OPCODE_CLV)
				&& (INSTRUCTION[0xb8].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xb8].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xb8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.overflow = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xb8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.overflow = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_decrement(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x2000) + 512 }, address_indirect = { .word = (rand() % 0x2000) + 0x2000 + 512 },
			data = {}, index = { .low = rand() }, status = {};

		if(ASSERT((INSTRUCTION[0xce].opcode == OPCODE_DEC)
				&& (INSTRUCTION[0xce].mode == MODE_ABSOLUTE)
				&& (INSTRUCTION[0xce].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xce);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		data.low = rand();
		nes_processor_write(&g_test.processor, address_indirect.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		--data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, address_indirect.word) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xde].opcode == OPCODE_DEC)
				&& (INSTRUCTION[0xde].mode == MODE_ABSOLUTE_X)
				&& (INSTRUCTION[0xde].cycles == 7))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xde);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		data.low = rand();
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		--data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		g_test.processor.index_x.low = index.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 6)
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, address_indirect.word + g_test.processor.index_x.low) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xc6].opcode == OPCODE_DEC)
				&& (INSTRUCTION[0xc6].mode == MODE_ZEROPAGE)
				&& (INSTRUCTION[0xc6].cycles == 5))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xc6);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		data.low = rand();
		nes_processor_write(&g_test.processor, address_indirect.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		--data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 4)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, address_indirect.low) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xd6].opcode == OPCODE_DEC)
				&& (INSTRUCTION[0xd6].mode == MODE_ZEROPAGE_X)
				&& (INSTRUCTION[0xd6].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xd6);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		data.low = rand();
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		--data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		g_test.processor.index_x.low = index.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, (address_indirect.word + g_test.processor.index_x.low) & UINT8_MAX) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xca].opcode == OPCODE_DEX)
				&& (INSTRUCTION[0xca].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xca].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xca);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		data.low = rand();
		g_test.processor.index_x.low = data.low;
		--data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x88].opcode == OPCODE_DEY)
				&& (INSTRUCTION[0x88].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x88].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x88);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		data.low = rand();
		g_test.processor.index_y.low = data.low;
		--data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == data.low))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_illegal(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {

		for(uint16_t opcode = 0; opcode < sizeof(ILLEGAL_OPCODE); ++opcode) {
			nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };
			const nes_processor_instruction_t *instruction = &INSTRUCTION[ILLEGAL_OPCODE[opcode]];

			if(ASSERT((instruction->opcode == OPCODE_XXX)
					&& (instruction->mode == MODE_IMPLIED)
					&& (instruction->cycles == 2))) {
				result = NES_ERR;
				goto exit;
			}

			nes_test_initialize();
			nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
			nes_processor_write(&g_test.processor, address.word, ILLEGAL_OPCODE[opcode]);
			nes_processor_reset(&g_test.processor);

			while(g_test.processor.cycles > 0) {
				nes_processor_step(&g_test.processor);
			}

			nes_processor_step(&g_test.processor);

			if(ASSERT((g_test.processor.cycles == 1)
					&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
					&& (g_test.processor.stack_pointer.low == 0xfd)
					&& (g_test.processor.status.low == 0x34)
					&& (g_test.processor.accumulator.low == 0)
					&& (g_test.processor.index_x.low == 0)
					&& (g_test.processor.index_y.low == 0))) {
				result = NES_ERR;
				goto exit;
			}
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_increment(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x2000) + 512 }, address_indirect = { .word = (rand() % 0x2000) + 0x2000 + 512 },
			data = {}, index = { .low = rand() }, status = {};

		if(ASSERT((INSTRUCTION[0xee].opcode == OPCODE_INC)
				&& (INSTRUCTION[0xee].mode == MODE_ABSOLUTE)
				&& (INSTRUCTION[0xee].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xee);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		data.low = rand();
		nes_processor_write(&g_test.processor, address_indirect.word, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		++data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, address_indirect.word) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xfe].opcode == OPCODE_INC)
				&& (INSTRUCTION[0xfe].mode == MODE_ABSOLUTE_X)
				&& (INSTRUCTION[0xfe].cycles == 7))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xfe);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		data.low = rand();
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		++data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		g_test.processor.index_x.low = index.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 6)
				&& (g_test.processor.program_counter.word == ((address.word + 3) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, address_indirect.word + g_test.processor.index_x.low) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xe6].opcode == OPCODE_INC)
				&& (INSTRUCTION[0xe6].mode == MODE_ZEROPAGE)
				&& (INSTRUCTION[0xe6].cycles == 5))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xe6);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		data.low = rand();
		nes_processor_write(&g_test.processor, address_indirect.low, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		++data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 4)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, address_indirect.low) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xf6].opcode == OPCODE_INC)
				&& (INSTRUCTION[0xf6].mode == MODE_ZEROPAGE_X)
				&& (INSTRUCTION[0xf6].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xf6);
		nes_processor_write(&g_test.processor, address.word + 1, address_indirect.low);
		data.low = rand();
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, data.low);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		++data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		g_test.processor.index_x.low = index.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == ((address.word + 2) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == index.low)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_read(&g_test.processor, (address_indirect.word + g_test.processor.index_x.low) & UINT8_MAX) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xe8].opcode == OPCODE_INX)
				&& (INSTRUCTION[0xe8].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xe8].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xe8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		data.low = rand();
		g_test.processor.index_x.low = data.low;
		++data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xc8].opcode == OPCODE_INY)
				&& (INSTRUCTION[0xc8].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xc8].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xc8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		data.low = rand();
		g_test.processor.index_y.low = data.low;
		++data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == data.low))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_jump(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x2000) + 512 }, address_indirect = { .word = (rand() % 0x2000) + 0x2000 + 512 },
			address_target = { .word = (rand() % 0x2000) + 0x4000 + 512 };

		if(ASSERT((INSTRUCTION[0x4c].opcode == OPCODE_JMP)
				&& (INSTRUCTION[0x4c].mode == MODE_ABSOLUTE)
				&& (INSTRUCTION[0x4c].cycles == 3))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x4c);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 2)
				&& (g_test.processor.program_counter.word == address_indirect.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x6c].opcode == OPCODE_JMP)
				&& (INSTRUCTION[0x6c].mode == MODE_INDIRECT)
				&& (INSTRUCTION[0x6c].cycles == 5))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x6c);
		address_indirect.low = 0x80;
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write_word(&g_test.processor, address_indirect.word, address_target.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 4)
				&& (g_test.processor.program_counter.word == address_target.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x6c);
		address_indirect.low = 0xff;
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word, address_target.low);
		nes_processor_write(&g_test.processor, address_indirect.high << CHAR_BIT, address_target.high);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 4)
				&& (g_test.processor.program_counter.word == address_target.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x20].opcode == OPCODE_JSR)
				&& (INSTRUCTION[0x20].mode == MODE_ABSOLUTE)
				&& (INSTRUCTION[0x20].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		address_indirect.word = (rand() % 0x2000) + 0x2000 + 512;
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x20);
		nes_processor_write_word(&g_test.processor, address.word + 1, address_indirect.word);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == address_indirect.word)
				&& (g_test.processor.stack_pointer.low == 0xfd - 2)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_pull_word(&g_test.processor) == address.word + 2))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_no_operation(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

		if(ASSERT((INSTRUCTION[0xea].opcode == OPCODE_NOP)
				&& (INSTRUCTION[0xea].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xea].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xea);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_pull(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 }, data = { .low = rand() }, status = {};

		if(ASSERT((INSTRUCTION[0x68].opcode == OPCODE_PLA)
				&& (INSTRUCTION[0x68].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x68].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x68);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_push(&g_test.processor, data.low);
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x28].opcode == OPCODE_PLP)
				&& (INSTRUCTION[0x28].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x28].cycles == 4))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x28);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_push(&g_test.processor, data.low);
		status.low = data.low;
		status.breakpoint = 0;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 3)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_push(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 }, data = { .low = rand() }, status = {};

		if(ASSERT((INSTRUCTION[0x48].opcode == OPCODE_PHA)
				&& (INSTRUCTION[0x48].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x48].cycles == 3))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x48);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 2)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfc)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_pull(&g_test.processor) == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x08].opcode == OPCODE_PHP)
				&& (INSTRUCTION[0x08].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x08].cycles == 3))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x08);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		status.low = g_test.processor.status.low;
		status.breakpoint = BREAKPOINT_SET;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 2)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfc)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_pull(&g_test.processor) == status.low))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_return(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 }, status = { .low = rand() };

		if(ASSERT((INSTRUCTION[0x40].opcode == OPCODE_RTI)
				&& (INSTRUCTION[0x40].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x40].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x40);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_push_word(&g_test.processor, address.word);
		nes_processor_push(&g_test.processor, status.low);
		status.breakpoint = 0;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == address.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x60].opcode == OPCODE_RTS)
				&& (INSTRUCTION[0x60].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x60].cycles == 6))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x60);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_push_word(&g_test.processor, address.word);
		++address.word;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 5)
				&& (g_test.processor.program_counter.word == address.word)
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_set(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

		if(ASSERT((INSTRUCTION[0x38].opcode == OPCODE_SEC)
				&& (INSTRUCTION[0x38].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x38].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x38);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.carry = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x35)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x38);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.carry = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x35)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xf8].opcode == OPCODE_SED)
				&& (INSTRUCTION[0xf8].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xf8].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xf8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.decimal = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x3c)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xf8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.decimal = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x3c)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x78].opcode == OPCODE_SEI)
				&& (INSTRUCTION[0x78].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x78].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x78);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.interrupt_disabled = false;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x78);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.status.interrupt_disabled = true;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_execute_transfer(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 }, data = { .low = rand() }, status = {};

		if(ASSERT((INSTRUCTION[0xaa].opcode == OPCODE_TAX)
				&& (INSTRUCTION[0xaa].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xaa].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xaa);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xa8].opcode == OPCODE_TAY)
				&& (INSTRUCTION[0xa8].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xa8].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xa8);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.accumulator.low = data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xba].opcode == OPCODE_TSX)
				&& (INSTRUCTION[0xba].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0xba].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0xba);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.stack_pointer.low = data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == data.low)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x8a].opcode == OPCODE_TXA)
				&& (INSTRUCTION[0x8a].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x8a].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x8a);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.index_x.low = data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x9a].opcode == OPCODE_TXS)
				&& (INSTRUCTION[0x9a].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x9a].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x9a);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.index_x.low = data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == data.low)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x98].opcode == OPCODE_TYA)
				&& (INSTRUCTION[0x98].mode == MODE_IMPLIED)
				&& (INSTRUCTION[0x98].cycles == 2))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, 0x98);
		nes_processor_reset(&g_test.processor);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		g_test.processor.index_y.low = data.low;
		status.low = 0x34;
		status.negative = data.negative;
		status.zero = !data.low;
		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == 1)
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == data.low))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_absolute(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .word = 0x4000 }, data = { .low = rand() };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word, data.low);
		nes_processor_reset(&g_test.processor);
		nes_processor_fetch_operand(&g_test.processor, MODE_ABSOLUTE);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.address_indirect.word == 0)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_absolute_x(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .word = 0x4000 }, data = { .low = rand() },
			index = { .low = 0x20 };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_x.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_ABSOLUTE_X);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address_indirect.word + g_test.processor.index_x.low)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		address_indirect.word = 0x4080;
		data.low = rand();
		index.low = UINT8_MAX;
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_x.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_ABSOLUTE_X);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address_indirect.word + g_test.processor.index_x.low)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == true))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_absolute_y(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .word = 0x4000 }, data = { .low = rand() },
			index = { .low = 0x20 };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_y.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_ABSOLUTE_Y);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address_indirect.word + g_test.processor.index_y.low)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		address_indirect.word = 0x4080;
		data.low = rand();
		index.low = UINT8_MAX;
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		nes_processor_write(&g_test.processor, address_indirect.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_y.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_ABSOLUTE_Y);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address_indirect.word + g_test.processor.index_y.low)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == true))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_immediate(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, data = { .low = rand() };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, data.low);
		nes_processor_reset(&g_test.processor);
		nes_processor_fetch_operand(&g_test.processor, MODE_IMMEDIATE);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address.word)
				&& (g_test.processor.fetched.operand.address_indirect.word == 0)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_implied(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, data = { .low = rand() };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_reset(&g_test.processor);
		g_test.processor.accumulator.low = data.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_IMPLIED);

		if(ASSERT((g_test.processor.fetched.operand.address.word == 0)
				&& (g_test.processor.fetched.operand.address_indirect.word == 0)
				&& (g_test.processor.fetched.operand.data.word == g_test.processor.accumulator.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_indirect(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .word = 0x4000 }, data = { .word = rand() };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		nes_processor_write_word(&g_test.processor, address_indirect.word, data.word);
		nes_processor_reset(&g_test.processor);
		nes_processor_fetch_operand(&g_test.processor, MODE_INDIRECT);

		if(ASSERT((g_test.processor.fetched.operand.address.word == data.word)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == 0)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		address_indirect.word = 0x5fff;
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		nes_processor_write_word(&g_test.processor, address_indirect.word, data.word);
		data.high = rand();
		nes_processor_write(&g_test.processor, address_indirect.high << CHAR_BIT, data.high);
		nes_processor_reset(&g_test.processor);
		nes_processor_fetch_operand(&g_test.processor, MODE_INDIRECT);

		if(ASSERT((g_test.processor.fetched.operand.address.word == data.word)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == 0)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_indirect_x(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .low = rand() }, data = { .low = rand() },
			index = { .low = rand() };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		address.word = 0x4000;
		nes_processor_write(&g_test.processor, (address_indirect.word + index.low) & UINT8_MAX, address.low);
		nes_processor_write(&g_test.processor, (address_indirect.word + index.low + 1) & UINT8_MAX, address.high);
		nes_processor_write(&g_test.processor, address.word, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_x.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_INDIRECT_X);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address.word)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_indirect_y(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .low = rand() }, data = { .low = rand() },
			index = { .low = 0x20 };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		address.word = 0x4000;
		nes_processor_write(&g_test.processor, address_indirect.word, address.low);
		nes_processor_write(&g_test.processor, (address_indirect.word + 1) & UINT8_MAX, address.high);
		nes_processor_write(&g_test.processor, address.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_y.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_INDIRECT_Y);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address.word + g_test.processor.index_y.low)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		address.word = 0x2000;
		index.low = 0x80;
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, address_indirect.word);
		address.word = 0x40f0;
		nes_processor_write(&g_test.processor, address_indirect.word, address.low);
		nes_processor_write(&g_test.processor, (address_indirect.word + 1) & UINT8_MAX, address.high);
		nes_processor_write(&g_test.processor, address.word + index.low, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_y.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_INDIRECT_Y);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address.word + g_test.processor.index_y.low)
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.word)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == true))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_relative(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, data = { .low = 0x10 };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, data.low);
		nes_processor_reset(&g_test.processor);
		nes_processor_fetch_operand(&g_test.processor, MODE_RELATIVE);

		if(ASSERT((g_test.processor.fetched.operand.address.word == ((g_test.processor.program_counter.word + data.word) & UINT16_MAX))
				&& (g_test.processor.fetched.operand.address_indirect.word == 0)
				&& (g_test.processor.fetched.operand.data.word == data.word)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}

		nes_test_initialize();
		data.word = 0xff80;
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write_word(&g_test.processor, address.word, data.low);
		nes_processor_reset(&g_test.processor);
		nes_processor_fetch_operand(&g_test.processor, MODE_RELATIVE);

		if(ASSERT((g_test.processor.fetched.operand.address.word == ((g_test.processor.program_counter.word + data.word) & UINT16_MAX))
				&& (g_test.processor.fetched.operand.address_indirect.word == 0)
				&& (g_test.processor.fetched.operand.data.word == data.word)
				&& (g_test.processor.fetched.operand.page_boundary == true))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_zeropage(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .low = rand() }, data = { .low = rand() };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, address_indirect.low);
		nes_processor_write(&g_test.processor, address_indirect.low, data.low);
		nes_processor_reset(&g_test.processor);
		nes_processor_fetch_operand(&g_test.processor, MODE_ZEROPAGE);

		if(ASSERT((g_test.processor.fetched.operand.address.word == address_indirect.low)
				&& (g_test.processor.fetched.operand.address_indirect.word == 0)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_zeropage_x(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .low = rand() }, data = { .low = rand() },
			index = { .low = rand() };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, address_indirect.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_x.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_ZEROPAGE_X);

		if(ASSERT((g_test.processor.fetched.operand.address.word == ((address_indirect.low + g_test.processor.index_x.low) & UINT8_MAX))
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.low)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_fetch_zeropage_y(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = 0x2000 }, address_indirect = { .low = rand() }, data = { .low = rand() },
			index = { .low = rand() };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, RESET_ADDRESS, address.word);
		nes_processor_write(&g_test.processor, address.word, address_indirect.low);
		nes_processor_write(&g_test.processor, (address_indirect.low + index.low) & UINT8_MAX, data.low);
		nes_processor_reset(&g_test.processor);
		g_test.processor.index_y.low = index.low;
		nes_processor_fetch_operand(&g_test.processor, MODE_ZEROPAGE_Y);

		if(ASSERT((g_test.processor.fetched.operand.address.word == ((address_indirect.low + g_test.processor.index_y.low) & UINT8_MAX))
				&& (g_test.processor.fetched.operand.address_indirect.word == address_indirect.low)
				&& (g_test.processor.fetched.operand.data.word == data.low)
				&& (g_test.processor.fetched.operand.page_boundary == false))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_interrupt_maskable(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, MASKABLE_ADDRESS, address.word + 256);
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
				&& (g_test.processor.program_counter.word == ((address.word + 1) & UINT16_MAX))
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
				&& (g_test.processor.program_counter.word == ((address.word + 256) & UINT16_MAX))
				&& (g_test.processor.stack_pointer.low == 0xfd - 3)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (g_test.processor.pending.maskable == false)
				&& (nes_processor_pull(&g_test.processor) == 0x20)
				&& (nes_processor_pull_word(&g_test.processor) == ((address.word + 1) & UINT16_MAX)))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_interrupt_non_maskable(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

		nes_test_initialize();
		nes_processor_write_word(&g_test.processor, NON_MASKABLE_ADDRESS, address.word + 256);
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
				&& (g_test.processor.program_counter.word == ((address.word + 256) & UINT16_MAX))
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
		nes_processor_write_word(&g_test.processor, NON_MASKABLE_ADDRESS, address.word + 512);

		while(g_test.processor.cycles > 0) {
			nes_processor_step(&g_test.processor);
		}

		nes_processor_step(&g_test.processor);

		if(ASSERT((g_test.processor.cycles == NON_MASKABLE_CYCLES - 1)
				&& (g_test.processor.program_counter.word == address.word + 512)
				&& (g_test.processor.stack_pointer.low == 0xfd - 3)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (g_test.processor.pending.non_maskable == false)
				&& (nes_processor_pull(&g_test.processor) == 0x20)
				&& (nes_processor_pull_word(&g_test.processor) == ((address.word + 256) & UINT16_MAX)))) {
			result = NES_ERR;
			goto exit;
		}
	}

exit:
	TEST_TRACE(result);

	return result;
}

int
nes_test_processor_reset(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

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

/* TODO */
break;
/* --- */
		}
	}

exit:
	nes_buffer_free(&g_test.memory);

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
