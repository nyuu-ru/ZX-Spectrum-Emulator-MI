/*
 * Device.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <cstdint>

class Device
{
public:
	virtual ~Device() = default;

	virtual void    write(uint32_t address, uint8_t data) = 0;
	virtual uint8_t read (uint32_t address) = 0;
};

#endif /* DEVICE_H_ */
