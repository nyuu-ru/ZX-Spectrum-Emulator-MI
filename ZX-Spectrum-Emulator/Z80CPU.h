/*
 * Z80CPU.h
 *
 *  Created on: 13 мая 2022 г.
 *      Author: unyuu
 */

#ifndef Z80CPU_H_
#define Z80CPU_H_

#include <cstdint>
#include <memory>
#include "BusInterface.h"

#define register
#include "Z80/Z80.h"

class ZXEmulator;

class Z80CPU final
{
private:
	std::shared_ptr<BusInterface> _bus;
	Z80 _cpu {};
	int _clock_diff { 0 };

	friend class ZXEmulator;


public:
	Z80CPU(std::shared_ptr<BusInterface> b);

	void clock(int clocks);
	void interrupt(uint8_t state);
	void reset();

	void mem_write(uint16_t address, uint8_t data)
	{
		_bus->mem_write(address, data);
	}

	uint8_t mem_read(uint16_t address) const
	{
		return _bus->mem_read(address);
	}

	void io_write(uint16_t address, uint8_t data)
	{
		_bus->io_write(address, data);
	}

	uint8_t io_read(uint16_t address) const
	{
		return _bus->io_read(address);
	}

};

#endif /* Z80CPU_H_ */
