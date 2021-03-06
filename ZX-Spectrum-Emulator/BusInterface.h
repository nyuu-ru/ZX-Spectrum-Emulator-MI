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
#include "PortFE.h"
#include "Keyboard.h"

class BusInterface
{
protected:
	ROM _rom;
	RAM _ram;
	PortFE _port_fe;
	Keyboard _keyboard;

public:
	BusInterface(const char * rom_filename, size_t ram_size);
	virtual ~BusInterface() = default;

	virtual void mem_write(uint16_t address, uint8_t data) = 0;
	virtual uint8_t mem_read(uint16_t address) const = 0;
	virtual void io_write(uint16_t address, uint8_t data) = 0;
	virtual uint8_t io_read(uint16_t address) const = 0;
	virtual uint8_t vmem_read(uint16_t offset) const = 0;

	RAM & ram() { return _ram; }
	uint8_t border() const { return _port_fe.border(); }

	void key_press(int row, int col)
	{
		_keyboard.key_press(row, col);
	}

	void key_release(int row, int col)
	{
		_keyboard.key_release(row, col);
	}
};

class BusInterface48k: public BusInterface
{
public:
	BusInterface48k(): BusInterface("seBasic321.rom", 65536) {}

	virtual uint8_t mem_read(uint16_t address) const override;
	virtual uint8_t io_read(uint16_t address) const override;
	virtual void io_write(uint16_t address, uint8_t data) override;
	virtual void mem_write(uint16_t address, uint8_t data) override;

	virtual uint8_t vmem_read(uint16_t offset) const
	{
		return mem_read(16384 + offset);
	}
};

#endif /* BUSINTERFACE_H_ */
