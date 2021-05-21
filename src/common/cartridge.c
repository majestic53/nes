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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_cartridge_load(
	__in const nes_t *configuration,
	__inout nes_cartridge_t *cartridge
	)
{
	int result = NES_OK;

	if(configuration->rom.length < sizeof(*cartridge->header)) {
		result = ERROR(NES_ERR, "cartridge is too small -- expecting %.02f KB (%u bytes)", sizeof(*cartridge->header) / (float)BYTES_PER_KBYTE, sizeof(*cartridge->header));
		goto exit;
	}

	cartridge->header = (const nes_cartridge_header_t *)configuration->rom.data;

	if(strncmp((char *)cartridge->header->magic, HEADER_MAGIC, strlen(HEADER_MAGIC))) {
		result = ERROR(NES_ERR, "cartridge magic mismatch -- expecting \"%s\"", HEADER_MAGIC);
		goto exit;
	}

	cartridge->mapper = (cartridge->header->flag_7.mapper_high << CHAR_BIT) | cartridge->header->flag_6.mapper_low;

	/* TODO */

exit:
	return result;
}

uint8_t
nes_cartridge_read_ram(
	__in const nes_cartridge_t *cartridge,
	__in size_t bank,
	__in uint16_t address
	)
{
	return cartridge->ram.data[(bank * RAM_BANK_WIDTH) + address];
}

uint8_t
nes_cartridge_read_rom(
	__in const nes_cartridge_t *cartridge,
	__in size_t bank,
	__in uint16_t address
	)
{
	return cartridge->rom.data[(bank * ROM_BANK_WIDTH) + address];
}

void
nes_cartridge_unload(
	__inout nes_cartridge_t *cartridge
	)
{
	memset(cartridge, 0, sizeof(*cartridge));
}

void
nes_cartridge_write_ram(
	__inout nes_cartridge_t *cartridge,
	__in size_t bank,
	__in uint16_t address,
	__in uint8_t data
	)
{
	cartridge->rom.data[(bank * ROM_BANK_WIDTH) + address] = data;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
