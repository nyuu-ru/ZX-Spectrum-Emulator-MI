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

private:
	const BusInterface &_bus;
	std::shared_ptr<SDL_Texture> _tex;
	SDL_Renderer * _r;
public:
	VideoOutput(const BusInterface &b, SDL_Renderer * rr);
	~VideoOutput();

	void render();					// будет непосредственно рисовать
	void update_line(int line);		// будет обновлять 1 строку текстуры
	void update();					// будет обновлять _всю_ текстуру
};

#endif /* VIDEOOUTPUT_H_ */
