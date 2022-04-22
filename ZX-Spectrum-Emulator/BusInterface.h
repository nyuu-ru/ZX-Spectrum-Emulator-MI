/*
 * BusInterface.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef BUSINTERFACE_H_
#define BUSINTERFACE_H_

#include "ROM.h"
#include "RAM.h"

class BusInterface
{
protected:
	ROM _rom;
	RAM _ram;
public:
	BusInterface(const char * rom_filename, size_t ram_size);
	virtual ~BusInterface() = default;

	virtual void mem_write(uint16_t address, uint8_t data) = 0;
	virtual uint8_t mem_read(uint16_t address) = 0;
	virtual void io_write(uint16_t address, uint8_t data) = 0;
	virtual uint8_t io_read(uint16_t address) = 0;

	RAM & ram() { return _ram; }
};

class BusInterface48k: public BusInterface
{
public:
	BusInterface48k(): BusInterface("seBasic321.rom", 65536) {}

	virtual uint8_t mem_read(uint16_t address) override;
	virtual uint8_t io_read(uint16_t address) override;
	virtual void io_write(uint16_t address, uint8_t data) override;
	virtual void mem_write(uint16_t address, uint8_t data) override;
};

#endif /* BUSINTERFACE_H_ */
