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

#define BLOCK_WIDTH 16

#define CHARACTER_FILL '.'

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
        STATUS_CARRY = (1 << 0),
        STATUS_ZERO = (1 << 1),
        STATUS_INTERRUPT_DISABLE = (1 << 2),
        STATUS_DECIMAL = (1 << 3),
        STATUS_BREAKPOINT = (1 << 4),
        STATUS_OVERFLOW = (1 << 6),
        STATUS_NEGATIVE = (1 << 7),
};

enum {
        WRITE_BYTE = 2,
};

static const char COMMAND[] = {
        'q', /* COMMAND_EXIT */
        'h', /* COMMAND_HELP */
        'p', /* COMMAND_PROCESSOR */
        'r', /* COMMAND_READ */
        'c', /* COMMAND_RUN */
        's', /* COMMAND_STEP */
        'v', /* COMMAND_VERSION */
        'w', /* COMMAND_WRITE */
        };

static const char *COMMAND_DESC[] = {
        "Exit debug mode", /* COMMAND_EXIT */
        "Show help information", /* COMMAND_HELP */
        "Read/Write/Show processor", /* COMMAND_PROCESSOR */
        "Read data from address", /* COMMAND_READ */
        "Run processor", /* COMMAND_RUN */
        "Step processor", /* COMMAND_STEP */
        "Show version information", /* COMMAND_VERSION */
        "Write data to address", /* COMMAND_WRITE */
        };

