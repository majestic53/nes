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

#include "./debug_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void
nes_launcher_debug(
        __in const nes_launcher_t *launcher
        )
{
        bool complete = false;

        using_history();
        fprintf(stdout, "%s %i.%i.%i\n%s\n\n", NES, launcher->version->major, launcher->version->minor, launcher->version->patch, NOTICE);
        fprintf(stdout, "Path: %s\nSize: %.02f KB (%zu bytes)\n", launcher->configuration.rom.path, launcher->configuration.rom.data.length / (float)BYTES_PER_KBYTE,
                launcher->configuration.rom.data.length);

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

                                        if(DEBUG_HDLR[command](launcher, argument, count) != NES_OK) {
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

int
nes_launcher_debug_disassemble(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        int result = NES_OK;
        uint16_t address = 0, offset = 1;
        nes_action_t request = { .type = NES_ACTION_BUS_READ }, response = {};

        switch(count) {
                case DISASSEMBLE_MULTIPLE:
                        address = strtol(argument[0], NULL, 16);
                        offset = strtol(argument[1], NULL, 10);
                        break;
                case DISASSEMBLE_SINGLE:
                        address = strtol(argument[0], NULL, 16);
                        break;
                default:
                        result = NES_ERR;
                        goto exit;
        }

        for(uint16_t index = 0; index < offset; ++index) {
                uint16_t operand = 0;
                request.address = address++;
                const nes_processor_instruction_t *instruction;

                if((result = nes_action(&request, &response)) != NES_OK) {
                        goto exit;
                }

                if(offset > 1) {
                        fprintf(stdout, "%04X>\t ", response.address);
                }

                fprintf(stdout, "%02X", response.data);
                instruction = nes_processor_instruction(response.data);

                switch(instruction->mode) {
                        case MODE_ABSOLUTE:
                        case MODE_ABSOLUTE_X:
                        case MODE_ABSOLUTE_Y:
                        case MODE_INDIRECT:
                                request.address = address++;

                                if((result = nes_action(&request, &response)) != NES_OK) {
                                        goto exit;
                                }

                                fprintf(stdout, " %02X", response.data);
                                operand |= (response.data & UINT8_MAX);
                                request.address = address++;

                                if((result = nes_action(&request, &response)) != NES_OK) {
                                        goto exit;
                                }

                                fprintf(stdout, " %02X  ", response.data);
                                operand |= ((response.data & UINT8_MAX) << CHAR_BIT);
                                break;
                        case MODE_IMMEDIATE:
                        case MODE_INDIRECT_X:
                        case MODE_INDIRECT_Y:
                        case MODE_RELATIVE:
                        case MODE_ZEROPAGE:
                        case MODE_ZEROPAGE_X:
                        case MODE_ZEROPAGE_Y:
                                request.address = address++;

                                if((result = nes_action(&request, &response)) != NES_OK) {
                                        goto exit;
                                }

                                fprintf(stdout, " %02X     ", response.data);
                                operand |= (response.data & UINT8_MAX);
                                break;
                        default:
                                fprintf(stdout, "        ");
                                break;
                }

                fprintf(stdout, "%s", OPCODE_FORMAT[instruction->opcode]);

                switch(instruction->mode) {
                        case MODE_ABSOLUTE:
                        case MODE_ABSOLUTE_X:
                        case MODE_ABSOLUTE_Y:
                        case MODE_INDIRECT:
                                fprintf(stdout, MODE_FORMAT[instruction->mode], operand);
                                break;
                        case MODE_IMMEDIATE:
                        case MODE_INDIRECT_X:
                        case MODE_INDIRECT_Y:
                        case MODE_ZEROPAGE:
                        case MODE_ZEROPAGE_X:
                        case MODE_ZEROPAGE_Y:
                                fprintf(stdout, MODE_FORMAT[instruction->mode], operand & UINT8_MAX);
                                break;
                        case MODE_RELATIVE:
                                fprintf(stdout, MODE_FORMAT[instruction->mode], operand & UINT8_MAX, (int8_t)(operand & UINT8_MAX),
                                        address + (int8_t)(operand & UINT8_MAX));
                                break;
                        case MODE_IMPLIED:
                        default:
                                break;
                }

                fprintf(stdout, "\n");
        }

exit:
        return result;
}

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
                                                fprintf(stdout, "%s>\t%04X\n", REGISTER[request.address], response.data & UINT16_MAX);
                                                break;
                                        case NES_PROCESSOR_STATUS:
                                                fprintf(stdout, "%s>\t%02X  [%c%c%c%c%c%c%c]\n", REGISTER[request.address], response.data & UINT8_MAX,
                                                        (response.data & STATUS_NEGATIVE) ? 'N' : '-', (response.data & STATUS_OVERFLOW) ? 'O' : '-',
                                                        (response.data & STATUS_BREAKPOINT) ? 'B' : '-', (response.data & STATUS_DECIMAL) ? 'D' : '-',
                                                        (response.data & STATUS_INTERRUPT_DISABLE) ? 'I' : '-', (response.data & STATUS_ZERO) ? 'Z' : '-',
                                                        (response.data & STATUS_CARRY) ? 'C' : '-');
                                                break;
                                        default:
                                                fprintf(stdout, "%s>\t%02X\n", REGISTER[request.address], response.data & UINT8_MAX);
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
                                        fprintf(stdout, "%04X>\t", request.address);
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

#ifdef __cplusplus
}
#endif /* __cplusplus */
