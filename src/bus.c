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

	if((result = nes_mapper_load(configuration, &g_bus.mapper)) != NES_OK) {
		goto exit;
	}

	/* TODO: LOAD SUBSYSTEMS */

	nes_processor_reset(&g_bus.processor);
	TRACE(LEVEL_VERBOSE, "%s", "Bus loaded");

exit:
	return result;
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

			switch(address) {

				/* TODO: READ DATA FROM ADDRESS */

				default:
					TRACE(LEVEL_WARNING, "Invalid processor address: %04X", address);
					break;
			}
			break;
		case BUS_VIDEO:

			switch(address) {

				/* TODO: READ DATA FROM ADDRESS */

				default:
					TRACE(LEVEL_WARNING, "Invalid video address: %04X", address);
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

	/* TODO: UNLOAD SUBSYSTEMS */

	nes_mapper_unload(&g_bus.mapper);
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

	switch(bus) {
		case BUS_PROCESSOR:

			switch(address) {

				/* TODO: READ DATA FROM ADDRESS */

				default:
					TRACE(LEVEL_WARNING, "Invalid processor address: %04X", address);
					break;
			}
			break;
		case BUS_VIDEO:

			switch(address) {

				/* TODO: READ DATA FROM ADDRESS */

				default:
					TRACE(LEVEL_WARNING, "Invalid video address: %04X", address);
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
