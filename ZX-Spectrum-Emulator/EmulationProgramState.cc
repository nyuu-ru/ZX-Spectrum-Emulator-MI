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
