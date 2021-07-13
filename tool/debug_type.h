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

#ifndef NES_LAUNCHER_DEBUG_TYPE_H_
#define NES_LAUNCHER_DEBUG_TYPE_H_

#include <readline/history.h>
#include <readline/readline.h>
#include "../src/common/cartridge_type.h"
#include "../src/system/processor_type.h"
#include "../src/system/video_type.h"
#include "./common.h"

#define ARGUMENT_MAX 10

#define CHARACTER_FILL '.'

#define PROMPT_ERROR "????"
#define PROMPT_MAX 32
#define PROMPT_POSTFIX "] $ "
#define PROMPT_PREFIX "\n["

enum {
        COMMAND_EXIT = 0,
        COMMAND_DISASSEMBLE,
        COMMAND_HELP,
        COMMAND_MAPPER,
        COMMAND_PROCESSOR,
        COMMAND_READ,
        COMMAND_RUN,
        COMMAND_STEP,
        COMMAND_VERSION,
        COMMAND_VIDEO,
        COMMAND_WRITE,
        COMMAND_MAX,
};

enum {
        DISASSEMBLE_SINGLE = 1,
        DISASSEMBLE_MULTIPLE,
};

enum {
        MAPPER_SHOW = 0,
        MAPPER_READ,
        MAPPER_WRITE,
};

enum {
        PROCESSOR_SHOW = 0,
        PROCESSOR_READ,
        PROCESSOR_WRITE,
};

enum {
        READ_BYTE = 1,
        READ_MULTIBYTE,
};

enum {
        VIDEO_SHOW = 0,
        VIDEO_READ,
        VIDEO_WRITE,
};

enum {
        WRITE_BYTE = 2,
};

static const char COMMAND[] = {
        'q', /* COMMAND_EXIT */
        'd', /* COMMAND_DISASSEMBLE */
        'h', /* COMMAND_HELP */
        'm', /* COMMAND_MAPPER */
        'p', /* COMMAND_PROCESSOR */
        'r', /* COMMAND_READ */
        'c', /* COMMAND_RUN */
        's', /* COMMAND_STEP */
        'e', /* COMMAND_VERSION */
        'v', /* COMMAND_VIDEO */
        'w', /* COMMAND_WRITE */
        };

static const char *COMMAND_DESC[] = {
        "Exit debug mode", /* COMMAND_EXIT */
        "Disassemble instructions", /* COMMAND_DISASSEMBLE */
        "Show help information", /* COMMAND_HELP */
        "Read/Write/Show mapper registers", /* COMMAND_MAPPER */
        "Read/Write/Show processor registers", /* COMMAND_PROCESSOR */
        "Read byte from address", /* COMMAND_READ */
        "Run processor", /* COMMAND_RUN */
        "Step processor", /* COMMAND_STEP */
        "Show version information", /* COMMAND_VERSION */
        "Read/Write/Show video registers", /* COMMAND_VIDEO */
        "Write byte to address", /* COMMAND_WRITE */
        };

static const char *INCREMENT_FORMAT[] = {
        "Across", /* VIDEO_INCREMENT_ACROSS */
        "Down", /* VIDEO_INCREMENT_DOWN */
        };

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

const char *MAPPER_NAME[] = {
        "NROM", /* MAPPER_NROM */
        };

static const char *MAPPER_REGISTER[] = {
        "prom0", /* NES_MAPPER_PROGRAM_ROM_0 */
        "prom1", /* NES_MAPPER_PROGRAM_ROM_1 */
        "pram", /* NES_MAPPER_PROGRAM_RAM */
        "crom", /* NES_MAPPER_CHARACTER_ROM */
        "cram", /* NES_MAPPER_CHARACTER_RAM */
        };

static const char *MODE_FORMAT[] = {
        " %04X", /* MODE_ABSOLUTE */
        " %04X+X", /* MODE_ABSOLUTE_X */
        " %04X+Y", /* MODE_ABSOLUTE_Y */
        " %02X", /* MODE_IMMEDIATE */
        "", /* MODE_IMPLIED */
        " (%04X)", /* MODE_INDIRECT */
        " (%02X,X)", /* MODE_INDIRECT_X */
        " (%02X),Y", /* MODE_INDIRECT_Y */
        " %02X(%i) [%04X]", /* MODE_RELATIVE */
        " %02X", /* MODE_ZEROPAGE */
        " %02X+X", /* MODE_ZEROPAGE_X */
        " %02X+Y", /* MODE_ZEROPAGE_Y */
        };

static const char *NAME_TABLE_FORMAT[] = {
        "2000-23FF", /* VIDEO_NAME_TABLE_2000 */
        "2400-27FF", /* VIDEO_NAME_TABLE_2400 */
        "2800-2BFF", /* VIDEO_NAME_TABLE_2800 */
        "2C00-2FFF", /* VIDEO_NAME_TABLE_2C00 */
        };

