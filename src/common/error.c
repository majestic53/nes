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

#include "./error_type.h"

static nes_error_t g_error = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

const char *
nes_error(void)
{
	return g_error.format;
}

int
nes_error_set(
	__in int error,
#ifndef NDEBUG
	__in const char *file,
	__in const char *function,
	__in size_t line,
#endif /* NDEBUG */
	__in const char *format,
	...
	)
{
	memset(&g_error, 0, sizeof(g_error));
	g_error.error = error;

	if(format) {
		va_list arguments;

		va_start(arguments, format);
		vsnprintf(g_error.format, FORMAT_MAX, format, arguments);
		va_end(arguments);
	}

#ifndef NDEBUG
	_TRACE(LEVEL_ERROR, file, function, line, "%s", g_error.format);
	snprintf(g_error.format + strlen(g_error.format), FORMAT_MAX - strlen(g_error.format), " (%s:%s@%zu)", function, file, line);
#endif /* NDEBUG */

	return g_error.error;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
