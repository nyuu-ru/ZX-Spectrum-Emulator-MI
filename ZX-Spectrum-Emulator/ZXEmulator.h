/*
 * ZXEmulator.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef ZXEMULATOR_H_
#define ZXEMULATOR_H_

#include <memory>
#include "BusInterface.h"
#include "VideoOutput.h"

class ZXEmulator
{
protected:
	std::shared_ptr<BusInterface> _bus;
	std::shared_ptr<VideoOutput> _video;
	SDL_Renderer * _r;
	// Z80CPU _cpu;
	// ...
	// TODO

public:
	ZXEmulator(SDL_Renderer * r);
	virtual ~ZXEmulator() = default;

	void render();
};

#endif /* ZXEMULATOR_H_ */
