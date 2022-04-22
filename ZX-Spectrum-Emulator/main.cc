/*
 * main.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "Window.h"

#include "StateMachine.h"
#include "EmulationProgramState.h"

int main(int, char **)
{
	std::cout << "Program started." << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);

	Window w { 1920, 1080 };

	StateMachine::enter_state(
			std::make_shared<EmulationProgramState>(
					w.renderer()));
	w.run();
	StateMachine::leave_state();

	std::cout << "Program ended." << std::endl;
	return 0;
}



