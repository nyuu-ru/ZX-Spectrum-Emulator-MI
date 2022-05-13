/*
 * ZXEmulator.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include "ZXEmulator.h"

#include <fstream>

ZXEmulator::ZXEmulator(SDL_Renderer *r)
: _r(r)
{
	_bus = std::make_shared<BusInterface48k>();
	_video = std::make_shared<VideoOutput>(_bus, r);
	_cpu = std::make_shared<Z80CPU>(_bus);

	_cpu->reset();
	_bus->io_write(0xfe, 0);
	_video->update();
	_initialized = true;
}

void ZXEmulator::render()
{
	_video->render();
}

void ZXEmulator::update_line()
{
	if (not initialized()) return;

	_cpu->clock(VideoOutput::SCREEN_WIDTH / 2);

	if (_current_line >= VideoOutput::INACTIVE_LINES) {
		_video->update_line(_current_line - VideoOutput::INACTIVE_LINES);
	}

	_current_line++;
	if (_current_line >= VideoOutput::TOTAL_LINES) {
		_current_line = 0;
		_cpu->interrupt(0);
	}

}
