/*
 * BusInterface.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include "BusInterface.h"

uint8_t BusInterface48k::mem_read(uint16_t address)
{
	if (address < 16384)
		return _rom.read(address);
	else
		return _ram.read(address);
}

void BusInterface48k::mem_write(uint16_t address, uint8_t data)
{
	if (address >= 16384)
		_ram.write(address, data);
}

uint8_t BusInterface48k::io_read(uint16_t address)
{
	return 0; // TODO
}

void BusInterface48k::io_write(uint16_t address, uint8_t data)
{
	// TODO
}


BusInterface::BusInterface(const char *rom_filename, size_t ram_size)
: _rom(rom_filename), _ram(ram_size)
{
}
