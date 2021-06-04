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

#include "./processor_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t
nes_processor_execute_breakpoint(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        nes_processor_register_t status = { .low = processor->status.low };

        TRACE(LEVEL_VERBOSE, "Processor BRK [%04X]", processor->fetched.address.word);
        nes_processor_push_word(processor, processor->program_counter.word);
        status.breakpoint = BREAKPOINT_SET;
        nes_processor_push(processor, status.low);
        processor->program_counter.word = nes_processor_read_word(processor, MASKABLE_ADDRESS);
        processor->status.interrupt_disabled = true;

        return 0;
}

uint8_t
nes_processor_execute_clear(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{

        switch(instruction->opcode) {
                case OPCODE_CLC:
                        processor->status.carry = false;
                        break;
                case OPCODE_CLD:
                        processor->status.decimal = false;
                        break;
                case OPCODE_CLI:
                        processor->status.interrupt_disabled = false;
                        break;
                case OPCODE_CLV:
                        processor->status.overflow = false;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid clear instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return 0;
}

uint8_t
nes_processor_execute_decrement(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{

        switch(instruction->opcode) {
                case OPCODE_DEX:
                        --processor->index_x.low;
                        processor->status.negative = processor->index_x.negative;
                        processor->status.zero = !processor->index_x.low;
                        break;
                case OPCODE_DEY:
                        --processor->index_y.low;
                        processor->status.negative = processor->index_y.negative;
                        processor->status.zero = !processor->index_y.low;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid decrement instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return 0;
}

uint8_t
nes_processor_execute_illegal(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        TRACE(LEVEL_WARNING, "Illegal instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode, instruction->opcode);

        return 0;
}

uint8_t
nes_processor_execute_increment(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{

        switch(instruction->opcode) {
                case OPCODE_INX:
                        ++processor->index_x.low;
                        processor->status.negative = processor->index_x.negative;
                        processor->status.zero = !processor->index_x.low;
                        break;
                case OPCODE_INY:
                        ++processor->index_y.low;
                        processor->status.negative = processor->index_y.negative;
                        processor->status.zero = !processor->index_y.low;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid increment instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return 0;
}

uint8_t
nes_processor_execute_no_operation(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        return 0;
}

uint8_t
nes_processor_execute_pull(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        nes_processor_register_t status = {};

        switch(instruction->opcode) {
                case OPCODE_PLA:
                        processor->accumulator.low = nes_processor_pull(processor);
                        processor->status.negative = processor->accumulator.negative;
                        processor->status.zero = !processor->accumulator.low;
                        break;
                case OPCODE_PLP:
                        status.low = processor->status.low;
                        processor->status.low = nes_processor_pull(processor);
                        processor->status.breakpoint = status.low;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid pull instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return 0;
}

uint8_t
nes_processor_execute_push(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        nes_processor_register_t status = {};

        switch(instruction->opcode) {
                case OPCODE_PHA:
                        nes_processor_push(processor, processor->accumulator.low);
                        break;
                case OPCODE_PHP:
                        status.low = processor->status.low;
                        status.breakpoint = BREAKPOINT_SET;
                        nes_processor_push(processor, status.low);
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid push instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return 0;
}

uint8_t
nes_processor_execute_return(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        nes_processor_register_t status = {};

        switch(instruction->opcode) {
                case OPCODE_RTI:
                        status.low = processor->status.low;
                        processor->status.low = nes_processor_pull(processor);
                        processor->status.breakpoint = status.low;
                        processor->program_counter.word = nes_processor_pull_word(processor);
                        break;
                case OPCODE_RTS:
                        processor->program_counter.word = nes_processor_pull_word(processor) + 1;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid return instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return 0;
}

uint8_t
nes_processor_execute_set(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{

        switch(instruction->opcode) {
                case OPCODE_SEC:
                        processor->status.carry = true;
                        break;
                case OPCODE_SED:
                        processor->status.decimal = true;
                        break;
                case OPCODE_SEI:
                        processor->status.interrupt_disabled = true;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid set instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return 0;
}

uint8_t
nes_processor_execute_transfer(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{

        switch(instruction->opcode) {
                case OPCODE_TAX:
                        processor->index_x.low = processor->accumulator.low;
                        processor->status.negative = processor->index_x.negative;
                        processor->status.zero = !processor->index_x.low;
                        break;
                case OPCODE_TAY:
                        processor->index_y.low = processor->accumulator.low;
                        processor->status.negative = processor->index_y.negative;
                        processor->status.zero = !processor->index_y.low;
                        break;
                case OPCODE_TSX:
                        processor->index_x.low = processor->stack_pointer.low;
                        processor->status.negative = processor->index_x.negative;
                        processor->status.zero = !processor->index_x.low;
                        break;
                case OPCODE_TXA:
                        processor->accumulator.low = processor->index_x.low;
                        processor->status.negative = processor->accumulator.negative;
                        processor->status.zero = !processor->accumulator.low;
                        break;
                case OPCODE_TXS:
                        processor->stack_pointer.low = processor->index_x.low;
                        break;
                case OPCODE_TYA:
                        processor->accumulator.low = processor->index_y.low;
                        processor->status.negative = processor->accumulator.negative;
                        processor->status.zero = !processor->accumulator.low;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid transfer instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return 0;
}

uint8_t
nes_processor_fetch(
        __inout nes_processor_t *processor
        )
{
        return nes_processor_read(processor, processor->program_counter.word++);
}

uint16_t
nes_processor_fetch_word(
        __inout nes_processor_t *processor
        )
{
        return nes_processor_fetch(processor) | (nes_processor_fetch(processor) << CHAR_BIT);
}

void
nes_processor_instruction(
        __inout nes_processor_t *processor
        )
{
        const nes_processor_instruction_t *instruction;

        processor->fetched.address.word = processor->program_counter.word;
        instruction = &INSTRUCTION[processor->fetched.opcode = nes_processor_fetch(processor)];

        switch(instruction->mode) {

                /**
                 * TODO: FETCH OPERAND BASED OFF MODE
                 *       POPULATE processor->fetched.operand
                 */

                case MODE_ABSOLUTE:

                        /* TODO */

                        break;
                case MODE_ABSOLUTE_X:

                        /* TODO */

                        break;
                case MODE_ABSOLUTE_Y:

                        /* TODO */

                        break;
                case MODE_IMMEDIATE:
                        processor->fetched.operand.address.word = processor->program_counter.word;
                        processor->fetched.operand.address_indirect.word = 0;
                        processor->fetched.operand.data.low = nes_processor_fetch(processor);
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_IMPLIED:
                        processor->fetched.operand.address.word = 0;
                        processor->fetched.operand.address_indirect.word = 0;
                        processor->fetched.operand.data.low = processor->accumulator.low;
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_INDIRECT:

                        /* TODO */

                        break;
                case MODE_INDIRECT_X:

                        /* TODO */

                        break;
                case MODE_INDIRECT_Y:

                        /* TODO */

                        break;
                case MODE_RELATIVE:

                        /* TODO */

                        break;
                case MODE_ZEROPAGE:

                        /* TODO */

                        break;
                case MODE_ZEROPAGE_X:

                        /* TODO */

                        break;
                case MODE_ZEROPAGE_Y:

                        /* TODO */

                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid addressing mode: [%04X] %i", processor->fetched.address.word, instruction->mode);
                        break;
        }

        TRACE_PROCESSOR_INSTRUCTION(LEVEL_VERBOSE, processor, instruction);
        processor->cycles = instruction->cycles + HANDLER[processor->fetched.opcode](processor, instruction);
}

void
nes_processor_interrupt(
        __inout nes_processor_t *processor,
        __in bool maskable
        )
{

        if(maskable) {
                processor->pending.maskable = true;
        } else {
                processor->pending.non_maskable = true;
        }
}

void
nes_processor_interrupt_maskable(
        __inout nes_processor_t *processor
        )
{
        nes_processor_register_t status = { .low = processor->status.low };

        TRACE(LEVEL_VERBOSE, "%s", "Processor IRQ");
        nes_processor_push_word(processor, processor->program_counter.word);
        status.breakpoint = BREAKPOINT_CLEAR;
        nes_processor_push(processor, status.low);
        processor->program_counter.word = nes_processor_read_word(processor, MASKABLE_ADDRESS);
        processor->status.interrupt_disabled = true;
        processor->pending.maskable = false;
        processor->cycles = MASKABLE_CYCLES;
}

void
nes_processor_interrupt_non_maskable(
        __inout nes_processor_t *processor
        )
{
        nes_processor_register_t status = { .low = processor->status.low };

        TRACE(LEVEL_VERBOSE, "%s", "Processor NMI");
        nes_processor_push_word(processor, processor->program_counter.word);
        status.breakpoint = BREAKPOINT_CLEAR;
        nes_processor_push(processor, status.low);
        processor->program_counter.word = nes_processor_read_word(processor, NON_MASKABLE_ADDRESS);
        processor->status.interrupt_disabled = true;
        processor->pending.non_maskable = false;
        processor->cycles = NON_MASKABLE_CYCLES;
}

uint8_t
nes_processor_pull(
        __inout nes_processor_t *processor
        )
{
        return nes_processor_read(processor, STACK_ADDRESS | ++processor->stack_pointer.low);
}

uint16_t
nes_processor_pull_word(
        __inout nes_processor_t *processor
        )
{
        return nes_processor_pull(processor) | (nes_processor_pull(processor) << CHAR_BIT);
}

void
nes_processor_push(
        __inout nes_processor_t *processor,
        __in uint8_t data
        )
{
        nes_processor_write(processor, STACK_ADDRESS | processor->stack_pointer.low--, data);
}

void
nes_processor_push_word(
        __inout nes_processor_t *processor,
        __in uint16_t data
        )
{
        nes_processor_push(processor, data >> CHAR_BIT);
        nes_processor_push(processor, data);
}

uint8_t
nes_processor_read(
        __in const nes_processor_t *processor,
        __in uint16_t address
        )
{
        return nes_bus_read(BUS_PROCESSOR, address);
}

uint16_t
nes_processor_read_word(
        __in const nes_processor_t *processor,
        __in uint16_t address
        )
{
        return nes_processor_read(processor, address) | (nes_processor_read(processor, address + 1) << CHAR_BIT);
}

void
nes_processor_reset(
        __inout nes_processor_t *processor
        )
{
        TRACE(LEVEL_VERBOSE, "%s", "Processor RESET");
        memset(processor, 0, sizeof(*processor));
        nes_processor_push_word(processor, processor->program_counter.word);
        nes_processor_push(processor, processor->status.low);
        processor->program_counter.word = nes_processor_read_word(processor, RESET_ADDRESS);
        processor->status.breakpoint = BREAKPOINT_SET;
        processor->status.interrupt_disabled = true;
        processor->cycles = RESET_CYCLES;
        TRACE_PROCESSOR(LEVEL_VERBOSE, processor);
}

void
nes_processor_step(
        __inout nes_processor_t *processor
        )
{

        if(!processor->cycles) {

                if(processor->pending.transfer) {
                        nes_processor_transfer_byte(processor);
                } else {

                        if(processor->pending.non_maskable) {
                                nes_processor_interrupt_non_maskable(processor);
                        } else if(processor->pending.maskable && !processor->status.interrupt_disabled) {
                                nes_processor_interrupt_maskable(processor);
                        } else {
                                nes_processor_instruction(processor);
                        }

                        TRACE_PROCESSOR(LEVEL_VERBOSE, processor);
                }
        }

        --processor->cycles;
}

#ifndef NDEBUG

void
nes_processor_trace(
        __in int level,
        __in const nes_processor_t *processor
        )
{
        TRACE(level, "Processor CYCLES: %u", processor->cycles);
        TRACE(level, "Processor REG-PC: %04X", processor->program_counter.word);
        TRACE(level, "Processor REG-SP: %02X", processor->stack_pointer.low);
        TRACE(level, "Processor REG-S: %02X [%c%c%c%c%c%c%c]", processor->status.low,
                processor->status.negative ? 'N' : '-', processor->status.overflow ? 'O' : '-',
                processor->status.breakpoint ? 'B' : '-', processor->status.decimal ? 'D' : '-',
                processor->status.interrupt_disabled ? 'I' : '-', processor->status.zero ? 'Z' : '-',
                processor->status.carry ? 'C' : '-');

        if(processor->pending.transfer) {
                TRACE(level, "Processor REG-P: %02X [%c%c%c], %04X (%u/%u)", processor->pending.low,
                        processor->pending.maskable ? 'M' : '-', processor->pending.non_maskable ? 'N' : '-',
                        processor->pending.transfer ? 'T' : '-', processor->transfer.source.word,
                        processor->transfer.offset.low + 1, PAGE_WIDTH);
        } else {
                TRACE(level, "Processor REG-P: %02X [%c%c%c]", processor->pending.low,
                        processor->pending.maskable ? 'M' : '-', processor->pending.non_maskable ? 'N' : '-',
                        processor->pending.transfer ? 'T' : '-');
        }

        TRACE(level, "Processor REG-A: %02X", processor->accumulator.low);
        TRACE(level, "Processor REG-X: %02X", processor->index_x.low);
        TRACE(level, "Processor REG-Y: %02X", processor->index_y.low);
}

void
nes_processor_trace_instruction(
        __in int level,
        __in const nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        /* TODO: TRACE PROCESSOR INSTRUCTION DISASSEMBLY */
}

#endif /* NDEBUG */

void
nes_processor_transfer(
        __inout nes_processor_t *processor,
        __in uint8_t page
        )
{
        processor->pending.transfer = true;
        processor->transfer.source.word = (page * PAGE_WIDTH);
        processor->transfer.offset.low = 0;
        TRACE(LEVEL_VERBOSE, "Processor transfer: %04X-%04X (Page %02X)", processor->transfer.source.word,
                processor->transfer.source.word + PAGE_WIDTH - 1, page);
}

void
nes_processor_transfer_byte(
        __inout nes_processor_t *processor
        )
{

        /* TODO: TRANSFER BYTE FROM CPU[SOURCE+OFFSET] TO OAM[OFFSET] */

        if(processor->transfer.offset.low == UINT8_MAX) {
                processor->pending.transfer = false;
                memset(&processor->transfer, 0, sizeof(processor->transfer));
                TRACE(LEVEL_VERBOSE, "%s", "Processor transfer complete");
        } else {
                TRACE(LEVEL_VERBOSE, "Processor transfer byte: %04X (%u/%u)", processor->transfer.source.word,
                        processor->transfer.offset.low + 1, PAGE_WIDTH);
                ++processor->transfer.offset.low;
        }

        processor->cycles = TRANSFER_CYCLES;
}

void
nes_processor_write(
        __inout nes_processor_t *processor,
        __in uint16_t address,
        __in uint8_t data
        )
{
        nes_bus_write(BUS_PROCESSOR, address, data);
}

void
nes_processor_write_word(
        __inout nes_processor_t *processor,
        __in uint16_t address,
        __in uint16_t data
        )
{
        nes_processor_write(processor, address, data);
        nes_processor_write(processor, address + 1, data >> CHAR_BIT);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
