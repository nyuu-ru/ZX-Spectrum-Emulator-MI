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
	0xffbf0000,		// 0001
	0xff0000bf,		// 0010
	0xffbf00bf,		// 0011
	0xff00bf00,		// 0100
	0xffbfbf00,		// 0101
	0xff00bfbf,		// 0110
	0xffbfbfbf,		// 0111
	0xff000000,		// 1000
	0xffff3f3f,		// 1001
	0xff3f3fff,		// 1010
	0xffff3fff,		// 1011
	0xff3fff3f,		// 1100
	0xffffff3f,		// 1101
	0xff3fffff,		// 1110
	0xffffffff,		// 1111
};


VideoOutput::VideoOutput(std::shared_ptr<BusInterface> b, SDL_Renderer * rr)
: _bus(b), _r(rr)
{
	_screen = std::shared_ptr<SDL_Surface>(
			SDL_CreateRGBSurface(
					0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
					0x000000ff,
					0x0000ff00,
					0x00ff0000,
					0xff000000),
			SDL_FreeSurface);
}

VideoOutput::~VideoOutput()
{
}

void VideoOutput::render()
{
	_tex = std::shared_ptr<SDL_Texture>(
			SDL_CreateTextureFromSurface(_r, _screen.get()),
			SDL_DestroyTexture);
	SDL_RenderCopy(_r, _tex.get(), nullptr, nullptr);
}

void VideoOutput::update_line(int line)
{
	uint32_t border_colour = COLOURS[_bus->border()];

	uint32_t * pixels = reinterpret_cast<uint32_t*>(
			_screen->pixels) + SCREEN_WIDTH * line;

	if (line < BORDER_TOP or line >= BORDER_TOP + PAPER_HEIGHT) {
		for (int i = 0; i < SCREEN_WIDTH; ++i)
			pixels[i] = border_colour;
	} else {
		int col = 0;
		for (; col < BORDER_LEFT; ++col)
			pixels[col] = border_colour;
		for (; col < BORDER_LEFT + PAPER_WIDTH; col += 8) {
			int y = line - BORDER_TOP;  // Номер строки __изображения__
			int x = col  - BORDER_LEFT; // Номер колонки __изображения__
			int xblock = x >> 3;        // Номер знакоместа в строке

			//	VA: 13  12  11  10   9   8   7   6   5   4   3   2   1   0
			//   P:  0 C12 C11  C7  C6  C5 C10  C9  C8  C4  C3  C2  C1  C0
			//   A:  0   1   1   0 C12 C11 C10  C9  C8  C4  C3  C2  C1  C0

			uint16_t block = y * 32 + xblock;
			uint16_t c_4_0   = (block & 0b0000'0000'0001'1111);      // PA
			uint16_t c_7_5   = (block & 0b0000'0000'1110'0000) << 3; // P
			uint16_t c_10_8  = (block & 0b0000'0111'0000'0000) >> 3; // P
			uint16_t c_12_8  = (block & 0b0001'1111'0000'0000) >> 3; // A
			uint16_t c_12_11 = (block & 0b0001'1000'0000'0000);      // P
			uint16_t a_12_10 =          0b0001'1000'0000'0000;       // A

			uint16_t pixel_address = c_4_0 | c_10_8 | c_7_5 | c_12_11;
			uint16_t attr_address  = c_4_0 | c_12_8 | a_12_10;

			uint8_t pix    = _bus->vmem_read(pixel_address);
			uint8_t attr   = _bus->vmem_read(attr_address);

			uint8_t ink    =   (attr & 0b0000'0111);
			uint8_t paper  =   (attr & 0b0011'1000) >> 3;
			uint8_t bright = !!(attr & 0b0100'0000);
			uint8_t flash  = !!(attr & 0b1000'0000);

			if (bright) { ink += 8; paper += 8; }
			if (flash and _flash_state) pix = ~pix;

			uint32_t paper_colour = COLOURS[paper];
			uint32_t ink_colour   = COLOURS[ink];

			for (int p = 0; p < 8; ++p) {
				pixels[col + 7 - p] = (pix & 1) ? ink_colour : paper_colour;
				pix >>= 1;
			}

		}
		for (; col < SCREEN_WIDTH; ++col) {
			pixels[col] = border_colour;
		}
	}
}

void VideoOutput::update()
{
	for (int l = 0; l < SCREEN_HEIGHT; ++l)
		update_line(l);
}
