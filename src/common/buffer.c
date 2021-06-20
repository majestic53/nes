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

#include "./buffer_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_buffer_allocate(
	__inout nes_buffer_t *buffer,
	__in size_t length
	)
{
	int result = NES_OK;

	if(!(buffer->ptr = calloc(length, sizeof(uint8_t)))) {
		result = ERROR(NES_ERR, "failed to allocate buffer -- %.02f KB (%u bytes)", length / (float)BYTES_PER_KBYTE, length);
		goto exit;
	}

	buffer->length = length;
	memset(buffer->ptr, BUFFER_FILL, buffer->length);
	TRACE(LEVEL_VERBOSE, "Buffer allocate %p, %.02f KB (%u bytes)", buffer->ptr, buffer->length / (float)BYTES_PER_KBYTE, buffer->length);

exit:
	return result;
}

void
nes_buffer_free(
	__inout nes_buffer_t *buffer
	)
{

	if(buffer->ptr) {
		free(buffer->ptr);
		TRACE(LEVEL_VERBOSE, "Buffer free %p", buffer->ptr);
	}

	memset(buffer, 0, sizeof(*buffer));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
