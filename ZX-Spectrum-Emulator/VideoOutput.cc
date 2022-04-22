/*
 * VideoOutput.cc
 *
 *  Created on: 22 апр. 2022 г.
 *      Author: unyuu
 */

#include "VideoOutput.h"
#include <iostream>

static const uint32_t COLOURS [16] {
	0xff000000,		// 0000	// I G R B
	0xff7f0000,		// 0001
	0xff00007f,		// 0010
	0xff7f007f,		// 0011
	0xff007f00,		// 0100
	0xff7f7f00,		// 0101
	0xff007f7f,		// 0110
	0xff7f7f7f,		// 0111
	0xff000000,		// 1000
	0xffff3f3f,		// 1001
	0xff3f3fff,		// 1010
	0xffff3fff,		// 1011
	0xff3fff3f,		// 1100
	0xffffff3f,		// 1101
	0xff3fffff,		// 1110
	0xffffffff,		// 1111
};


VideoOutput::VideoOutput(const BusInterface &b, SDL_Renderer * rr)
: _bus(b), _r(rr)
{
	_tex = std::shared_ptr<SDL_Texture>(
			SDL_CreateTexture(_r, SDL_PIXELFORMAT_ABGR8888,
			                  SDL_TEXTUREACCESS_STREAMING,
			                  SCREEN_WIDTH, SCREEN_HEIGHT),
			SDL_DestroyTexture);
}

VideoOutput::~VideoOutput()
{
}

void VideoOutput::render()
{
	SDL_RenderCopy(_r, _tex.get(), nullptr, nullptr);
}

void VideoOutput::update_line(int line)
{
	uint32_t border_colour = COLOURS[2];


	uint32_t * pixels;
	int pitch;
	SDL_Rect line_rect { 0, line, SCREEN_WIDTH, 1 };
	SDL_LockTexture(_tex.get(), &line_rect,
	                reinterpret_cast<void**>(&pixels),
	                &pitch);

	if (line < BORDER_TOP or line >= BORDER_TOP + PAPER_HEIGHT) {
		for (int i = 0; i < SCREEN_WIDTH; ++i)
			pixels[i] = border_colour;
	} else {
		int col = 0;
		for (; col < BORDER_LEFT; ++col)
			pixels[col] = border_colour;
		for (; col < BORDER_LEFT + PAPER_WIDTH; ++col) {
			uint32_t paper_colour = COLOURS[9];
			uint32_t ink_colour = COLOURS[13];
			// TODO: вывод изображения
			if ((col ^ line)&1)
				pixels[col] = ink_colour;
			else
				pixels[col] = paper_colour;
		}
		for (; col < SCREEN_WIDTH; ++col) {
			pixels[col] = border_colour;
		}
	}

	SDL_UnlockTexture(_tex.get());

}

void VideoOutput::update()
{
	for (int l = 0; l < SCREEN_HEIGHT; ++l)
		update_line(l);
}
