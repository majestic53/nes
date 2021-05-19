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

#ifndef NES_LAUNCHER_TYPE_H_
#define NES_LAUNCHER_TYPE_H_

#include <getopt.h>
#include "../include/common.h"

#define NOTICE "Copyright (C) 2021 David Jolly"

#define OPTION_HELP 'h'
#define OPTION_VERSION 'v'
#define OPTIONS "hv"

#define USAGE "nes [options] file"

enum {
	FLAG_HELP = 0,
	FLAG_VERSION,
	FLAG_MAX,
};

static const char *FLAG[] = {
	"-h", /* FLAG_HELP */
	"-v", /* FLAG_VERSION */
	};

static const char *FLAG_DESC[] = {
	"Display help information", /* FLAG_HELP */
	"Display version information", /* FLAG_VERSION */
	};

typedef struct {
	const char *path;
	const nes_version_t *version;
	nes_t configuration;
} nes_launcher_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_launcher_load(void);

void nes_launcher_unload(void);

void nes_launcher_usage(
	__in FILE *stream,
	__in bool verbose
	);

void nes_launcher_version(
	__in FILE *stream,
	__in bool verbose
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_LAUNCHER_TYPE_H_ */
