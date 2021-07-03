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

static nes_bus_t g_bus = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

nes_bus_t *
nes_bus(void)
{
	return &g_bus;
}

int
nes_bus_load(
	__in const nes_t *configuration
	)
{
	int result = NES_OK;

	TRACE(LEVEL_VERBOSE, "%s", "Bus loading");

	if((result = nes_buffer_allocate(&g_bus.ram_object, OBJECT_RAM_WIDTH, OBJECT_RAM_FILL)) != NES_OK) {
		goto exit;
	}

	if((result = nes_buffer_allocate(&g_bus.ram_processor, PROCESSOR_RAM_WIDTH, PROCESSOR_RAM_FILL)) != NES_OK) {
		goto exit;
	}

	if((result = nes_buffer_allocate(&g_bus.ram_video, VIDEO_RAM_WIDTH, VIDEO_RAM_FILL)) != NES_OK) {
		goto exit;
	}

	if((result = nes_buffer_allocate(&g_bus.ram_video_palette, VIDEO_PALETTE_RAM_WIDTH, VIDEO_PALETTE_RAM_FILL)) != NES_OK) {
		goto exit;
	}

	if((result = nes_mapper_load(configuration, &g_bus.mapper)) != NES_OK) {
		goto exit;
	}

	nes_processor_reset(&g_bus.processor);
	nes_video_reset(&g_bus.video);
	TRACE(LEVEL_VERBOSE, "%s", "Bus loaded");
	g_bus.loaded = true;

exit:
	return result;
}

uint8_t
nes_bus_read(
	__in int bus,
	__in uint16_t address
	)
{
	uint8_t result = g_bus.data;

	switch(bus) {
		case BUS_OBJECT:

			switch(address) {
				case OBJECT_RAM_BEGIN ... OBJECT_RAM_END: /* 0x0000 - 0x00ff */
					result = g_bus.ram_object.ptr[address - OBJECT_RAM_BEGIN];
					break;
				default:
					TRACE(LEVEL_WARNING, "Invalid object read: [%04X]", address);
					break;
			}
			break;
		case BUS_PROCESSOR:

			switch(address) {
				case PROCESSOR_RAM_BEGIN ... PROCESSOR_RAM_END: /* 0x0000 - 0x1fff */
					result = g_bus.ram_processor.ptr[(address - PROCESSOR_RAM_BEGIN) % PROCESSOR_RAM_MIRROR];
					break;
				case VIDEO_PORT_BEGIN ... VIDEO_PORT_END: /* 0x2000 - 0x3fff */
					result = nes_video_port_read(&g_bus.video, (address - VIDEO_PORT_BEGIN) % VIDEO_PORT_MIRROR);
					break;
				case PROCESSOR_WORK_RAM_BEGIN ... PROCESSOR_WORK_RAM_END: /* 0x6000 - 0x7fff */
					result = nes_mapper_read_ram(&g_bus.mapper, RAM_PROGRAM, address - PROCESSOR_WORK_RAM_BEGIN);
					break;
				case PROCESSOR_ROM_0_BEGIN ... PROCESSOR_ROM_0_END: /* 0x8000 - 0xbfff */
				case PROCESSOR_ROM_1_BEGIN ... PROCESSOR_ROM_1_END: /* 0xc000 - 0xffff */
					result = nes_mapper_read_rom(&g_bus.mapper, ROM_PROGRAM, address - PROCESSOR_ROM_0_BEGIN);
					break;
				default:
					TRACE(LEVEL_WARNING, "Invalid processor read: [%04X]", address);
					break;
			}
			break;
		case BUS_VIDEO:

			switch(address) {
				case VIDEO_ROM_BEGIN ... VIDEO_ROM_END: /* 0x0000 - 0x1fff */
					result = nes_mapper_read_rom(&g_bus.mapper, ROM_CHARACTER, address - VIDEO_ROM_BEGIN);
					break;
				case VIDEO_RAM_BEGIN ... VIDEO_RAM_END: /* 0x2000 - 0x3eff */
					result = g_bus.ram_video.ptr[(address - VIDEO_RAM_BEGIN) % VIDEO_RAM_MIRROR];
					break;
				case VIDEO_PALETTE_RAM_BEGIN ... VIDEO_PALETTE_RAM_END: /* 0x3f00 - 0x3fff */
					result = g_bus.ram_video_palette.ptr[(address - VIDEO_PALETTE_RAM_BEGIN) % VIDEO_PALETTE_RAM_MIRROR];
					break;
				default:
					TRACE(LEVEL_WARNING, "Invalid video read: [%04X]", address);
					break;
			}
			break;
		default:
			TRACE(LEVEL_WARNING, "Invalid bus type: %i", bus);
			break;
	}

	return result;
}

void
nes_bus_unload(void)
{
	TRACE(LEVEL_VERBOSE, "%s", "Bus unloading");
	nes_mapper_unload(&g_bus.mapper);
	nes_buffer_free(&g_bus.ram_video_palette);
	nes_buffer_free(&g_bus.ram_video);
	nes_buffer_free(&g_bus.ram_processor);
	nes_buffer_free(&g_bus.ram_object);
	memset(&g_bus, 0, sizeof(g_bus));
	TRACE(LEVEL_VERBOSE, "%s", "Bus unloaded");
}

