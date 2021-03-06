/*
 * EmulationProgramState.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef EMULATIONPROGRAMSTATE_H_
#define EMULATIONPROGRAMSTATE_H_

#include "ProgramState.h"
#include "ZXEmulator.h"

class EmulationProgramState final : public ProgramState
{
private:
	std::shared_ptr<ZXEmulator> _emulator;

public:
	EmulationProgramState(SDL_Renderer * r);
	virtual ~EmulationProgramState() = default;

	virtual void render(SDL_Renderer *r, int width, int height) override;

	virtual std::chrono::microseconds update_interval() const override;
	virtual void update() override;
	virtual void event(const SDL_Event &event) override;
};

#endif /* EMULATIONPROGRAMSTATE_H_ */
