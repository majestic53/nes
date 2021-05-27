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

#include "./mapper_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_mapper_load(
	__in const nes_t *configuration,
	__inout nes_mapper_t *mapper
	)
{
	int result = NES_OK;

	/* TODO */

	return result;
}

uint8_t
nes_mapper_read_ram(
	__in const nes_mapper_t *mapper,
	__in uint16_t address
	)
{
	uint8_t result = 0;

	/* TODO */

	return result;
}

uint8_t
nes_mapper_read_rom(
	__in const nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address
	)
{
	uint8_t result = 0;

	/* TODO */

	return result;
}

void
nes_mapper_unload(
	__inout nes_mapper_t *mapper
	)
{
	/* TODO */
}

void
nes_mapper_write_ram(
	__inout nes_mapper_t *mapper,
	__in uint16_t address,
	__in uint8_t data
	)
{
	/* TODO */
}

void
nes_mapper_write_rom(
	__inout nes_mapper_t *mapper,
	__in int type,
	__in uint16_t address,
	__in uint8_t data
	)
{
	/* TODO */
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
