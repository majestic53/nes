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

static const char COMMAND[] = {
        'q', /* COMMAND_EXIT */
        'h', /* COMMAND_HELP */
        'v', /* COMMAND_VERSION */
        };

static const char *COMMAND_DESC[] = {
        "Exit debug mode", /* COMMAND_EXIT */
        "Show help information", /* COMMAND_HELP */
        "Show version information", /* COMMAND_VERSION */
        };

static const nes_launcher_debug_hdlr HANDLER[] = {
        NULL, /* COMMAND_EXIT */
        nes_launcher_debug_help, /* COMMAND_HELP */
        nes_launcher_debug_version, /* COMMAND_VERSION */
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

        for(int command = 0; command < COMMAND_MAX; ++command) {
                fprintf(stdout, "%c\t%s\n", COMMAND[command], COMMAND_DESC[command]);
        }

        return NES_OK;
}

int
nes_launcher_debug_version(
        __in const nes_launcher_t *launcher,
        __in const char *argument[],
        __in uint32_t count
        )
{
        fprintf(stdout, "%i.%i.%i\n", launcher->version->major, launcher->version->minor, launcher->version->patch);

        return NES_OK;
}

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
                        snprintf(prompt, PROMPT_MAX, "%s%04X%s", PROMPT_PREFIX, response.data, PROMPT_POSTFIX);
                } else {
                        snprintf(prompt, PROMPT_MAX, "%s%s%s", PROMPT_PREFIX, PROMPT_ERROR, PROMPT_POSTFIX);
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
                                fprintf(stderr, "Unsupported command: %s\n", input);
                                goto cleanup;
                        }

                        add_history(input);
                        next = strtok(input, " ");

                        while(next) {

                                if(count >= ARGUMENT_MAX) {
                                        fprintf(stderr, "Too many arguments: %u\n", count + 1);
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
                                                fprintf(stderr, "Command failed: %s\n", input);
                                        }
                                        break;
                                default:
                                        fprintf(stderr, "Unsupported command type: %i\n", command);
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
