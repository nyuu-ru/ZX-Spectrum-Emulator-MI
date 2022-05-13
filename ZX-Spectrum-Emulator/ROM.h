/*
 * ROM.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef ROM_H_
#define ROM_H_

#include <cstddef>
#include <vector>
#include "Device.h"

class ROM final : public Device
{
private:
	std::vector<uint8_t> _rom;
public:
	ROM(const char *filename);
	virtual ~ROM() = default;

	virtual uint8_t read(uint32_t address) const override
	{
		return _rom[address % _rom.size()];
	}
	virtual void write(uint32_t address, uint8_t data) override { }
};

#endif /* ROM_H_ */
