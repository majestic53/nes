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

#include "./processor_trace_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef NDEBUG

static const char *MODE_FORMAT[] = {
        " [%04X]->%02X", /* MODE_ABSOLUTE */
        " %04X+X [%04X]->%02X", /* MODE_ABSOLUTE_X */
        " %04X+Y [%04X]->%02X", /* MODE_ABSOLUTE_Y */
        " %02X", /* MODE_IMMEDIATE */
        "", /* MODE_IMPLIED */
        " (%04X) [%04X]", /* MODE_INDIRECT */
        " (%02X,X) [%04X]->%02X", /* MODE_INDIRECT_X */
        " (%02X),Y [%04X]->%02X", /* MODE_INDIRECT_Y */
        " %02X(%i) [%04X]", /* MODE_RELATIVE */
        " [%02X]->%02X", /* MODE_ZEROPAGE */
        " %02X+X [%02X]->%02X", /* MODE_ZEROPAGE_X */
        " %02X+Y [%02X]->%02X", /* MODE_ZEROPAGE_Y */
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

void
nes_processor_trace(
        __in int level,
        __in const nes_processor_t *processor
        )
{

        if(level <= LEVEL) {
                TRACE(level, "Processor cycles: %u", processor->cycles);
                TRACE(level, "Processor PC: %04X", processor->program_counter.word);
                TRACE(level, "Processor SP: %02X", processor->stack_pointer.low);
                TRACE(level, "Processor S: %02X [%c%c%c%c%c%c%c]", processor->status.low,
                        processor->status.negative ? 'N' : '-', processor->status.overflow ? 'O' : '-',
                        processor->status.breakpoint ? 'B' : '-', processor->status.decimal ? 'D' : '-',
                        processor->status.interrupt_disabled ? 'I' : '-', processor->status.zero ? 'Z' : '-',
                        processor->status.carry ? 'C' : '-');

                if(processor->pending.transfer) {
                        TRACE(level, "Processor P: %02X [%c%c%c], %04X (%u/%u)", processor->pending.low,
                                processor->pending.maskable ? 'M' : '-', processor->pending.non_maskable ? 'N' : '-',
                                processor->pending.transfer ? 'T' : '-', processor->transfer.source.word,
                                processor->transfer.offset.low + 1, PAGE_WIDTH);
                } else {
                        TRACE(level, "Processor P: %02X [%c%c%c]", processor->pending.low,
                                processor->pending.maskable ? 'M' : '-', processor->pending.non_maskable ? 'N' : '-',
                                processor->pending.transfer ? 'T' : '-');
                }

                TRACE(level, "Processor A: %02X", processor->accumulator.low);
                TRACE(level, "Processor X: %02X", processor->index_x.low);
                TRACE(level, "Processor Y: %02X", processor->index_y.low);
        }
}

void
nes_processor_trace_instruction(
        __in int level,
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{

        if(level <= LEVEL) {
                memset(processor->format, 0, FORMAT_MAX);
                snprintf(processor->format, FORMAT_MAX, "[%04X] %02X", processor->fetched.address.word, processor->fetched.opcode);

                switch(instruction->mode) {
                        case MODE_ABSOLUTE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %02X %02X",
                                        processor->fetched.operand.address.low, processor->fetched.operand.address.high);
                                break;
                        case MODE_ABSOLUTE_X:
                        case MODE_ABSOLUTE_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %02X %02X",
                                        processor->fetched.operand.address_indirect.low, processor->fetched.operand.address_indirect.high);
                                break;
                        case MODE_IMMEDIATE:
                        case MODE_RELATIVE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %02X",
                                        processor->fetched.operand.data.low);
                                break;
                        case MODE_INDIRECT:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %02X %02X",
                                        processor->fetched.operand.address_indirect.low, processor->fetched.operand.address_indirect.high);
                                break;
                        case MODE_INDIRECT_X:
                        case MODE_INDIRECT_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %02X",
                                        processor->fetched.operand.address_indirect.low);
                                break;
                        case MODE_ZEROPAGE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %02X",
                                        processor->fetched.operand.address.low);
                                break;
                        case MODE_ZEROPAGE_X:
                        case MODE_ZEROPAGE_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %02X",
                                        processor->fetched.operand.address_indirect.low);
                                break;
                        case MODE_IMPLIED:
                        default:
                                break;
                }

                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %s", OPCODE_FORMAT[instruction->opcode]);

                switch(instruction->mode) {
                        case MODE_ABSOLUTE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_FORMAT[instruction->mode],
                                        processor->fetched.operand.address.word, processor->fetched.operand.data.low);
                                break;
                        case MODE_ABSOLUTE_X:
                        case MODE_ABSOLUTE_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_FORMAT[instruction->mode],
                                        processor->fetched.operand.address_indirect.word, processor->fetched.operand.address.word,
                                        processor->fetched.operand.data.low);
                                break;
                        case MODE_IMMEDIATE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_FORMAT[instruction->mode],
                                        processor->fetched.operand.data.low);
                                break;
                        case MODE_INDIRECT:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_FORMAT[instruction->mode],
                                        processor->fetched.operand.address_indirect.word, processor->fetched.operand.address.word);
                                break;
                        case MODE_INDIRECT_X:
                        case MODE_INDIRECT_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_FORMAT[instruction->mode],
                                        processor->fetched.operand.address_indirect.low, processor->fetched.operand.address.word,
                                        processor->fetched.operand.data.low);
                                break;
                        case MODE_RELATIVE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_FORMAT[instruction->mode],
                                        processor->fetched.operand.data.low, processor->fetched.operand.data.low, processor->fetched.operand.address.word);
                                break;
                        case MODE_ZEROPAGE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_FORMAT[instruction->mode],
                                        processor->fetched.operand.address.low, processor->fetched.operand.data.low);
                                break;
                        case MODE_ZEROPAGE_X:
                        case MODE_ZEROPAGE_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_FORMAT[instruction->mode],
                                        processor->fetched.operand.address_indirect.low, processor->fetched.operand.address.low,
                                        processor->fetched.operand.data.low);
                                break;
                        case MODE_IMPLIED:
                        default:
                                break;
                }

                if(processor->fetched.operand.page_boundary) {
                        snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " (PAGE-BOUNDARY)");
                }

                TRACE(level, "%s", processor->format);
        }
}

#endif /* NDEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */
