/*
 * ZXEmulator.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include "ZXEmulator.h"

ZXEmulator::ZXEmulator(SDL_Renderer *r)
: _r(r)
{
	_bus = std::make_shared<BusInterface48k>();
	_video = std::make_shared<VideoOutput>(*_bus, r);
	_video->update();
}

void ZXEmulator::render()
{
	_video->render();
}
