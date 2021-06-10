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
nes_test_processor_execute_break(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

		if(ASSERT((INSTRUCTION[0x00].opcode == OPCODE_BRK)
				&& (INSTRUCTION[0x00].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 256))
				&& (g_test.processor.stack_pointer.low == 0xfd - 3)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_pull(&g_test.processor) == 0x34)
				&& (nes_processor_pull_word(&g_test.processor) == (uint16_t)(address.word + 1)))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 256))
				&& (g_test.processor.stack_pointer.low == 0xfd - 3)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (nes_processor_pull(&g_test.processor) == 0x30)
				&& (nes_processor_pull_word(&g_test.processor) == (uint16_t)(address.word + 1)))) {
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
				&& (INSTRUCTION[0x18].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xd8].opcode == OPCODE_CLD)
				&& (INSTRUCTION[0xd8].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x58].opcode == OPCODE_CLI)
				&& (INSTRUCTION[0x58].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x30)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xb8].opcode == OPCODE_CLV)
				&& (INSTRUCTION[0xb8].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 }, data = {}, status = {};

		/* TODO: DEC */

		if(ASSERT((INSTRUCTION[0xca].opcode == OPCODE_DEX)
				&& (INSTRUCTION[0xca].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x88].opcode == OPCODE_DEY)
				&& (INSTRUCTION[0x88].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
					&& (instruction->mode == MODE_IMPLIED))) {
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
					&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 }, data = {}, status = {};

		/* TODO: INC */

		if(ASSERT((INSTRUCTION[0xe8].opcode == OPCODE_INX)
				&& (INSTRUCTION[0xe8].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xc8].opcode == OPCODE_INY)
				&& (INSTRUCTION[0xc8].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
nes_test_processor_execute_no_operation(void)
{
	int result = NES_OK;

	for(size_t trial = 0; trial < TRIALS; ++trial) {
		nes_processor_register_t address = { .word = (rand() % 0x8000) + 512 };

		if(ASSERT((INSTRUCTION[0xea].opcode == OPCODE_NOP)
				&& (INSTRUCTION[0xea].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (INSTRUCTION[0x68].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x28].opcode == OPCODE_PLP)
				&& (INSTRUCTION[0x28].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (INSTRUCTION[0x48].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (INSTRUCTION[0x08].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (INSTRUCTION[0x40].mode == MODE_IMPLIED))) {
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
				&& (INSTRUCTION[0x60].mode == MODE_IMPLIED))) {
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
				&& (INSTRUCTION[0x38].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x35)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xf8].opcode == OPCODE_SED)
				&& (INSTRUCTION[0xf8].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == 0x3c)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x78].opcode == OPCODE_SEI)
				&& (INSTRUCTION[0x78].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (INSTRUCTION[0xaa].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xa8].opcode == OPCODE_TAY)
				&& (INSTRUCTION[0xa8].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == data.low))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0xba].opcode == OPCODE_TSX)
				&& (INSTRUCTION[0xba].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == data.low)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x8a].opcode == OPCODE_TXA)
				&& (INSTRUCTION[0x8a].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == 0xfd)
				&& (g_test.processor.status.low == status.low)
				&& (g_test.processor.accumulator.low == data.low)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x9a].opcode == OPCODE_TXS)
				&& (INSTRUCTION[0x9a].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
				&& (g_test.processor.stack_pointer.low == data.low)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == data.low)
				&& (g_test.processor.index_y.low == 0))) {
			result = NES_ERR;
			goto exit;
		}

		if(ASSERT((INSTRUCTION[0x98].opcode == OPCODE_TYA)
				&& (INSTRUCTION[0x98].mode == MODE_IMPLIED))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 1))
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 256))
				&& (g_test.processor.stack_pointer.low == 0xfd - 3)
				&& (g_test.processor.status.low == 0x34)
				&& (g_test.processor.accumulator.low == 0)
				&& (g_test.processor.index_x.low == 0)
				&& (g_test.processor.index_y.low == 0)
				&& (g_test.processor.pending.maskable == false)
				&& (nes_processor_pull(&g_test.processor) == 0x20)
				&& (nes_processor_pull_word(&g_test.processor) == (uint16_t)(address.word + 1)))) {
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
				&& (g_test.processor.program_counter.word == (uint16_t)(address.word + 256))
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
				&& (nes_processor_pull_word(&g_test.processor) == (uint16_t)(address.word + 256)))) {
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
		}
	}

exit:
	nes_buffer_free(&g_test.memory);

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
