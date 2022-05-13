/*
 * Window.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <memory>
#include <mutex>
#include <SDL2/SDL.h>
#include "StateMachine.h"

class Window
{
private:
	void create_window(int width, int height);
	void create_renderer();

protected:
	std::shared_ptr<SDL_Window> _window {};
	std::shared_ptr<SDL_Renderer> _renderer {};
	std::mutex _render_mutex {};

	int _width, _height;

public:
	Window(int width, int height);
	virtual ~Window() = default;

	void run();

	SDL_Renderer * renderer() { return _renderer.get(); }
};

#endif /* WINDOW_H_ */
