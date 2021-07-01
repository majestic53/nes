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
        NES_ACTION_CARTRIDGE_HEADER, /* Read cartridge header */
        NES_ACTION_MAX,
};

/**
 * NES processor enum
 */
enum {
        NES_PROCESSOR_PROGRAM_COUNTER = 0, /* Processor program counter register */
        NES_PROCESSOR_STACK_POINTER, /* Processor stack pointer register */
        NES_PROCESSOR_STATUS, /* Processor status register */
        NES_PROCESSOR_ACCUMULATOR, /* Processor accumulator register */
        NES_PROCESSOR_INDEX_X, /* Processor index-x register */
        NES_PROCESSOR_INDEX_Y, /* Processor index-y register */
        NES_PROCESSOR_MAX,
};

/**
 * NES action struct
 */
typedef struct {
        int type; /* Action type */
        uint16_t address; /* Action address */

        union {
                uint16_t data; /* Action data */
                const void *ptr; /* Action pointer */
        };
} nes_action_t;

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
        uint8_t magic[4];
        uint8_t rom_program_count;
        uint8_t rom_character_count;

        struct {
                uint8_t mirroring : 1;
                uint8_t battery : 1;
                uint8_t trainer : 1;
                uint8_t four_screen : 1;
                uint8_t mapper_low : 4;
        } flag_6;

        struct {
                uint8_t unused : 2;
                uint8_t version : 2;
                uint8_t mapper_high : 4;
        } flag_7;

        uint8_t ram_program_count;
#endif /* NES_INES_VERSION >= NES_INES_VERSION_1 */
#if NES_INES_VERSION == NES_INES_VERSION_1
        uint8_t unused[7];
#endif /* NES_INES_VERSION == NES_INES_VERSION_1 */
} nes_header_t;

/**
 * NES ROM struct
 */
typedef struct {
        nes_buffer_t data; /* ROM data */
        const char *path; /* ROM path */
} nes_rom_t;

/**
 * NES version struct
 */
typedef struct {
	int major; /* Major number */
	int minor; /* Minor number */
	int patch; /* Patch number */
} nes_version_t;

/**
 * NES configuration struct
 */
typedef struct {
#if NES_API_VERSION >= NES_API_VERSION_1
        nes_display_t display; /* Display configuration */
        nes_rom_t rom; /* ROM configuration */
#endif /* NES_API_VERSION >= NES_API_VERSION_1 */
} nes_t;

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