static const nes_launcher_debug_hdlr HANDLER[] = {
        NULL, /* COMMAND_EXIT */
        nes_launcher_debug_help, /* COMMAND_HELP */
        nes_launcher_debug_processor, /* COMMAND_PROCESSOR */
        nes_launcher_debug_read, /* COMMAND_READ */
        nes_launcher_debug_run, /* COMMAND_RUN */
        nes_launcher_debug_step, /* COMMAND_STEP */
        nes_launcher_debug_version, /* COMMAND_VERSION */
        nes_launcher_debug_write, /* COMMAND_WRITE */
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

static const char *REGISTER[] = {
        "pc", /* Processor program counter register */
        "sp", /* Processor stack pointer register */
        "s", /* Processor status register */
        "a", /* Processor accumulator register */
        "x", /* Processor index-x register */
        "y", /* Processor index-y register */
        };

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_launcher_debug_help(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        int result = NES_OK;

        if(count) {
                result = NES_ERR;
                goto exit;
        }

        for(int command = 0; command < COMMAND_MAX; ++command) {
                fprintf(stdout, "%c\t%s\n", COMMAND[command], COMMAND_DESC[command]);
        }

exit:
        return result;
}

int
nes_launcher_debug_processor(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        int result = NES_OK;
        nes_action_t request = { .type = NES_ACTION_PROCESSOR_READ }, response = {};

        switch(count) {
                case PROCESSOR_READ:

                        for(request.address = 0; request.address < NES_PROCESSOR_MAX; ++request.address) {

                                if(!strcmp(REGISTER[request.address], argument[0])) {
                                        break;
                                }
                        }

                        if(request.address == NES_PROCESSOR_MAX) {
                                result = NES_ERR;
                                goto exit;
                        }

                        if((result = nes_action(&request, &response)) != NES_OK) {
                                goto exit;
                        }

                        switch(request.address) {
                                case NES_PROCESSOR_PROGRAM_COUNTER:
                                        fprintf(stdout, "%04X\n", response.data & UINT16_MAX);
                                        break;
                                case NES_PROCESSOR_STATUS:
                                        fprintf(stdout, "%02X  [%c%c%c%c%c%c%c]\n", response.data & UINT8_MAX,
                                                (response.data & STATUS_NEGATIVE) ? 'N' : '-', (response.data & STATUS_OVERFLOW) ? 'O' : '-',
                                                (response.data & STATUS_BREAKPOINT) ? 'B' : '-', (response.data & STATUS_DECIMAL) ? 'D' : '-',
                                                (response.data & STATUS_INTERRUPT_DISABLE) ? 'I' : '-', (response.data & STATUS_ZERO) ? 'Z' : '-',
                                                (response.data & STATUS_CARRY) ? 'C' : '-');
                                        break;
                                default:
                                        fprintf(stdout, "%02X\n", response.data & UINT8_MAX);
                                        break;
                        }
                        break;
                case PROCESSOR_SHOW:

                        for(request.address = 0; request.address < NES_PROCESSOR_MAX; ++request.address) {

                                if((result = nes_action(&request, &response)) != NES_OK) {
                                        goto exit;
                                }

                                switch(request.address) {
                                        case NES_PROCESSOR_PROGRAM_COUNTER:
                                                fprintf(stdout, "%s> %04X\n", REGISTER[request.address], response.data & UINT16_MAX);
                                                break;
                                        case NES_PROCESSOR_STATUS:
                                                fprintf(stdout, "%s> %02X  [%c%c%c%c%c%c%c]\n", REGISTER[request.address], response.data & UINT8_MAX,
                                                        (response.data & STATUS_NEGATIVE) ? 'N' : '-', (response.data & STATUS_OVERFLOW) ? 'O' : '-',
                                                        (response.data & STATUS_BREAKPOINT) ? 'B' : '-', (response.data & STATUS_DECIMAL) ? 'D' : '-',
                                                        (response.data & STATUS_INTERRUPT_DISABLE) ? 'I' : '-', (response.data & STATUS_ZERO) ? 'Z' : '-',
                                                        (response.data & STATUS_CARRY) ? 'C' : '-');
                                                break;
                                        default:
                                                fprintf(stdout, "%s> %02X\n", REGISTER[request.address], response.data & UINT8_MAX);
                                                break;
                                }
                        }
                        break;
                case PROCESSOR_WRITE:
                        request.type = NES_ACTION_PROCESSOR_WRITE;
                        request.data = strtol(argument[1], NULL, 16);

                        for(request.address = 0; request.address < NES_PROCESSOR_MAX; ++request.address) {

                                if(!strcmp(REGISTER[request.address], argument[0])) {
                                        break;
                                }
                        }

                        if(request.address == NES_PROCESSOR_MAX) {
                                result = NES_ERR;
                                goto exit;
                        }

                        if((result = nes_action(&request, &response)) != NES_OK) {
                                goto exit;
                        }
                        break;
                default:
                        result = NES_ERR;
                        goto exit;
        }

exit:
        return result;
}

int
nes_launcher_debug_read(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        int result = NES_OK;
        uint16_t base, offset;
        char str[BLOCK_WIDTH + 1] = {};
        nes_action_t request = { .type = NES_ACTION_BUS_READ }, response = {};

        switch(count) {
                case READ_BYTE:
                        request.address = strtol(argument[0], NULL, 16);

                        if((result = nes_action(&request, &response)) != NES_OK) {
                                goto exit;
                        }

                        fprintf(stdout, "%02X\n", response.data);
                        break;
                case READ_MULTIBYTE:
                        base = strtol(argument[0], NULL, 16);
                        offset = strtol(argument[1], NULL, 16);

                        for(uint32_t address = base, count = 0; address < base + offset; ++address, ++count) {
                                request.address = address & UINT16_MAX;

                                if(count == BLOCK_WIDTH) {
                                        count = 0;

                                        if(strlen(str)) {
                                                fprintf(stdout, "  %s", str);
                                        }

                                        memset(str, 0, BLOCK_WIDTH);
                                        fprintf(stdout, "\n");
                                }

                                if(!count) {
                                        fprintf(stdout, "%04X> ", request.address);
                                }

                                if((result = nes_action(&request, &response)) != NES_OK) {
                                        goto exit;
                                }

                                if(isprint((char)response.data) && !isspace((char)response.data)) {
                                        str[count] = (char)response.data;
                                } else {
                                        str[count] = CHARACTER_FILL;
                                }

                                fprintf(stdout, " %02X", response.data);
                        }

                        if(strlen(str)) {
                                fprintf(stdout, "  %s\n", str);
                        }
                        break;
                default:
                        result = NES_ERR;
                        goto exit;
        }

exit:
        return result;
}

int
nes_launcher_debug_run(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        int result = NES_OK;
        nes_action_t request = { .type = NES_ACTION_RUN };

        if(count) {
                result = NES_ERR;
                goto exit;
        }

        if((result = nes_action(&request, NULL)) != NES_OK) {
                goto exit;
        }

exit:
        return result;
}

int
nes_launcher_debug_step(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        int result = NES_OK;
        nes_action_t request = { .type = NES_ACTION_STEP };

        if(count) {
                result = NES_ERR;
                goto exit;
        }

        if((result = nes_action(&request, NULL)) != NES_OK) {
                goto exit;
        }

exit:
        return result;
}

int
nes_launcher_debug_version(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        int result = NES_OK;

        if(count) {
                result = NES_ERR;
                goto exit;
        }

        fprintf(stdout, "%i.%i.%i\n", launcher->version->major, launcher->version->minor, launcher->version->patch);

exit:
        return result;
}

int
nes_launcher_debug_write(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        int result = NES_OK;
        nes_action_t request = { .type = NES_ACTION_BUS_WRITE };

        switch(count) {
                case WRITE_BYTE:
                        request.address = strtol(argument[0], NULL, 16);
                        request.data = strtol(argument[1], NULL, 16);
                        result = nes_action(&request, NULL);
                        break;
                default:
                        result = NES_ERR;
                        goto exit;
        }

exit:
        return result;
}

void
nes_launcher_debug(
        __in const nes_launcher_t *launcher
        )
{
        bool complete = false;

        using_history();
        fprintf(stdout, "%s%s %i.%i.%i\n%s\n\n", LEVEL_COLOR(LEVEL_VERBOSE), NES, launcher->version->major, launcher->version->minor, launcher->version->patch, NOTICE);
        fprintf(stdout, "Path: %s\nSize: %.02f KB (%zu bytes)\n%s", launcher->configuration.rom.path, launcher->configuration.rom.data.length / (float)BYTES_PER_KBYTE,
                launcher->configuration.rom.data.length, LEVEL_COLOR(LEVEL_MAX));

        while(!complete) {
                char *input, prompt[PROMPT_MAX] = {};
                nes_action_t request = {}, response = {};

                request.type = NES_ACTION_PROCESSOR_READ;
                request.address = NES_PROCESSOR_PROGRAM_COUNTER;

                if(nes_action(&request, &response) == NES_OK) {
                        snprintf(prompt, PROMPT_MAX, "%s%s%04X%s%s", PROMPT_PREFIX, LEVEL_COLOR(LEVEL_INFORMATION), response.data, LEVEL_COLOR(LEVEL_MAX),
                                        PROMPT_POSTFIX);
                } else {
                        snprintf(prompt, PROMPT_MAX, "%s%s%s%s%s", PROMPT_PREFIX, LEVEL_COLOR(LEVEL_INFORMATION), PROMPT_ERROR, LEVEL_COLOR(LEVEL_MAX),
                                        PROMPT_POSTFIX);
                }

                if((input = readline(prompt))) {
                        int command = 0;
                        uint32_t count = 0;
                        bool first = true, from_history = false;
                        const char *argument[ARGUMENT_MAX] = {}, *next;

                        if((from_history = !strlen(input))) {
                                HIST_ENTRY *previous;

                                if(!(previous = history_get(where_history()))) {
                                        goto cleanup;
                                }

                                input = previous->line;
                        }

                        for(; command < COMMAND_MAX; ++command) {

                                if(input[0] == COMMAND[command]) {
                                        break;
                                }
                        }

                        if(command >= COMMAND_MAX) {
                                fprintf(stderr, "%sUnsupported command: %s\n%s", LEVEL_COLOR(LEVEL_ERROR), input, LEVEL_COLOR(LEVEL_MAX));
                                goto cleanup;
                        }

                        add_history(input);
                        next = strtok(input, " ");

                        while(next) {

                                if(count >= ARGUMENT_MAX) {
                                        fprintf(stderr, "%sToo many arguments: %u\n%s", LEVEL_COLOR(LEVEL_ERROR), count + 1, LEVEL_COLOR(LEVEL_MAX));
                                        goto cleanup;
                                }

                                if(!first) {
                                        argument[count++] = next;
                                }

                                next = strtok(NULL, " ");
                                first = false;
                        }

                        switch(command) {
                                case COMMAND_EXIT:
                                        complete = true;
                                        break;
                                case (COMMAND_EXIT + 1) ... (COMMAND_MAX - 1):

                                        if(HANDLER[command](launcher, argument, count) != NES_OK) {
                                                fprintf(stderr, "%sCommand failed: %s\n%s", LEVEL_COLOR(LEVEL_ERROR), input, LEVEL_COLOR(LEVEL_MAX));
                                        }
                                        break;
                                default:
                                        fprintf(stderr, "%sUnsupported command type: %i\n%s", LEVEL_COLOR(LEVEL_ERROR), command, LEVEL_COLOR(LEVEL_MAX));
                                        break;
                        }

cleanup:

                        if(!from_history && input) {
                                free(input);
                        }
                }
        }

        clear_history();
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