static const char *OPCODE_FORMAT[] = {
        "ADC", /* OPCODE_ADC */
        "AND", /* OPCODE_AND */
        "ASL", /* OPCODE_ASL */
        "BCC", /* OPCODE_BCC */
        "BCS", /* OPCODE_BCS */
        "BEQ", /* OPCODE_BEQ */
        "BIT", /* OPCODE_BIT */
        "BMI", /* OPCODE_BMI */
        "BNE", /* OPCODE_BNE */
        "BPL", /* OPCODE_BPL */
        "BRK", /* OPCODE_BRK */
        "BVC", /* OPCODE_BVC */
        "BVS", /* OPCODE_BVS */
        "CLC", /* OPCODE_CLC */
        "CLD", /* OPCODE_CLD */
        "CLI", /* OPCODE_CLI */
        "CLV", /* OPCODE_CLV */
        "CMP", /* OPCODE_CMP */
        "CPX", /* OPCODE_CPX */
        "CPY", /* OPCODE_CPY */
        "DEC", /* OPCODE_DEC */
        "DEX", /* OPCODE_DEX */
        "DEY", /* OPCODE_DEY */
        "EOR", /* OPCODE_EOR */
        "INC", /* OPCODE_INC */
        "INX", /* OPCODE_INX */
        "INY", /* OPCODE_INY */
        "JMP", /* OPCODE_JMP */
        "JSR", /* OPCODE_JSR */
        "LDA", /* OPCODE_LDA */
        "LDX", /* OPCODE_LDX */
        "LDY", /* OPCODE_LDY */
        "LSR", /* OPCODE_LSR */
        "NOP", /* OPCODE_NOP */
        "ORA", /* OPCODE_ORA */
        "PHA", /* OPCODE_PHA */
        "PHP", /* OPCODE_PHP */
        "PLA", /* OPCODE_PLA */
        "PLP", /* OPCODE_PLP */
        "ROL", /* OPCODE_ROL */
        "ROR", /* OPCODE_ROR */
        "RTI", /* OPCODE_RTI */
        "RTS", /* OPCODE_RTS */
        "SBC", /* OPCODE_SBC */
        "SEC", /* OPCODE_SEC */
        "SED", /* OPCODE_SED */
        "SEI", /* OPCODE_SEI */
        "STA", /* OPCODE_STA */
        "STX", /* OPCODE_STX */
        "STY", /* OPCODE_STY */
        "TAX", /* OPCODE_TAX */
        "TAY", /* OPCODE_TAY */
        "TSX", /* OPCODE_TSX */
        "TXA", /* OPCODE_TXA */
        "TXS", /* OPCODE_TXS */
        "TYA", /* OPCODE_TYA */
        "XXX", /* OPCODE_XXX */
        };

static const char *PATTERN_TABLE_FORMAT[] = {
        "0000-0FFF", /* VIDEO_PATTERN_TABLE_0000 */
        "1000-1FFF", /* VIDEO_PATTERN_TABLE_1000 */
        };

static const char *PROCESSOR_REGISTER[] = {
        "pc", /* NES_PROCESSOR_PROGRAM_COUNTER */
        "sp", /* NES_PROCESSOR_STACK_POINTER */
        "s", /* NES_PROCESSOR_STATUS */
        "p", /* NES_PROCESSOR_PENDING */
        "a", /* NES_PROCESSOR_ACCUMULATOR */
        "x", /* NES_PROCESSOR_INDEX_X */
        "y", /* NES_PROCESSOR_INDEX_Y */
        };

static const char *SPRITE_SIZE_FORMAT[] = {
        "8x8", /* VIDEO_SPRITE_SIZE_8_8 */
        "8x16", /* VIDEO_SPRITE_SIZE_8_16 */
        };

static const char *VIDEO_REGISTER[] = {
        "ctrl", /* NES_VIDEO_CONTROL */
        "mask", /* NES_VIDEO_MASK */
        "stat", /* NES_VIDEO_STATUS */
        "oaddr", /* NES_VIDEO_OBJECT_ADDRESS */
        "odata", /* NES_VIDEO_OBJECT_DATA */
        "scrx", /* NES_VIDEO_SCROLL_X */
        "scry", /* NES_VIDEO_SCROLL_Y */
        "addr", /* NES_VIDEO_ADDRESS */
        "data", /* NES_VIDEO_DATA */
        };

typedef int (*nes_launcher_debug_hdlr)(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        );

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_launcher_debug_disassemble(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

int nes_launcher_debug_help(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

int nes_launcher_debug_mapper(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        );

int nes_launcher_debug_processor(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

int nes_launcher_debug_read(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

int nes_launcher_debug_run(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

int nes_launcher_debug_step(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

int nes_launcher_debug_version(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

int nes_launcher_debug_video(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        );

int nes_launcher_debug_write(
	__in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
	);

static const nes_launcher_debug_hdlr DEBUG_HDLR[] = {
        NULL, /* COMMAND_EXIT */
        nes_launcher_debug_disassemble, /* COMMAND_DISASSEMBLE */
        nes_launcher_debug_help, /* COMMAND_HELP */
        nes_launcher_debug_mapper, /* COMMAND_MAPPER */
        nes_launcher_debug_processor, /* COMMAND_PROCESSOR */
        nes_launcher_debug_read, /* COMMAND_READ */
        nes_launcher_debug_run, /* COMMAND_RUN */
        nes_launcher_debug_step, /* COMMAND_STEP */
        nes_launcher_debug_version, /* COMMAND_VERSION */
        nes_launcher_debug_video, /* COMMAND_VIDEO */
        nes_launcher_debug_write, /* COMMAND_WRITE */
        };

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_LAUNCHER_DEBUG_TYPE_H_ */
