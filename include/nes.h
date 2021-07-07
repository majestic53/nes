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

#ifndef NES_H_
#define NES_H_

#include <stddef.h>
#include <stdint.h>

/**
 * NES API versions
 */
#define NES_API_VERSION_1 1

/**
 * NES API current version
 */
#define NES_API_VERSION NES_API_VERSION_1

/**
 * NES iNES versions
 */
#define NES_INES_VERSION_1 1

/**
 * NES iNES version support
 */
#define NES_INES_VERSION NES_INES_VERSION_1

/**
 * NES error enum
 */
enum {
	NES_EVT = -1, /* Internal event */
	NES_OK, /* Success */
	NES_ERR, /* Failure */
};

/**
 * NES action enum
 */
enum {
        NES_ACTION_RUN = 0, /* Run emulator */
        NES_ACTION_STEP, /* Step emulator */
        NES_ACTION_BUS_READ, /* Read byte from bus */
        NES_ACTION_BUS_WRITE, /* Write byte to bus */
        NES_ACTION_PROCESSOR_READ, /* Read processor register */
        NES_ACTION_PROCESSOR_WRITE, /* Write processor register */
        NES_ACTION_MAPPER_READ, /* Read mapper register */
        NES_ACTION_MAPPER_WRITE, /* Write mapper register */
        NES_ACTION_CARTRIDGE_HEADER, /* Read cartridge header */
        NES_ACTION_MAX,
};

/**
 * NES mapper enum
 */
enum {
        NES_MAPPER_PROGRAM_ROM_0 = 0, /* Mapper program ROM register (PRG-ROM0) */
        NES_MAPPER_PROGRAM_ROM_1, /* Mapper program ROM register (PRG-ROM0) */
        NES_MAPPER_PROGRAM_RAM, /* Mapper program RAM register (PRG-RAM) */
        NES_MAPPER_CHARACTER_ROM, /* Mapper character ROM register (CHR-ROM) */
        NES_MAPPER_CHARACTER_RAM, /* Mapper character RAM register (CHR-RAM) */
        NES_MAPPER_MAX,
};

/**
 * NES processor enum
 */
enum {
        NES_PROCESSOR_PROGRAM_COUNTER = 0, /* Processor program counter register (PC) */
        NES_PROCESSOR_STACK_POINTER, /* Processor stack pointer register (SP) */
        NES_PROCESSOR_STATUS, /* Processor status register (S) */
        NES_PROCESSOR_PENDING, /* Processor pending register (P) */
        NES_PROCESSOR_ACCUMULATOR, /* Processor accumulator register (A) */
        NES_PROCESSOR_INDEX_X, /* Processor index-x register (X) */
        NES_PROCESSOR_INDEX_Y, /* Processor index-y register (Y) */
        NES_PROCESSOR_MAX,
};

/**
 * NES buffer struct
 */
typedef struct {
	uint8_t *ptr; /* Pointer to data */
	size_t length; /* Data length in bytes */
} nes_buffer_t;

/**
 * NES display struct
 */
typedef struct {
        bool fullscreen; /* DIsplay fullscreen */
        unsigned scale; /* Display scale */
} nes_display_t;

/**
 * NES header struct
 */
typedef struct {
#if NES_INES_VERSION >= NES_INES_VERSION_1
        uint8_t magic[4]; /* Magic string */
        uint8_t rom_program_count; /* Number of ROM-PRG banks */
        uint8_t rom_character_count; /* Number of ROM-CHR banks */

        struct {
                uint8_t mirroring : 1; /* Mirroring mode */
                uint8_t battery : 1; /* Battery present */
                uint8_t trainer : 1; /* Trainer present */
                uint8_t four_screen : 1; /* Four-screen mode */
                uint8_t mapper_low : 4; /* Mapper low-nibble */
        } flag_6;

        struct {
                uint8_t unused : 2; /* Unused fields */
                uint8_t version : 2; /* iNES version */
                uint8_t mapper_high : 4; /* Mapper high-nibble */
        } flag_7;

        uint8_t ram_program_count; /* Number of RAM-PRG banks */
#endif /* NES_INES_VERSION >= NES_INES_VERSION_1 */
#if NES_INES_VERSION == NES_INES_VERSION_1
        uint8_t unused[7]; /* Unused bytes */
#endif /* NES_INES_VERSION == NES_INES_VERSION_1 */
} nes_header_t;

/**
 * NES register struct
 */
typedef union {

        struct {

                union {

                        struct {
                                uint8_t transfer : 1; /* Pending transfer (DMA) */
                                uint8_t non_maskable : 1; /* Pending non-maskable interrupt (NMI) */
                                uint8_t maskable : 1; /* Pending maskable interrupt (IRQ) */
                        };

                        struct {
                                uint8_t carry : 1; /* Carry flag (C) */
                                uint8_t zero : 1; /* Zero flag (Z) */
                                uint8_t interrupt_disabled : 1; /* Interrupt disabled flag (I) */
                                uint8_t decimal : 1; /* Decimal flag (D) */
                                uint8_t breakpoint : 2; /* Breakpoint flags (B) */
                                uint8_t overflow : 1; /* Overflow flag (O) */
                                uint8_t negative : 1; /* Negative flag (N) */
                        };

                        uint8_t low; /* Low byte */
                };

                uint8_t high; /* High byte */
        };

        uint16_t word; /* Word */
        uint32_t dword; /* Double-word */
} nes_register_t;

/**
 * NES ROM struct
 */
typedef struct {
        nes_buffer_t data; /* ROM data */
        const char *path; /* ROM path */
} nes_rom_t;

/**
 * NES configuration struct
 */
typedef struct {
#if NES_API_VERSION >= NES_API_VERSION_1
        nes_display_t display; /* Display configuration */
        nes_rom_t rom; /* ROM configuration */
#endif /* NES_API_VERSION >= NES_API_VERSION_1 */
} nes_t;

/**
 * NES action struct
 */
typedef struct {
        int type; /* Action type */
        nes_register_t address; /* Action address */

        union {
                nes_register_t data; /* Action data */
                const void *ptr; /* Action pointer */
        };
} nes_action_t;

/**
 * NES version struct
 */
typedef struct {
        int major; /* Major number */
        int minor; /* Minor number */
        int patch; /* Patch number */
} nes_version_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Load NES emulator
 * @param[in] Pointer to configuration struct
 * @return NES_OK on success, NES_ERR otherwise
 */
int nes_load(const nes_t *);

/**
 * Unload NES emulator
 */
void nes_unload(void);

/**
 * Run NES action in emulator
 * @param[in] Pointer to request action struct
 * @param[inout] Pointer to response action struct
 * @return NES_OK on success, NES_ERR otherwise
 */
int nes_action(const nes_action_t *, nes_action_t *);

/**
 * Retrieve NES emulator error
 * @return Pointer to error string
 */
const char *nes_error(void);

/**
 * Retrieve NES emulator version
 * @return Pointer to version struct
 */
const nes_version_t *nes_version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NES_H_ */
