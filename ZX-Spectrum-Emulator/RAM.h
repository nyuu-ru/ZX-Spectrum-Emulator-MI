/*
 * RAM.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef RAM_H_
#define RAM_H_

#include <cstddef>
#include <vector>
#include "Device.h"


class RAM final : public Device
{
private:
	std::vector<uint8_t> _ram;
public:
	RAM(size_t size): _ram(size) {}
	virtual ~RAM() = default;

	virtual uint8_t read(uint32_t address) const override
	{
		return _ram[address % _ram.size()];
	}

	virtual void write(uint32_t address, uint8_t data) override
	{
		_ram[address % _ram.size()] = data;
	}
};

#endif /* RAM_H_ */
