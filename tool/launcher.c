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

#include "./launcher_type.h"

static nes_launcher_t g_launcher = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_launcher_load(void)
{
	FILE *file;
	int length, result = NES_OK;

	if(!(file = fopen(g_launcher.configuration.path, "rb"))) {
		fprintf(stderr, "%s: file not found -- %s\n", g_launcher.path, g_launcher.configuration.path);
		result = NES_ERR;
		goto exit;
	}

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	if(length < 0) {
		fprintf(stderr, "%s: malformed file -- %s\n", g_launcher.path, g_launcher.configuration.path);
		result = NES_ERR;
		goto exit;
	} else if(!length) {
		fprintf(stderr, "%s: empty file -- %s\n", g_launcher.path, g_launcher.configuration.path);
		result = NES_ERR;
		goto exit;
	}

	if((result = nes_buffer_allocate(&g_launcher.configuration.rom, length)) != NES_OK) {
		fprintf(stderr, "%s: %s\n", g_launcher.path, nes_error());
		goto exit;
	}

	if(fread(g_launcher.configuration.rom.data, sizeof(uint8_t), g_launcher.configuration.rom.length, file) != g_launcher.configuration.rom.length) {
		fprintf(stderr, "%s: file read error -- %s {%.02f KB (%zu bytes)}\n", g_launcher.path, g_launcher.configuration.path, g_launcher.configuration.rom.length / (float)BYTES_PER_KBYTE,
			g_launcher.configuration.rom.length);
		result = NES_ERR;
		goto exit;
	}

exit:

	if(file) {
		fclose(file);
		file = NULL;
	}

	return result;
}

void
nes_launcher_unload(void)
{
	nes_buffer_free(&g_launcher.configuration.rom);
	memset(&g_launcher, 0, sizeof(g_launcher));
}

void
nes_launcher_usage(
	__in FILE *stream,
	__in bool verbose
	)
{

	if(verbose) {
		nes_launcher_version(stream, true);
	}

	fprintf(stream, "\n%s\n\n", USAGE);

	for(int flag = 0; flag < FLAG_MAX; ++flag) {
		fprintf(stream, "%s\t%s\n", FLAG[flag], FLAG_DESC[flag]);
	}
}

void
nes_launcher_version(
	__in FILE *stream,
	__in bool verbose
	)
{
	fprintf(stream, "%s %i.%i.%i\n", NES, g_launcher.version->major, g_launcher.version->minor, g_launcher.version->minor);

	if(verbose) {
		fprintf(stream, "%s\n", NOTICE);
	}
}

int
main(
	__in int argc,
	__in char *argv[]
	)
{
	int option, result = NES_OK;

	g_launcher.path = argv[0];
	g_launcher.version = nes_version();

	if(!argc) {
		nes_launcher_usage(stderr, false);
		result = NES_ERR;
		goto exit;
	}

	opterr = 1;

	while((option = getopt(argc, argv, OPTIONS)) != -1) {

		switch(option) {
			case OPTION_HELP:
				nes_launcher_usage(stdout, true);
				goto exit;
			case OPTION_VERSION:
				nes_launcher_version(stdout, false);
				goto exit;
			case '?':
			default:
				nes_launcher_usage(stderr, false);
				result = NES_ERR;
				goto exit;
		}
	}

	for(option = optind; option < argc; ++option) {

		if(g_launcher.configuration.path) {
			fprintf(stderr, "%s: redefined file path -- %s\n", g_launcher.path, argv[option]);
			nes_launcher_usage(stderr, false);
			result = NES_ERR;
			goto exit;
		}

		g_launcher.configuration.path = argv[option];
	}

	if(!g_launcher.configuration.path) {
		fprintf(stderr, "%s: undefined file path\n", g_launcher.path);
		nes_launcher_usage(stderr, false);
		result = NES_ERR;
		goto exit;
	}

	if((result = nes_launcher_load()) != NES_OK) {
		goto exit;
	}

	if((result = nes(&g_launcher.configuration)) != NES_OK) {
		fprintf(stderr, "%s: %s\n", g_launcher.path, nes_error());
	}

exit:
	nes_launcher_unload();

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
