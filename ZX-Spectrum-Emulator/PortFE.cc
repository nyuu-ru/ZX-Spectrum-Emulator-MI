/*
 * PortFE.cc
 *
 *  Created on: 13 мая 2022 г.
 *      Author: unyuu
 */

#include "PortFE.h"

uint8_t PortFE::read(uint32_t address) const
{
	return 0; // TODO: реализовать этот порт
}

void PortFE::write(uint32_t address, uint8_t data)
{
	// TODO: реализовать запись на ленту и вывод звука на бипер
	_border = data & 0b00000111; // младшие 3 бита - цвет бордюра.
}
