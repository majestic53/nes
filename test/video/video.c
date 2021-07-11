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

#include "./video_type.h"

static nes_test_video_t g_test = {};

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
		case BUS_OBJECT:
			result = g_test.object.ptr[address];
			break;
		case BUS_VIDEO:
			result = g_test.memory.ptr[address];
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
		case BUS_OBJECT:
			g_test.object.ptr[address] = data;
			break;
		case BUS_VIDEO:
			g_test.memory.ptr[address] = data;
			break;
		default:
			break;
	}
}

void
nes_test_initialize(void)
{
	memset(g_test.memory.ptr, 0x00, g_test.memory.length);
	memset(g_test.object.ptr, 0x00, g_test.object.length);
	nes_video_reset(&g_test.video);
}

int
nes_test_video_port_read(void)
{
	int result = NES_OK;
	nes_register_t data = {};

	nes_test_initialize();
	g_test.video.control.raw = rand();

	if(ASSERT(nes_video_port_read(&g_test.video, VIDEO_PORT_CONTROL) == 0)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.video.mask.raw = rand();

	if(ASSERT(nes_video_port_read(&g_test.video, VIDEO_PORT_MASK) == 0)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	data.word = rand();
	g_test.video.status.raw = data.low;
	g_test.video.address_latch = data.low % 2;

	if(ASSERT((nes_video_port_read(&g_test.video, VIDEO_PORT_STATUS) == data.low)
			&& !g_test.video.status.vblank
			&& !g_test.video.address_latch)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.video.object_address.word = rand();

	if(ASSERT(nes_video_port_read(&g_test.video, VIDEO_PORT_OBJECT_ADDRESS) == 0)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	for(g_test.video.object_address.word = 0; g_test.video.object_address.word <= UINT8_MAX; ++g_test.video.object_address.word) {
		data.low = rand();
		g_test.object.ptr[g_test.video.object_address.low] = data.low;

		if(ASSERT(nes_video_port_read(&g_test.video, VIDEO_PORT_OBJECT_DATA) == data.low)) {
			result = NES_ERR;
			goto exit;
		}
	}

	nes_test_initialize();
	g_test.video.scroll_x.word = rand();
	g_test.video.scroll_y.word = rand();

	if(ASSERT(nes_video_port_read(&g_test.video, VIDEO_PORT_SCROLL) == 0)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.video.address.word = rand();

	if(ASSERT(nes_video_port_read(&g_test.video, VIDEO_PORT_ADDRESS) == 0)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	for(uint8_t increment = 0; increment < VIDEO_INCREMENT_MAX; ++increment) {
		nes_register_t address = {}, data_prev = {};

		g_test.video.address.word = 0;
		g_test.video.control.increment = increment;

		for(; address.word < VIDEO_ADDRESS_MIRROR; address.word += VIDEO_INCREMENT[increment]) {
			data.low = rand();
			g_test.memory.ptr[g_test.video.address.word] = data.low;

			if(ASSERT(g_test.video.address.word == address.word)) {
				result = NES_ERR;
				goto exit;
			}

			switch(g_test.video.address.word) {
	                        case VIDEO_PALETTE_RAM_BEGIN ... VIDEO_PALETTE_RAM_END: /* 0x3f00 - 0x3fff */

					if(ASSERT((nes_video_port_read(&g_test.video, VIDEO_PORT_DATA) == data.low)
							&& (g_test.video.data.low == data.low))) {
						result = NES_ERR;
						goto exit;
					}
	                                break;
	                        default: /* 0x0000 - 0x3eff */
					data_prev.low = g_test.video.data.low;

					if(ASSERT((nes_video_port_read(&g_test.video, VIDEO_PORT_DATA) == data_prev.low)
							&& (g_test.video.data.low == data.low))) {
						result = NES_ERR;
						goto exit;
					}
	                                break;
			}
		}
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_video_port_write(void)
{
	int result = NES_OK;
	nes_register_t address = {}, data = {};

	nes_test_initialize();
	data.low = rand();
	nes_video_port_write(&g_test.video, VIDEO_PORT_CONTROL, data.low);

	if(ASSERT(g_test.video.control.raw == data.low)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	data.low = rand();
	nes_video_port_write(&g_test.video, VIDEO_PORT_MASK, data.low);

	if(ASSERT(g_test.video.mask.raw == data.low)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	data.low = g_test.video.status.raw;
	nes_video_port_write(&g_test.video, VIDEO_PORT_STATUS, rand());

	if(ASSERT(g_test.video.status.raw == data.low)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	data.low = rand();
	nes_video_port_write(&g_test.video, VIDEO_PORT_OBJECT_ADDRESS, data.low);

	if(ASSERT(g_test.video.object_address.low == data.low)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	g_test.video.object_address.word = 0;

	for(address.word = 0; address.word < UINT8_MAX + 1; ++address.word) {
		data.low = rand();
		nes_video_port_write(&g_test.video, VIDEO_PORT_OBJECT_DATA, data.low);

		if(ASSERT((g_test.object.ptr[address.low & UINT8_MAX] == data.low)
				&& (g_test.video.object_address.low == ((address.low + 1) & UINT8_MAX)))) {
			result = NES_ERR;
			goto exit;
		}
	}

	nes_test_initialize();
	data.low = rand();
	g_test.video.address_latch = false;
	nes_video_port_write(&g_test.video, VIDEO_PORT_SCROLL, data.low);

	if(ASSERT((g_test.video.scroll_x.low == data.low)
			&& g_test.video.address_latch)) {
		result = NES_ERR;
		goto exit;
	}

	data.low = rand();
	nes_video_port_write(&g_test.video, VIDEO_PORT_SCROLL, data.low);

	if(ASSERT((g_test.video.scroll_y.low == data.low)
			&& !g_test.video.address_latch)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();
	data.low = rand();
	g_test.video.address_latch = false;
	nes_video_port_write(&g_test.video, VIDEO_PORT_ADDRESS, data.low);
	data.high = data.low;
	data.low = 0;
	data.word %= VIDEO_ADDRESS_MIRROR;

	if(ASSERT((g_test.video.address.word == data.word)
			&& g_test.video.address_latch)) {
		result = NES_ERR;
		goto exit;
	}

	data.low = rand();
	nes_video_port_write(&g_test.video, VIDEO_PORT_ADDRESS, data.low);
	data.word %= VIDEO_ADDRESS_MIRROR;

	if(ASSERT((g_test.video.address.word == data.word)
			&& !g_test.video.address_latch)) {
		result = NES_ERR;
		goto exit;
	}

	nes_test_initialize();

	for(uint8_t increment = 0; increment < VIDEO_INCREMENT_MAX; ++increment) {
		g_test.video.address.word = 0;
		g_test.video.control.increment = increment;

		for(address.word = 0; address.word < VIDEO_ADDRESS_MIRROR; address.word += VIDEO_INCREMENT[increment]) {
			data.low = rand();

			if(ASSERT(g_test.video.address.word == address.word)) {
				result = NES_ERR;
				goto exit;
			}

			nes_video_port_write(&g_test.video, VIDEO_PORT_DATA, data.low);

			if(ASSERT(g_test.memory.ptr[address.word] == data.low)) {
				result = NES_ERR;
				goto exit;
			}
		}
	}

exit:
	TRACE_RESULT(result);

	return result;
}

int
nes_test_video_reset(void)
{
	int result = NES_OK;

	nes_test_initialize();

	if(ASSERT(!g_test.video.address.word
			&& !g_test.video.address_latch
			&& !g_test.video.control.raw
			&& !g_test.video.cycles
			&& !g_test.video.data.word
			&& !g_test.video.mask.raw
			&& !g_test.video.object_address.word
			&& !g_test.video.scroll_x.word
			&& !g_test.video.scroll_y.word
			&& !g_test.video.status.raw)) {
		result = NES_ERR;
		goto exit;
	}

exit:
	TRACE_RESULT(result);

	return result;
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

	if((result = nes_buffer_allocate(&g_test.memory, VIDEO_ADDRESS_MIRROR, 0x00)) != NES_OK) {
		goto exit;
	}

	if((result = nes_buffer_allocate(&g_test.object, UINT8_MAX + 1, 0x00)) != NES_OK) {
		goto exit;
	}

	for(size_t test = 0; test < TEST_COUNT(TEST); ++test) {

		if(TEST[test]() != NES_OK) {
			result = NES_ERR;
		}
	}

exit:
	nes_buffer_free(&g_test.object);
	nes_buffer_free(&g_test.memory);

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
