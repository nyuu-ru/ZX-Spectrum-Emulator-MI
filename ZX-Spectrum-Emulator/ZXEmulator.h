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

class ZXEmulator
{
protected:
	std::shared_ptr<BusInterface> _bus;
	// Z80CPU _cpu;
	// ...
	// TODO

public:
	ZXEmulator();
	virtual ~ZXEmulator() = default;
};

#endif /* ZXEMULATOR_H_ */
