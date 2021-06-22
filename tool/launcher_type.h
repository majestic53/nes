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

#include <readline/history.h>
#include <readline/readline.h>
#include <getopt.h>
#include "../include/common.h"

#define ARGUMENT_MAX 10

#define DISPLAY_FULLSCREEN false
#define DISPLAY_SCALE 2

#define NOTICE "Copyright (C) 2021 David Jolly"

#define OPTION_DEBUG 'd'
#define OPTION_FULLSCREEN 'f'
#define OPTION_HELP 'h'
#define OPTION_SCALE 's'
#define OPTION_VERSION 'v'
#define OPTIONS "dfhs:v"

#define PROMPT_ERROR "????"
#define PROMPT_MAX 32
#define PROMPT_POSTFIX "] $ "
#define PROMPT_PREFIX "\n["

#define USAGE "nes [options] file"

enum {
	COMMAND_EXIT = 0,
	COMMAND_HELP,
	COMMAND_VERSION,
	COMMAND_MAX,
};

enum {
	FLAG_DEBUG = 0,
	FLAG_FULLSCREEN,
	FLAG_HELP,
	FLAG_SCALE,
	FLAG_VERSION,
	FLAG_MAX,
};

typedef struct {
	nes_t configuration;
	bool debug;
	const char *path;
	nes_action_t request;
	nes_action_t response;
	const nes_version_t *version;
} nes_launcher_t;

typedef int (*nes_launcher_debug_hdlr)(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        );

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nes_launcher_debug(
	__in const nes_launcher_t *launcher
	);

int nes_launcher_debug_help(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

int nes_launcher_debug_version(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

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
