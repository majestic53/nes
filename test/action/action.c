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
			g_test.address = address;
			result = g_test.data;
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
			g_test.address = address;
			g_test.data = data;
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

int
nes_test_action_bus_read(void)
{
	int result = NES_OK;

	nes_test_initialize();
	g_test.data = rand();
	g_test.request.type = NES_ACTION_BUS_READ;
	g_test.request.address = rand();

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_BUS_READ)
			&& (g_test.response.address == g_test.request.address)
			&& (g_test.response.data == (g_test.data & UINT8_MAX))
			&& (g_test.address == g_test.request.address))) {
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
	g_test.request.address = rand();
	g_test.request.data = rand();

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.address == g_test.request.address)
			&& (g_test.data == (g_test.request.data & UINT8_MAX)))) {
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
	g_test.request.address = NES_PROCESSOR_PROGRAM_COUNTER;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address == NES_PROCESSOR_PROGRAM_COUNTER)
			&& (g_test.response.data == g_test.bus.processor.program_counter.word))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.stack_pointer.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address = NES_PROCESSOR_STACK_POINTER;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address == NES_PROCESSOR_STACK_POINTER)
			&& (g_test.response.data == g_test.bus.processor.stack_pointer.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.status.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address = NES_PROCESSOR_STATUS;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address == NES_PROCESSOR_STATUS)
			&& (g_test.response.data == g_test.bus.processor.status.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.accumulator.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address = NES_PROCESSOR_ACCUMULATOR;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address == NES_PROCESSOR_ACCUMULATOR)
			&& (g_test.response.data == g_test.bus.processor.accumulator.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.index_x.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address = NES_PROCESSOR_INDEX_X;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address == NES_PROCESSOR_INDEX_X)
			&& (g_test.response.data == g_test.bus.processor.index_x.low))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.bus.processor.index_y.low = rand();
	g_test.request.type = NES_ACTION_PROCESSOR_READ;
	g_test.request.address = NES_PROCESSOR_INDEX_Y;

	if(ASSERT((nes_action(&g_test.request, &g_test.response) == NES_OK)
			&& (g_test.response.type == NES_ACTION_PROCESSOR_READ)
			&& (g_test.response.address == NES_PROCESSOR_INDEX_Y)
			&& (g_test.response.data == g_test.bus.processor.index_y.low))) {
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
	g_test.request.address = NES_PROCESSOR_PROGRAM_COUNTER;
	g_test.request.data = rand();

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.program_counter.word == g_test.request.data))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address = NES_PROCESSOR_STACK_POINTER;
	g_test.request.data = rand();

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.stack_pointer.low == (g_test.request.data & UINT8_MAX)))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address = NES_PROCESSOR_STATUS;
	g_test.request.data = rand();

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.status.low == (g_test.request.data & UINT8_MAX)))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address = NES_PROCESSOR_ACCUMULATOR;
	g_test.request.data = rand();

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.accumulator.low == (g_test.request.data & UINT8_MAX)))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address = NES_PROCESSOR_INDEX_X;
	g_test.request.data = rand();

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.index_x.low == (g_test.request.data & UINT8_MAX)))) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.request.type = NES_ACTION_PROCESSOR_WRITE;
	g_test.request.address = NES_PROCESSOR_INDEX_Y;
	g_test.request.data = rand();

	if(ASSERT((nes_action(&g_test.request, NULL) == NES_OK)
			&& (g_test.bus.processor.index_y.low == (g_test.request.data & UINT8_MAX)))) {
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
