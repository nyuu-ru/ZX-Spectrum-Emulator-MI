/*
 * ProgramState.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef PROGRAMSTATE_H_
#define PROGRAMSTATE_H_

#include <chrono>
#include <SDL2/SDL.h>

#include <iostream>

class ProgramState
{
public:
	virtual ~ProgramState() = default;

	virtual void initialize() {}
	virtual void finalize()   {}
	virtual void enter()      {}
	virtual void leave()      {}

	virtual void render(SDL_Renderer * r, int width, int height) {}
	virtual void event(const SDL_Event &event) {}
	virtual void update() {}

	virtual std::chrono::microseconds update_interval() const
	{
		return std::chrono::microseconds(1000000 / 60);
	}
};

class TestState: public ProgramState
{
public:
	virtual void render(SDL_Renderer *r, int width, int height) override
	{
		SDL_SetRenderDrawColor(r, 16, 48, 128, 255);
		SDL_RenderClear(r);
	}

	virtual void update()
	{
		std::cout << "Meow!" << std::endl;
	}
};

#endif /* PROGRAMSTATE_H_ */
