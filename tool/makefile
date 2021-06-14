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

BIN=nes

DIR_BIN=../bin/
DIR_BIN_LIB=../bin/lib/
DIR_BUILD=../build/
DIR_ROOT=./

FLAGS=-std=c11 -Wall -Werror
FLAGS_LIB=-lreadline -lSDL2 -lSDL2main

LIB=libnes.a

build: build_tool link

build_tool: tool_launcher.o

tool_launcher.o: $(DIR_ROOT)launcher.c
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT)launcher.c -o $(DIR_BUILD)tool_launcher.o

link:
	@echo ''
	@echo '--- BUILDING TOOL -------------------------------------------------------------'
	$(CC) $(FLAGS) $(BUILD_FLAGS) $(DIR_BUILD)tool_launcher.o $(DIR_BIN_LIB)$(LIB) $(FLAGS_LIB) -o $(DIR_BIN)$(BIN)
	@echo '--- DONE ----------------------------------------------------------------------'
	@echo ''

package:
	@echo ''
	@echo '--- PACKAGING TOOL ------------------------------------------------------------'
	strip -s $(DIR_BIN)$(BIN)
	strip -s $(DIR_BIN_LIB)$(LIB)
	@echo '--- DONE ----------------------------------------------------------------------'
	@echo ''
