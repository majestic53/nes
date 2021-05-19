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

#ifndef NES_TRACE_H_
#define NES_TRACE_H_

#include "./define.h"

enum {
	LEVEL_NONE = 0,
	LEVEL_ERROR,
	LEVEL_WARNING,
	LEVEL_INFORMATION,
	LEVEL_VERBOSE,
	LEVEL_MAX,
};

#ifndef NDEBUG
#define _TRACE(_LEVEL_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...) \
	if((_LEVEL_) <= (LEVEL)) nes_trace(_LEVEL_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, __VA_ARGS__)
#define TRACE(_LEVEL_, _FORMAT_, ...) \
	_TRACE(_LEVEL_, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)
#define TRACE_RESET() \
	nes_trace_reset()
#define TRACE_STEP() \
	nes_trace_step()
#else
#define TRACE(_LEVEL_, _FORMAT_, ...)
#define TRACE_RESET()
#define TRACE_STEP()
#endif /* NDEBUG */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef NDEBUG

void nes_trace(
	__in int level,
	__in const char *file,
	__in const char *function,
	__in size_t line,
	__in const char *format,
	...
	);

void nes_trace_reset(void);

void nes_trace_step(void);

#endif /* NDEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_TRACE_H_ */
