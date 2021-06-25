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

#include "./video_type.h"

static nes_test_video_t g_test = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* TODO */

void
nes_test_initialize(void)
{

	/* TODO */
	memset(&g_test, 0, sizeof(g_test));
	/* --- */

}

int
main(
	__in int argc,
	__in char *argv[]
	)
{
	int result = NES_OK, seed;

	if(argc > 1) {
		seed = strtol(argv[1], NULL, 16);
	} else {
		seed = time(NULL);
	}

	srand(seed);
	TRACE_SEED(seed);

	for(size_t test = 0; test < TEST_COUNT(TEST); ++test) {

		if(TEST[test]() != NES_OK) {
			result = NES_ERR;
		}
	}

	return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
