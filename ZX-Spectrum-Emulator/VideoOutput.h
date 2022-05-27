/*
 * VideoOutput.h
 *
 *  Created on: 22 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef VIDEOOUTPUT_H_
#define VIDEOOUTPUT_H_

#include <memory>
#include <SDL2/SDL.h>
#include "RAM.h"
#include "BusInterface.h"

class VideoOutput final
{
public:
	static constexpr int BORDER_LEFT   = 48;
	static constexpr int BORDER_RIGHT  = 48;
	static constexpr int BORDER_TOP    = 48;
	static constexpr int BORDER_BOTTOM = 56;
	static constexpr int PAPER_WIDTH   = 256;
	static constexpr int PAPER_HEIGHT  = 192;
	static constexpr int SCREEN_WIDTH  =
			BORDER_LEFT + PAPER_WIDTH  + BORDER_RIGHT;
	static constexpr int SCREEN_HEIGHT =
			BORDER_TOP  + PAPER_HEIGHT + BORDER_BOTTOM;
	static constexpr int INACTIVE_LINES = 16;
	static constexpr int INACTIVE_COLS  = 96;
	static constexpr int TOTAL_LINES =
			SCREEN_HEIGHT + INACTIVE_LINES;
	static constexpr int TOTAL_COLS =
			SCREEN_WIDTH + INACTIVE_COLS;


private:
	std::shared_ptr<BusInterface> _bus;
	std::shared_ptr<SDL_Texture> _tex;
	std::shared_ptr<SDL_Surface> _screen;
	SDL_Renderer * _r;
	bool _flash_state { false };

public:
	VideoOutput(std::shared_ptr<BusInterface> b, SDL_Renderer * rr);
	~VideoOutput();

	void render();					// будет непосредственно рисовать
	void update_line(int line);		// будет обновлять 1 строку текстуры
	void update();					// будет обновлять _всю_ текстуру

	void flash() { _flash_state = not _flash_state; }
};

#endif /* VIDEOOUTPUT_H_ */
