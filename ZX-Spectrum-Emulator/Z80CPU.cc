/*
 * Z80CPU.cc
 *
 *  Created on: 13 мая 2022 г.
 *      Author: unyuu
 */

#include "Z80CPU.h"

// FIXME: надо всё-таки сделать по-человечески...
// (чтоб процессоры различать, если их несколько)
static Z80CPU * s_cpu = nullptr;

extern "C" {
void WrZ80(register word Addr,register byte Value)
{
	s_cpu->mem_write(Addr, Value);
}

byte RdZ80(register word Addr)
{
	return s_cpu->mem_read(Addr);
}

void OutZ80(register word Port,register byte Value)
{
	s_cpu->io_write(Port, Value);
}

byte InZ80(register word Port)
{
	return s_cpu->io_read(Port);
}

void PatchZ80(register Z80 *R)
{

}
} /* extern "C" */

Z80CPU::Z80CPU(std::shared_ptr<BusInterface> b)
: _bus(b)
{
	s_cpu = this;
}

void Z80CPU::clock(int clocks)
{
	_clock_diff = ExecZ80(&_cpu, clocks + _clock_diff);
}

void Z80CPU::interrupt(uint8_t state)
{
	IntZ80(&_cpu, 0);
}

void Z80CPU::reset()
{
	ResetZ80(&_cpu);
}
