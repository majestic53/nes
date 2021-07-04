# NES
# Copyright (C) 2021 David Jolly
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
# AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

BIN=test-action

DIR_BUILD=../../build/
DIR_BUILD_TEST=../../build/test/
DIR_ROOT=./

FLAGS=-std=c11 -Wall -Werror

build: build_test link run

build_test: test_action.o

test_action.o: $(DIR_ROOT)action.c
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT)action.c -o $(DIR_BUILD)test_action.o

link:
	@echo ''
	@echo '--- BUILDING ACTION TEST ------------------------------------------------------'
	$(CC) $(FLAGS) $(BUILD_FLAGS) $(DIR_BUILD)test_action.o \
		$(DIR_BUILD)base_action.o \
		$(DIR_BUILD)common_buffer.o $(DIR_BUILD)common_error.o $(DIR_BUILD)common_trace.o \
		-o $(DIR_BUILD_TEST)$(BIN)
	@echo '--- DONE ----------------------------------------------------------------------'
	@echo ''

run:
	@echo '--- RUNNING ACTION TEST -------------------------------------------------------'
	@cd $(DIR_BUILD_TEST) && if ./$(BIN); \
	then \
		echo '--- PASSED --------------------------------------------------------------------'; \
	else \
		echo '--- FAILED --------------------------------------------------------------------'; \
		exit 1; \
	fi
	@echo ''
