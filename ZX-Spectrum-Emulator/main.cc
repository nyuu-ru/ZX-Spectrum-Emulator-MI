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


int main(int, char **)
{
	std::cout << "Program started." << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);

	Window w { 1920, 1080 };

	w.run();

	std::cout << "Program ended." << std::endl;
	return 0;
}



