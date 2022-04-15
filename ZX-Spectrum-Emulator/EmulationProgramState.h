/*
 * EmulationProgramState.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef EMULATIONPROGRAMSTATE_H_
#define EMULATIONPROGRAMSTATE_H_

#include "ProgramState.h"

class EmulationProgramState final : public ProgramState
{
public:
	virtual ~EmulationProgramState() = default;

	virtual void render(SDL_Renderer *r, int width, int height) override;
};

#endif /* EMULATIONPROGRAMSTATE_H_ */
