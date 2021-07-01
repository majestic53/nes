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

BUILD_DEBUG=BUILD_FLAGS=-g\ -D$(SERVICE)\ -D$(COLOR)\ -DLEVEL=
BUILD_RELEASE=BUILD_FLAGS=-O2\ -D$(SERVICE)\ -D$(COLOR)\ -DNDEBUG

DIR_BIN=./bin/
DIR_BIN_INCLUDE=./bin/include/
DIR_BIN_LIB=./bin/lib/
DIR_BUILD=./build/
DIR_ROOT=./
DIR_SRC=./src/
DIR_TEST_ACTION=./test/action/
DIR_TEST_BUS=./test/bus/
DIR_TEST_CARTRIDGE=./test/cartridge/
DIR_TEST_MAPPER=./test/mapper/
DIR_TEST_PROCESSOR=./test/processor/
DIR_TEST_VIDEO=./test/video/
DIR_TOOL=./tool/

COLOR?=COLOR

LEVEL?=0

SERVICE?=SDL

SLOTS?=12

all: release
debug: clean setup library_debug test_debug tool_debug
release: clean setup library_release test_release tool_release

analyze:
	@echo ''
	cloc $(DIR_ROOT)
	@echo ''
	cppcheck --enable=all --std=c11 $(DIR_ROOT)

clean:
	rm -rf $(DIR_BIN)
	rm -rf $(DIR_BUILD)

setup:
	mkdir -p $(DIR_BIN_INCLUDE)
	mkdir -p $(DIR_BIN_LIB)
	mkdir -p $(DIR_BUILD)

library_debug:
	cd $(DIR_SRC) && make $(BUILD_DEBUG)$(LEVEL) build -j$(SLOTS)
	cd $(DIR_SRC) && make archive

library_release:
	cd $(DIR_SRC) && make $(BUILD_RELEASE) build -j$(SLOTS)
	cd $(DIR_SRC) && make archive

test_debug:
	cd $(DIR_TEST_ACTION) && make $(BUILD_DEBUG)$(LEVEL) build
	cd $(DIR_TEST_BUS) && make $(BUILD_DEBUG)$(LEVEL) build
	cd $(DIR_TEST_CARTRIDGE) && make $(BUILD_DEBUG)$(LEVEL) build
	cd $(DIR_TEST_MAPPER) && make $(BUILD_DEBUG)$(LEVEL) build
	cd $(DIR_TEST_PROCESSOR) && make $(BUILD_DEBUG)$(LEVEL) build
	cd $(DIR_TEST_VIDEO) && make $(BUILD_DEBUG)$(LEVEL) build

test_release:
	cd $(DIR_TEST_ACTION) && make $(BUILD_RELEASE) build
	cd $(DIR_TEST_BUS) && make $(BUILD_RELEASE) build
	cd $(DIR_TEST_CARTRIDGE) && make $(BUILD_RELEASE) build
	cd $(DIR_TEST_MAPPER) && make $(BUILD_RELEASE) build
	cd $(DIR_TEST_PROCESSOR) && make $(BUILD_RELEASE) build
	cd $(DIR_TEST_VIDEO) && make $(BUILD_RELEASE) build

tool_debug:
	cd $(DIR_TOOL) && make $(BUILD_DEBUG)$(LEVEL) build

tool_release:
	cd $(DIR_TOOL) && make $(BUILD_RELEASE) build
	cd $(DIR_TOOL) && make package
