/*
 * ROM.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include <stdexcept>
#include <fstream>
#include "ROM.h"

ROM::ROM(const char *filename)
{
	std::ifstream f { filename, std::ios::binary | std::ios::ate };
	if (not f.good())
		throw std::runtime_error(
				std::string("Can't open a ROM file ") +
				std::string(filename));
	_rom.resize(f.tellg());
	f.seekg(0);
	f.read(reinterpret_cast<char *>(&_rom[0]), _rom.size());
}
