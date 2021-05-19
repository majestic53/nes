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

#include "./trace_type.h"

#ifndef NDEBUG
static nes_trace_t g_trace = {};
#endif /* NDEBUG */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef NDEBUG

void
nes_trace(
	__in int level,
	__in const char *file,
	__in const char *function,
	__in size_t line,
	__in const char *format,
	...
	)
{
	FILE *stream = NULL;
	time_t current = time(NULL);

	strftime(g_trace.format, FORMAT_MAX, TIMESTAMP, localtime(&current));
	snprintf(g_trace.format + strlen(g_trace.format), FORMAT_MAX - strlen(g_trace.format), "(%lu) {%.02f ms} ", g_trace.cycle, g_trace.runtime);

	if(format) {
		va_list arguments;

		va_start(arguments, format);
		vsnprintf(g_trace.format + strlen(g_trace.format), FORMAT_MAX - strlen(g_trace.format), format, arguments);
		va_end(arguments);
	}

	snprintf(g_trace.format + strlen(g_trace.format), FORMAT_MAX - strlen(g_trace.format), " (%s:%s@%zu)", function, file, line);

	switch(level) {
		case LEVEL_ERROR:
		case LEVEL_WARNING:
			stream = stderr;
			break;
		default:
			stream = stdout;
			break;
	}

	if(stream) {
		fprintf(stream, "%s%s%s\n", LEVEL_COLOR(level), g_trace.format, LEVEL_COLOR(LEVEL_MAX));
	}
}

void
nes_trace_reset(void)
{
	memset(&g_trace, 0, sizeof(g_trace));
}

void
nes_trace_step(void)
{
	g_trace.runtime = ((++g_trace.cycle * MILLISEC_PER_SEC) / (float)CYCLES_PER_FRAME) / (float)FRAMES_PER_SEC;
}

#endif /* NDEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */
