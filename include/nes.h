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

#ifndef NES_H_
#define NES_H_

#include <stddef.h>
#include <stdint.h>

#define NES_API_VERSION_1 1
#define NES_API_VERSION NES_API_VERSION_1

enum {
	NES_EVT = -1,
	NES_OK,
	NES_ERR,
};

typedef struct {
	uint8_t *data;
	size_t length;
} nes_buffer_t;

typedef struct {
	int major;
	int minor;
	int patch;
} nes_version_t;

typedef struct {
#if NES_API_VERSION >= NES_API_VERSION_1
	const char *path;
	nes_buffer_t rom;
#endif /* NES_API_VERSION >= NES_API_VERSION_1 */
} nes_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes(const nes_t *);

const char *nes_error(void);

const nes_version_t *nes_version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_H_ */
