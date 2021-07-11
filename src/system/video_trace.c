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

#include "./video_trace_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef NDEBUG

static const char *INCREMENT_FORMAT[] = {
        "Across", /* VIDEO_INCREMENT_ACROSS */
        "Down", /* VIDEO_INCREMENT_DOWN */
        };

static const char *NAME_TABLE_FORMAT[] = {
        "2000-23FF", /* VIDEO_NAME_TABLE_2000 */
        "2400-27FF", /* VIDEO_NAME_TABLE_2400 */
        "2800-2BFF", /* VIDEO_NAME_TABLE_2800 */
        "2C00-2FFF", /* VIDEO_NAME_TABLE_2C00 */
        };

static const char *PATTERN_TABLE_FORMAT[] = {
        "0000-0FFF", /* VIDEO_PATTERN_TABLE_0000 */
        "1000-1FFF", /* VIDEO_PATTERN_TABLE_1000 */
        };

static const char *SPRITE_SIZE_FORMAT[] = {
        "8x8", /* VIDEO_SPRITE_SIZE_8_8 */
        "8x16", /* VIDEO_SPRITE_SIZE_8_16 */
        };

void
nes_video_trace(
        __in int level,
        __in const nes_video_t *video
        )
{

        if(level <= LEVEL) {
                TRACE(level, "Video cycles: %u", video->cycles);
                TRACE(level, "Video CTRL: %02X [%s, %s, %s, %s, %s, %s]", video->control.raw, NAME_TABLE_FORMAT[video->control.name_table],
                        INCREMENT_FORMAT[video->control.increment], PATTERN_TABLE_FORMAT[video->control.sprite_pattern_table],
                        PATTERN_TABLE_FORMAT[video->control.background_pattern_table], SPRITE_SIZE_FORMAT[video->control.sprite_size],
                        video->control.interrupt ? "Vblank" : "None");
                TRACE(level, "Video MASK: %02X [%c%c%c%c%c%c%c%c]", video->mask.raw, video->mask.blue_emphasis ? 'B' : '-', video->mask.green_emphasis ? 'G' : '-',
                        video->mask.red_emphasis ? 'R' : '-', video->mask.sprite_show ? 'S' : '-', video->mask.background_show ? 'B' : '-',
                        video->mask.sprite_show_top ? 'S' : '-', video->mask.background_show_top ? 'B' : '-', video->mask.grayscale ? 'G' : '-');
                TRACE(level, "Video STAT: %02X [%c%c%c]", video->status.raw, video->status.vblank ? 'V' : '-', video->status.sprite_0_hit ? 'H' : '-',
                        video->status.sprite_overflow ? 'O' : '-');
                TRACE(level, "Video ADDR: %04X", video->address.word);
                TRACE(level, "Video DATA: %02X", video->data.low);
                TRACE(level, "Video OBJ-ADDR: %02X", video->object_address.low);
                TRACE(level, "Video OBJ-DATA: %02X", video->object_data.low);
                TRACE(level, "Video SCR-X: %02X, %02X", video->scroll_x);
                TRACE(level, "Video SCR-Y: %02X, %02X", video->scroll_y);
        }
}

#endif /* NDEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */
