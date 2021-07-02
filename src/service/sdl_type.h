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

#ifndef NES_SDL_TYPE_H_
#define NES_SDL_TYPE_H_

#ifdef SDL

#include <SDL2/SDL.h>
#include <libgen.h>
#include "../../include/service.h"

#define KEY_FULLSCREEN SDL_SCANCODE_F11

#define SCALE_MAX 4
#define SCALE_MIN 1

#define TITLE_MAX 128

#define WINDOW_HEIGHT 240
#define WINDOW_WIDTH 256

typedef union {

        struct {
                uint8_t blue;
                uint8_t green;
                uint8_t red;
                uint8_t alpha;
        };

        uint32_t raw;
} nes_color_t;

typedef struct {
	uint32_t frame;
	uint32_t frame_begin;
	float framerate;
	uint32_t framerate_begin;
	bool fullscreen;
	nes_color_t pixel[WINDOW_HEIGHT][WINDOW_WIDTH];
	SDL_Renderer *renderer;
        uint8_t scale;
	SDL_Texture *texture;
	char title[TITLE_MAX];
	SDL_version version;
	SDL_Window *window;
#ifndef NDEBUG
        char format[FORMAT_MAX];
#endif /* NDEBUG */
} nes_sdl_t;

static const nes_color_t BACKGROUND = {{ 0x00, 0x00, 0x00, 0xff }};
static const nes_color_t FOREGROUND = {{ 0x10, 0x10, 0x10, 0xff }};

static const nes_color_t PALETTE[] = {
        /* 0x00 */
        {{ 0x7c, 0x7c, 0x7c, 0xff }},
        {{ 0xfc, 0x00, 0x00, 0xff }},
        {{ 0xbc, 0x00, 0x00, 0xff }},
        {{ 0xbc, 0x28, 0x44, 0xff }},
        {{ 0x84, 0x00, 0x94, 0xff }},
        {{ 0x20, 0x00, 0xa8, 0xff }},
        {{ 0x00, 0x10, 0xa8, 0xff }},
        {{ 0x00, 0x14, 0x88, 0xff }},
        /* 0x08 */
        {{ 0x00, 0x30, 0x50, 0xff }},
        {{ 0x00, 0x78, 0x00, 0xff }},
        {{ 0x00, 0x68, 0x00, 0xff }},
        {{ 0x00, 0x58, 0x00, 0xff }},
        {{ 0x58, 0x40, 0x00, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        /* 0x10 */
        {{ 0xbc, 0xbc, 0xbc, 0xff }},
        {{ 0xf8, 0x78, 0x00, 0xff }},
        {{ 0xf8, 0x58, 0x00, 0xff }},
        {{ 0xfc, 0x44, 0x68, 0xff }},
        {{ 0xcc, 0x00, 0xd8, 0xff }},
        {{ 0x58, 0x00, 0xe4, 0xff }},
        {{ 0x00, 0x38, 0xf8, 0xff }},
        {{ 0x10, 0x5c, 0xe4, 0xff }},
        /* 0x18 */
        {{ 0x00, 0x7c, 0xac, 0xff }},
        {{ 0x00, 0xb8, 0x00, 0xff }},
        {{ 0x00, 0xa8, 0x00, 0xff }},
        {{ 0x44, 0xa8, 0x00, 0xff }},
        {{ 0x88, 0x88, 0x00, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        /* 0x20 */
        {{ 0xf8, 0xf8, 0xf8, 0xff }},
        {{ 0xfc, 0xbc, 0xf3, 0xff }},
        {{ 0xfc, 0x88, 0x68, 0xff }},
        {{ 0xf8, 0x78, 0x98, 0xff }},
        {{ 0xf8, 0x78, 0xf8, 0xff }},
        {{ 0x98, 0x58, 0xf8, 0xff }},
        {{ 0x58, 0x78, 0xf8, 0xff }},
        {{ 0x44, 0xa0, 0xfc, 0xff }},
        /* 0x28 */
        {{ 0x00, 0xb8, 0xf8, 0xff }},
        {{ 0x18, 0xf8, 0xb8, 0xff }},
        {{ 0x56, 0xd8, 0x58, 0xff }},
        {{ 0x98, 0xf8, 0x58, 0xff }},
        {{ 0xd8, 0xe8, 0x00, 0xff }},
        {{ 0x78, 0x78, 0x78, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        /* 0x30 */
        {{ 0xfc, 0xfc, 0xfc, 0xff }},
        {{ 0xfc, 0x4e, 0xa4, 0xff }},
        {{ 0xf8, 0xb8, 0xb8, 0xff }},
        {{ 0xf8, 0xb8, 0xd8, 0xff }},
        {{ 0xf8, 0xb8, 0xf8, 0xff }},
        {{ 0xc0, 0xa4, 0x8a, 0xff }},
        {{ 0xb0, 0xd0, 0xf0, 0xff }},
        {{ 0xa8, 0xe0, 0xce, 0xff }},
        /* 0x38 */
        {{ 0x78, 0xd8, 0xf8, 0xff }},
        {{ 0x78, 0xf8, 0xd8, 0xff }},
        {{ 0xb8, 0xf8, 0xb8, 0xff }},
        {{ 0xd8, 0xf8, 0xb8, 0xff }},
        {{ 0xfc, 0xfc, 0x00, 0xff }},
        {{ 0xf8, 0xd8, 0xf8, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        {{ 0x00, 0x00, 0x00, 0xff }},
        };

#define PALETTE_MAX 64

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int nes_service_clear(void);

int nes_service_fullscreen(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SDL */

#endif /* NES_SDL_TYPE_H_ */
