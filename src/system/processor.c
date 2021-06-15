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
nes_processor_execute_arithmetic(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;
        nes_processor_register_t value = {};

        switch(instruction->opcode) {
                case OPCODE_ADC:
                        break;
                case OPCODE_SBC:
                        processor->fetched.operand.data.low = ~processor->fetched.operand.data.low;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid arithmetic instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        value.word = processor->accumulator.low + processor->fetched.operand.data.low + (processor->status.carry ? 1 : 0);
        processor->status.carry = value.word > UINT8_MAX;
        processor->status.overflow = !(processor->accumulator.negative ^ processor->fetched.operand.data.negative)
                                        && (processor->accumulator.negative ^ value.negative);
        processor->accumulator.low = value.low;
        processor->status.negative = processor->accumulator.negative;
        processor->status.zero = !processor->accumulator.low;

        if(processor->fetched.operand.page_boundary) {
                ++result;
        }

        return result;
}

uint8_t
nes_processor_execute_bit(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        processor->status.negative = processor->fetched.operand.data.negative;
        processor->status.overflow = processor->fetched.operand.data.overflow;
        processor->status.zero = !(processor->accumulator.low & processor->fetched.operand.data.low);

        return 0;
}

uint8_t
nes_processor_execute_bitwise(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;

        switch(instruction->opcode) {
                case OPCODE_AND:
                        processor->accumulator.low &= processor->fetched.operand.data.low;
                        break;
                case OPCODE_EOR:
                        processor->accumulator.low ^= processor->fetched.operand.data.low;
                        break;
                case OPCODE_ORA:
                        processor->accumulator.low |= processor->fetched.operand.data.low;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid binary instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        processor->status.negative = processor->accumulator.negative;
        processor->status.zero = !processor->accumulator.low;

        if(processor->fetched.operand.page_boundary) {
                ++result;
        }

        return result;
}

uint8_t
nes_processor_execute_branch(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;
        bool taken = false;

        switch(instruction->opcode) {
                case OPCODE_BCC:
                        taken = !processor->status.carry;
                        break;
                case OPCODE_BCS:
                        taken = processor->status.carry;
                        break;
                case OPCODE_BEQ:
                        taken = processor->status.zero;
                        break;
                case OPCODE_BMI:
                        taken = processor->status.negative;
                        break;
                case OPCODE_BNE:
                        taken = !processor->status.zero;
                        break;
                case OPCODE_BPL:
                        taken = !processor->status.negative;
                        break;
                case OPCODE_BVC:
                        taken = !processor->status.overflow;
                        break;
                case OPCODE_BVS:
                        taken = processor->status.overflow;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid branch instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        if(taken) {
                processor->program_counter.word = processor->fetched.operand.address.word;

                if(processor->fetched.operand.page_boundary) {
                        ++result;
                }

                ++result;
        }

        return result;
}

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
nes_processor_execute_compare(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;
        nes_processor_register_t value = {};

        switch(instruction->opcode) {
                case OPCODE_CMP:
                        value.low = processor->accumulator.low;
                        break;
                case OPCODE_CPX:
                        value.low = processor->index_x.low;
                        break;
                case OPCODE_CPY:
                        value.low = processor->index_y.low;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid compare instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        processor->status.carry = (value.low >= processor->fetched.operand.data.low);
        processor->status.zero = (value.low == processor->fetched.operand.data.low);
        value.low -= processor->fetched.operand.data.low;
        processor->status.negative = value.negative;

        if(processor->fetched.operand.page_boundary) {
                ++result;
        }

        return result;
}

uint8_t
nes_processor_execute_decrement(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{

        switch(instruction->opcode) {
                case OPCODE_DEC:
                        nes_processor_write(processor, processor->fetched.operand.address.word, --processor->fetched.operand.data.low);
                        processor->status.negative = processor->fetched.operand.data.negative;
                        processor->status.zero = !processor->fetched.operand.data.low;
                        break;
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
                case OPCODE_INC:
                        nes_processor_write(processor, processor->fetched.operand.address.word, ++processor->fetched.operand.data.low);
                        processor->status.negative = processor->fetched.operand.data.negative;
                        processor->status.zero = !processor->fetched.operand.data.low;
                        break;
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
nes_processor_execute_jump(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;

        switch(instruction->opcode) {
                case OPCODE_JMP:
                        processor->program_counter.word = processor->fetched.operand.address.word;
                        break;
                case OPCODE_JSR:
                        nes_processor_push_word(processor, processor->program_counter.word - 1);
                        processor->program_counter.word = processor->fetched.operand.address.word;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid jump instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return result;
}

uint8_t
nes_processor_execute_load(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;

        switch(instruction->opcode) {
                case OPCODE_LDA:
                        processor->accumulator.low = processor->fetched.operand.data.low;
                        break;
                case OPCODE_LDX:
                        processor->index_x.low = processor->fetched.operand.data.low;
                        break;
                case OPCODE_LDY:
                        processor->index_y.low = processor->fetched.operand.data.low;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid load instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        processor->status.negative = processor->fetched.operand.data.negative;
        processor->status.zero = !processor->fetched.operand.data.low;

        if(processor->fetched.operand.page_boundary) {
                ++result;
        }

        return result;
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
nes_processor_execute_rotate(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;
        bool carry = processor->status.carry;
        nes_processor_register_t value = { .low = processor->fetched.operand.data.low };

        switch(instruction->opcode) {
                case OPCODE_ROL:
                        processor->status.carry = value.negative;
                        value.low <<= 1;
                        value.carry = carry;
                        break;
                case OPCODE_ROR:
                        processor->status.carry = value.carry;
                        value.low >>= 1;
                        value.negative = carry;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid rotate instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        processor->status.negative = value.negative;
        processor->status.zero = !value.low;

        if(instruction->mode != MODE_IMPLIED) {
                nes_processor_write(processor, processor->fetched.operand.address.word, value.low);
        } else {
                processor->accumulator.low = value.low;
        }

        return result;
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
nes_processor_execute_shift(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;
        nes_processor_register_t value = { .low = processor->fetched.operand.data.low };

        switch(instruction->opcode) {
                case OPCODE_ASL:
                        processor->status.carry = value.negative;
                        value.low <<= 1;
                        break;
                case OPCODE_LSR:
                        processor->status.carry = value.carry;
                        value.low >>= 1;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid shift instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        processor->status.negative = value.negative;
        processor->status.zero = !value.low;

        if(instruction->mode != MODE_IMPLIED) {
                nes_processor_write(processor, processor->fetched.operand.address.word, value.low);
        } else {
                processor->accumulator.low = value.low;
        }

        return result;
}

uint8_t
nes_processor_execute_store(
        __inout nes_processor_t *processor,
        __in const nes_processor_instruction_t *instruction
        )
{
        uint8_t result = 0;

        switch(instruction->opcode) {
                case OPCODE_STA:
                        nes_processor_write(processor, processor->fetched.operand.address.word, processor->accumulator.low);
                        break;
                case OPCODE_STX:
                        nes_processor_write(processor, processor->fetched.operand.address.word, processor->index_x.low);
                        break;
                case OPCODE_STY:
                        nes_processor_write(processor, processor->fetched.operand.address.word, processor->index_y.low);
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid store instruction: [%04X] %02X (%i)", processor->fetched.address.word, processor->fetched.opcode,
                                instruction->opcode);
                        break;
        }

        return result;
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

void
nes_processor_fetch_operand(
        __inout nes_processor_t *processor,
        __in int mode
        )
{

        switch(mode) {
                case MODE_ABSOLUTE:
                        processor->fetched.operand.address.word = nes_processor_fetch_word(processor);
                        processor->fetched.operand.address_indirect.word = 0;
                        processor->fetched.operand.data.word = nes_processor_read(processor, processor->fetched.operand.address.word);
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_ABSOLUTE_X:
                        processor->fetched.operand.address_indirect.word = nes_processor_fetch_word(processor);
                        processor->fetched.operand.address.word = processor->fetched.operand.address_indirect.word + processor->index_x.low;
                        processor->fetched.operand.data.word = nes_processor_read(processor, processor->fetched.operand.address.word);
                        processor->fetched.operand.page_boundary = (processor->fetched.operand.address.high != processor->fetched.operand.address_indirect.high);
                        break;
                case MODE_ABSOLUTE_Y:
                        processor->fetched.operand.address_indirect.word = nes_processor_fetch_word(processor);
                        processor->fetched.operand.address.word = processor->fetched.operand.address_indirect.word + processor->index_y.low;
                        processor->fetched.operand.data.word = nes_processor_read(processor, processor->fetched.operand.address.word);
                        processor->fetched.operand.page_boundary = (processor->fetched.operand.address.high != processor->fetched.operand.address_indirect.high);
                        break;
                case MODE_IMMEDIATE:
                        processor->fetched.operand.address.word = processor->program_counter.word;
                        processor->fetched.operand.address_indirect.word = 0;
                        processor->fetched.operand.data.word = nes_processor_fetch(processor);
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_IMPLIED:
                        processor->fetched.operand.address.word = 0;
                        processor->fetched.operand.address_indirect.word = 0;
                        processor->fetched.operand.data.word = processor->accumulator.low;
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_INDIRECT:
                        processor->fetched.operand.address_indirect.word = nes_processor_fetch_word(processor);

                        if(processor->fetched.operand.address_indirect.low == UINT8_MAX) {
                                processor->fetched.operand.address.low = nes_processor_read(processor, processor->fetched.operand.address_indirect.word);
                                processor->fetched.operand.address.high = nes_processor_read(processor, processor->fetched.operand.address_indirect.high << CHAR_BIT);
                        } else {
                                processor->fetched.operand.address.word = nes_processor_read_word(processor, processor->fetched.operand.address_indirect.word);
                        }

                        processor->fetched.operand.data.word = 0;
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_INDIRECT_X:
                        processor->fetched.operand.address_indirect.word = nes_processor_fetch(processor);
                        processor->fetched.operand.address.low = nes_processor_read(processor,
                                        (processor->fetched.operand.address_indirect.word + processor->index_x.low) & UINT8_MAX);
                        processor->fetched.operand.address.high = nes_processor_read(processor,
                                        (processor->fetched.operand.address_indirect.word + processor->index_x.low + 1) & UINT8_MAX);
                        processor->fetched.operand.data.word = nes_processor_read(processor, processor->fetched.operand.address.word);
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_INDIRECT_Y:
                        processor->fetched.operand.address_indirect.word = nes_processor_fetch(processor);
                        processor->fetched.operand.data.low = nes_processor_read(processor, processor->fetched.operand.address_indirect.low);
                        processor->fetched.operand.data.high = nes_processor_read(processor, (processor->fetched.operand.address_indirect.low + 1) & UINT8_MAX);
                        processor->fetched.operand.address.word = processor->fetched.operand.data.word + processor->index_y.low;
                        processor->fetched.operand.page_boundary = (processor->fetched.operand.address.high != processor->fetched.operand.data.high);
                        processor->fetched.operand.data.word = nes_processor_read(processor, processor->fetched.operand.address.word);
                        break;
                case MODE_RELATIVE:
                        processor->fetched.operand.data.word = nes_processor_fetch(processor);

                        if(processor->fetched.operand.data.negative) {
                                processor->fetched.operand.data.high = UINT8_MAX;
                        }

                        processor->fetched.operand.address.word = processor->program_counter.word + processor->fetched.operand.data.word;
                        processor->fetched.operand.address_indirect.word = 0;
                        processor->fetched.operand.page_boundary = (processor->program_counter.high != processor->fetched.operand.address.high);
                        break;
                case MODE_ZEROPAGE:
                        processor->fetched.operand.address.word = nes_processor_fetch(processor);
                        processor->fetched.operand.address_indirect.word = 0;
                        processor->fetched.operand.data.word = nes_processor_read(processor, processor->fetched.operand.address.word);
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_ZEROPAGE_X:
                        processor->fetched.operand.address_indirect.word = nes_processor_fetch(processor);
                        processor->fetched.operand.address.word = (processor->fetched.operand.address_indirect.word + processor->index_x.low) & UINT8_MAX;
                        processor->fetched.operand.data.word = nes_processor_read(processor, processor->fetched.operand.address.word);
                        processor->fetched.operand.page_boundary = false;
                        break;
                case MODE_ZEROPAGE_Y:
                        processor->fetched.operand.address_indirect.word = nes_processor_fetch(processor);
                        processor->fetched.operand.address.word = (processor->fetched.operand.address_indirect.word + processor->index_y.low) & UINT8_MAX;
                        processor->fetched.operand.data.word = nes_processor_read(processor, processor->fetched.operand.address.word);
                        processor->fetched.operand.page_boundary = false;
                        break;
                default:
                        TRACE(LEVEL_WARNING, "Invalid addressing mode: [%04X] %i", processor->fetched.address.word, mode);
                        break;
        }
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
        processor->fetched.opcode = nes_processor_fetch(processor);
        instruction = &INSTRUCTION[processor->fetched.opcode];
        nes_processor_fetch_operand(processor, instruction->mode);
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

        if(level <= LEVEL) {
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
}

static const char *MODE_STR[] = {
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

static const char *OPCODE_STR[] = {
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

                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), " %s", OPCODE_STR[instruction->opcode]);

                switch(instruction->mode) {
                        case MODE_ABSOLUTE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_STR[instruction->mode],
                                        processor->fetched.operand.address.word, processor->fetched.operand.data.low);
                                break;
                        case MODE_ABSOLUTE_X:
                        case MODE_ABSOLUTE_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_STR[instruction->mode],
                                        processor->fetched.operand.address_indirect.word, processor->fetched.operand.address.word,
                                        processor->fetched.operand.data.low);
                                break;
                        case MODE_IMMEDIATE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_STR[instruction->mode],
                                        processor->fetched.operand.data.low);
                                break;
                        case MODE_INDIRECT:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_STR[instruction->mode],
                                        processor->fetched.operand.address_indirect.word, processor->fetched.operand.address.word);
                                break;
                        case MODE_INDIRECT_X:
                        case MODE_INDIRECT_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_STR[instruction->mode],
                                        processor->fetched.operand.address_indirect.low, processor->fetched.operand.address.word,
                                        processor->fetched.operand.data.low);
                                break;
                        case MODE_RELATIVE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_STR[instruction->mode],
                                        processor->fetched.operand.data.low, processor->fetched.operand.data.low, processor->fetched.operand.address.word);
                                break;
                        case MODE_ZEROPAGE:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_STR[instruction->mode],
                                        processor->fetched.operand.address.low, processor->fetched.operand.data.low);
                                break;
                        case MODE_ZEROPAGE_X:
                        case MODE_ZEROPAGE_Y:
                                snprintf(processor->format + strlen(processor->format), FORMAT_MAX - strlen(processor->format), MODE_STR[instruction->mode],
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
