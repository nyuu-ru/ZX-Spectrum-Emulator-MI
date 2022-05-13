/*
 * PortFE.h
 *
 *  Created on: 13 мая 2022 г.
 *      Author: unyuu
 */

#ifndef PORTFE_H_
#define PORTFE_H_

#include "Device.h"

class PortFE final : public Device
{
private:
	uint8_t _border { 0 };

public:
	virtual uint8_t read(uint32_t address) const override;
	virtual void write(uint32_t address, uint8_t data) override;

	uint8_t border() const { return _border; }
};

#endif /* PORTFE_H_ */
