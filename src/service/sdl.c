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

#include "./sdl_type.h"

#ifdef SDL

static nes_sdl_t g_sdl = {};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
nes_service_clear(void)
{

	for(size_t y = 0; y < WINDOW_HEIGHT; ++y) {

		for(size_t x = 0; x < WINDOW_WIDTH; ++x) {
			nes_service_pixel(&FOREGROUND, x, y);
		}
	}

	return nes_service_show();
}

int
nes_service_fullscreen(void)
{
	int result = NES_OK;
	bool fullscreen = !g_sdl.fullscreen;

	if(g_sdl.window) {

		if(SDL_SetWindowFullscreen(g_sdl.window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)) {
			result = ERROR(NES_ERR, "sdl error -- %s", SDL_GetError());
			goto exit;
		}

		SDL_ShowCursor(fullscreen ? SDL_DISABLE : SDL_ENABLE);
	}

	g_sdl.fullscreen = fullscreen;
	TRACE(LEVEL_VERBOSE, "Service %s toggle", g_sdl.fullscreen ? "fullscreen" : "window");

exit:
	return result;
}

bool
nes_service_key(
	__in int key
	)
{
	return (SDL_GetKeyboardState(NULL)[key] > 0);
}

int
nes_service_load(
	__in const nes_t *configuration
	)
{
	int result = NES_OK;

	TRACE(LEVEL_VERBOSE, "%s", "Service loading");
	SDL_GetVersion(&g_sdl.version);
	TRACE(LEVEL_INFORMATION, "SDL ver.%i.%i.%i", g_sdl.version.major, g_sdl.version.minor, g_sdl.version.patch);

	snprintf(g_sdl.title, TITLE_MAX, "%s -- %s", basename((char *)configuration->path), NES);
	TRACE(LEVEL_VERBOSE, "Service title: \"%s\"", g_sdl.title);

	if(SDL_Init(SDL_INIT_VIDEO)) {
		result = ERROR(NES_ERR, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if(!(g_sdl.window = SDL_CreateWindow(g_sdl.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE, SDL_WINDOW_RESIZABLE))) {
		result = ERROR(NES_ERR, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if(!(g_sdl.renderer = SDL_CreateRenderer(g_sdl.window, -1, SDL_RENDERER_ACCELERATED))) {
		result = ERROR(NES_ERR, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if(SDL_RenderSetLogicalSize(g_sdl.renderer, WINDOW_WIDTH, WINDOW_HEIGHT)) {
		result = ERROR(NES_ERR, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if(SDL_SetRenderDrawColor(g_sdl.renderer, BACKGROUND.red, BACKGROUND.green, BACKGROUND.blue, BACKGROUND.alpha)) {
		result = ERROR(NES_ERR, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") == SDL_FALSE) {
		result = ERROR(NES_ERR, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if(!(g_sdl.texture = SDL_CreateTexture(g_sdl.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT))) {
		result = ERROR(NES_ERR, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if((result = nes_service_clear()) != NES_ERR) {
		goto exit;
	}

	TRACE(LEVEL_VERBOSE, "%s", "Service loaded");

exit:
	return result;
}

void
nes_service_pixel(
	__in const nes_color_t *color,
	__in uint32_t x,
	__in uint32_t y
	)
{
	g_sdl.pixel[y][x].raw = color->raw;
}

int
nes_service_poll(void)
{
	int result = NES_OK;
	SDL_Event event = {};

	while(SDL_PollEvent(&event)) {

		switch(event.type) {
			case SDL_KEYUP:

				if(!event.key.repeat) {

					switch(event.key.keysym.scancode) {
						case KEY_FULLSCREEN:

							if((result = nes_service_fullscreen()) != NES_OK) {
								goto exit;
							}
							break;
						default:
							break;
					}
				}
				break;
			case SDL_QUIT:
				TRACE(LEVEL_WARNING, "%s", "Service quit event");
				result = NES_EVT;
				goto exit;
			default:
				break;
		}
	}

exit:
	return result;
}

int
nes_service_show(void)
{
	uint32_t elapsed;
	int result = NES_OK;

	if(SDL_RenderClear(g_sdl.renderer)) {
		result = ERROR(NES_OK, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if(SDL_UpdateTexture(g_sdl.texture, NULL, (nes_color_t *)g_sdl.pixel, WINDOW_WIDTH * sizeof(nes_color_t))) {
		result = ERROR(NES_OK, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	if(SDL_RenderCopy(g_sdl.renderer, g_sdl.texture, NULL, NULL)) {
		result = ERROR(NES_OK, "sdl error -- %s", SDL_GetError());
		goto exit;
	}

	SDL_RenderPresent(g_sdl.renderer);

	if((elapsed = (SDL_GetTicks() - g_sdl.frame_begin)) < FRAME_FREQUENCY) {
		SDL_Delay(FRAME_FREQUENCY - elapsed);
	}

	g_sdl.frame_begin = SDL_GetTicks();

	if(g_sdl.frame >= FRAMES_PER_SEC) {
		g_sdl.framerate = (g_sdl.frame / ((SDL_GetTicks() - g_sdl.framerate_begin) / (float)MILLISEC_PER_SEC));
		g_sdl.framerate_begin = SDL_GetTicks();
		g_sdl.frame = 0;
		TRACE(LEVEL_INFORMATION, "Service framerate: %.2f", g_sdl.framerate);
#ifndef NDEBUG
		snprintf(g_sdl.format, sizeof(g_sdl.format), "%s [%.02f]", g_sdl.title, g_sdl.framerate);
		SDL_SetWindowTitle(g_sdl.window, g_sdl.format);
#endif /* NDEBUG */
	} else {
		++g_sdl.frame;
	}

exit:
	return result;
}

void nes_service_unload(void)
{
	TRACE(LEVEL_VERBOSE, "%s", "Service unloading");

	if(g_sdl.texture) {
		SDL_DestroyTexture(g_sdl.texture);
	}

	if(g_sdl.renderer) {
		SDL_DestroyRenderer(g_sdl.renderer);
	}

	if(g_sdl.window) {
		SDL_DestroyWindow(g_sdl.window);
	}

	SDL_Quit();
	memset(&g_sdl, 0, sizeof(g_sdl));
	TRACE(LEVEL_VERBOSE, "%s", "Service unloaded");
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SDL */