void
nes_bus_write(
	__in int bus,
	__in uint16_t address,
	__in uint8_t data
	)
{
	g_bus.data = data;

	switch(bus) {
		case BUS_OBJECT:

			switch(address) {
				case OBJECT_RAM_BEGIN ... OBJECT_RAM_END: // 0x0000 - 0x00ff
					g_bus.ram_object.ptr[address - OBJECT_RAM_BEGIN] = data;
					break;
				default:
					TRACE(LEVEL_WARNING, "Invalid object write: [%04X]<-%02X", address, data);
					break;
			}
			break;
		case BUS_PROCESSOR:

			switch(address) {
				case PROCESSOR_RAM_BEGIN ... PROCESSOR_RAM_END: /* 0x0000 - 0x1fff */
					g_bus.ram_processor.ptr[(address - PROCESSOR_RAM_BEGIN) % PROCESSOR_RAM_MIRROR] = data;
					break;
				case VIDEO_PORT_BEGIN ... VIDEO_PORT_END: /* 0x2000 - 0x3fff */
					nes_video_port_write(&g_bus.video, (address - VIDEO_PORT_BEGIN) % VIDEO_PORT_MIRROR, data);
					break;
				case PROCESSOR_OAM: /* 0x4014 */
					nes_processor_transfer(&g_bus.processor, data);
					break;
				case PROCESSOR_WORK_RAM_BEGIN ... PROCESSOR_WORK_RAM_END: /* 0x6000 - 0x7fff */
					nes_mapper_write_ram(&g_bus.mapper, RAM_PROGRAM, address - PROCESSOR_WORK_RAM_BEGIN, data);
					break;
				case PROCESSOR_ROM_0_BEGIN ... PROCESSOR_ROM_0_END: /* 0x8000 - 0xbfff */
				case PROCESSOR_ROM_1_BEGIN ... PROCESSOR_ROM_1_END: /* 0xc000 - 0xffff */
					nes_mapper_write_rom(&g_bus.mapper, ROM_PROGRAM, address - PROCESSOR_ROM_0_BEGIN, data);
					break;
				default:
					TRACE(LEVEL_WARNING, "Invalid processor write: [%04X]<-%02X", address, data);
					break;
			}
			break;
		case BUS_VIDEO:

			switch(address) {
				case VIDEO_ROM_BEGIN ... VIDEO_ROM_END: /* 0x0000 - 0x1fff */
					nes_mapper_write_rom(&g_bus.mapper, ROM_CHARACTER, address - VIDEO_ROM_BEGIN, data);
					break;
				case VIDEO_RAM_BEGIN ... VIDEO_RAM_END: /* 0x2000 - 0x3eff */
					g_bus.ram_video.ptr[(address - VIDEO_RAM_BEGIN) % VIDEO_RAM_MIRROR] = data;
					break;
				case VIDEO_PALETTE_RAM_BEGIN ... VIDEO_PALETTE_RAM_END: /* 0x3f00 - 0x3fff */
					g_bus.ram_video_palette.ptr[(address - VIDEO_PALETTE_RAM_BEGIN) % VIDEO_PALETTE_RAM_MIRROR] = data;
					break;
				default:
					TRACE(LEVEL_WARNING, "Invalid video write: [%04X]<-%02X", address, data);
					break;
			}
			break;
		default:
			TRACE(LEVEL_WARNING, "Invalid bus type: %i", bus);
			break;
	}
}

int
nes_load(
	__in const nes_t *configuration
	)
{
	int result = NES_OK;

	if(!configuration) {
		result = ERROR(NES_ERR, "invalid configuration -- %p", configuration);
		goto exit;
	}

	TRACE(LEVEL_INFORMATION, "%s ver.%i.%i.%i", NES, nes_version()->major, nes_version()->minor, nes_version()->patch);
	TRACE(LEVEL_VERBOSE, "Configuration path: \"%s\"", configuration->rom.path);
	TRACE(LEVEL_VERBOSE, "Configuration rom: %p, %.02f KB (%u bytes)", configuration->rom.data.ptr, configuration->rom.data.length / (float)BYTES_PER_KBYTE,
		configuration->rom.data.length);
	TRACE(LEVEL_VERBOSE, "Configuration display: %sx%u", configuration->display.fullscreen ? "Fullscreen" : "Windowed", configuration->display.scale);

	if((result = nes_service_load(configuration)) != NES_OK) {
		goto exit;
	}

	if((result = nes_bus_load(configuration)) != NES_OK) {
		goto exit;
	}

	TRACE(LEVEL_INFORMATION, "%s", "Emulation loaded");
	TRACE_RESET();

exit:
	return result;
}

void
nes_unload(void)
{
	TRACE(LEVEL_INFORMATION, "%s", "Emulation unloaded");
	TRACE_RESET();

	nes_bus_unload();
	nes_service_unload();
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
