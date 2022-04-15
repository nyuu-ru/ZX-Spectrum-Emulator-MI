/*
 * Window.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include <string>
#include <stdexcept>
#include "Window.h"

void Window::create_window(int width, int height)
{
	_window = std::shared_ptr<SDL_Window>(
	        SDL_CreateWindow("ZX Spectrum Emulator :: MI-180x",
	                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                         width, height, SDL_WINDOW_RESIZABLE),
	        SDL_DestroyWindow);
	if (_window == nullptr)
		throw std::runtime_error(
		        std::string("Can't create a window: ") +
		        std::string(SDL_GetError()));
}

void Window::create_renderer()
{
	_renderer = std::shared_ptr<SDL_Renderer>(
	        SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
	        SDL_DestroyRenderer);
	if (_renderer == nullptr)
		throw std::runtime_error(
		        std::string("Can't create a renderer: ") +
		        std::string(SDL_GetError()));
}

Window::Window(int width, int height)
: _width(width), _height(height)
{
	create_window(width, height);
	create_renderer();
}

void Window::run()
{
	SDL_Event event;

	for (;;) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				return;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					SDL_GetWindowSize(_window.get(), &_width, &_height);
				}
				break;
			}
		}

		SDL_RenderClear(_renderer.get());
		SDL_RenderPresent(_renderer.get());
	}
}



