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

#ifndef NES_TRACE_TYPE_H_
#define NES_TRACE_TYPE_H_

#include "../../include/common.h"

#ifndef NDEBUG
#define TIMESTAMP "[%Y-%m-%d %H:%M:%S] "

#ifdef COLOR

static const char *LEVEL_COL[] = {
	"\x1b[0m", /* LEVEL_NONE */
	"\x1b[91m", /* LEVEL_ERROR */
	"\x1b[93m", /* LEVEL_WARNING */
	"\x1b[94m", /* LEVEL_INFORMATION */
	"\x1b[90m", /* LEVEL_VERBOSE */
};

#define LEVEL_COLOR(_LEVEL_) \
	LEVEL_COL[((_LEVEL_) < LEVEL_MAX) ? (_LEVEL_) : LEVEL_NONE]
#else
#define LEVEL_COLOR(_LEVEL_) ""
#endif /* COLOR */

typedef struct {
	uint64_t cycle;
	double runtime;
	char format[FORMAT_MAX];
} nes_trace_t;

#endif /* NDEBUG */

#endif /* NES_TRACE_TYPE_H_ */
