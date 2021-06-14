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

#ifndef NES_TEST_COMMON_H_
#define NES_TEST_COMMON_H_

#include "../include/common.h"

#define ASSERT(_CONDITION_) \
	(_CONDITION_) ? NES_OK : NES_ERR

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

#define TEST_COUNT(_TESTS_) \
	sizeof(_TESTS_) / sizeof(*(_TESTS_))

#define TRACE_RESULT(_RESULT_) \
	fprintf(((_RESULT_) != NES_OK) ? stderr : stdout, "[%s%s%s] %s\n", LEVEL_COLOR(((_RESULT_) != NES_OK) ? LEVEL_ERROR : LEVEL_INFORMATION), \
		((_RESULT_) != NES_OK) ? "FAIL" : "PASS", LEVEL_COLOR(LEVEL_MAX), __FUNCTION__)

#define TRACE_SEED(_SEED_) \
        fprintf(stdout, "[%sSEED%s] %x\n", LEVEL_COLOR(LEVEL_INFORMATION), LEVEL_COLOR(LEVEL_MAX), _SEED_)

typedef int (*nes_test)(void);

#endif /* NES_TEST_COMMON_H_ */
