/*
 * Keyboard.h
 *
 *  Created on: 13 мая 2022 г.
 *      Author: unyuu
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <cstdint>

class Keyboard final
{
private:
	uint8_t _key_matrix[8] { 255, 255, 255, 255, 255, 255, 255, 255 };

public:
	void key_press(int row, int col);
	void key_release(int row, int col);
	uint8_t read(uint8_t mask) const;
};

#endif /* KEYBOARD_H_ */
