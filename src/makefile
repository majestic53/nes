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

DIR_BIN_INCLUDE=../bin/include/
DIR_BIN_LIB=../bin/lib/
DIR_BUILD=../build/
DIR_INCLUDE=../include/
DIR_INCLUDE_COMMON=../include/common/
DIR_ROOT=./
DIR_ROOT_COMMON=./common/
DIR_ROOT_SERVICE=./service/

FLAGS=-std=c11 -Wall -Werror

LIB=libnes.a

build: build_base build_common build_service

build_base: base_bus.o

base_bus.o: $(DIR_ROOT)bus.c $(DIR_INCLUDE)bus.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT)bus.c -o $(DIR_BUILD)base_bus.o

build_common: common_buffer.o common_cartridge.o common_error.o common_trace.o common_version.o

common_buffer.o: $(DIR_ROOT_COMMON)buffer.c $(DIR_INCLUDE_COMMON)buffer.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT_COMMON)buffer.c -o $(DIR_BUILD)common_buffer.o

common_cartridge.o: $(DIR_ROOT_COMMON)cartridge.c $(DIR_INCLUDE_COMMON)cartridge.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT_COMMON)cartridge.c -o $(DIR_BUILD)common_cartridge.o

common_error.o: $(DIR_ROOT_COMMON)error.c $(DIR_INCLUDE_COMMON)error.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT_COMMON)error.c -o $(DIR_BUILD)common_error.o

common_trace.o: $(DIR_ROOT_COMMON)trace.c $(DIR_INCLUDE_COMMON)trace.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT_COMMON)trace.c -o $(DIR_BUILD)common_trace.o

common_version.o: $(DIR_ROOT_COMMON)version.c
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT_COMMON)version.c -o $(DIR_BUILD)common_version.o

build_service: service_sdl.o

service_sdl.o: $(DIR_ROOT_SERVICE)sdl.c $(DIR_INCLUDE)service.h
	$(CC) $(FLAGS) $(BUILD_FLAGS) -c $(DIR_ROOT_SERVICE)sdl.c -o $(DIR_BUILD)service_sdl.o

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY ----------------------------------------------------------'
	ar rcs $(DIR_BIN_LIB)$(LIB) $(DIR_BUILD)base_bus.o \
		$(DIR_BUILD)common_buffer.o $(DIR_BUILD)common_cartridge.o $(DIR_BUILD)common_error.o $(DIR_BUILD)common_trace.o $(DIR_BUILD)common_version.o \
		$(DIR_BUILD)service_sdl.o
	cp $(DIR_INCLUDE)nes.h $(DIR_BIN_INCLUDE)
	@echo '--- DONE ----------------------------------------------------------------------'
	@echo ''
