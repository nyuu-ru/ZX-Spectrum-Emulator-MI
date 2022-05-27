/*
 * EmulationProgramState.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include "EmulationProgramState.h"

EmulationProgramState::EmulationProgramState(SDL_Renderer * r)
{
	_emulator = std::make_shared<ZXEmulator>(r);
}

void EmulationProgramState::render(SDL_Renderer *r, int width, int height)
{
	_emulator->render();
}

std::chrono::microseconds EmulationProgramState::update_interval() const
{
	return std::chrono::microseconds(1'000'000 / 50);
}

void EmulationProgramState::update()
{
	if (not _emulator->initialized())
		return;

	for (int i = 0; i < VideoOutput::TOTAL_LINES; ++i) {
		_emulator->update_line();
	}
}

void EmulationProgramState::event(const SDL_Event &event)
{
	if (event.type == SDL_KEYDOWN)
		_emulator->key_press(event.key.keysym.scancode);
	if (event.type == SDL_KEYUP)
		_emulator->key_release(event.key.keysym.scancode);
}
