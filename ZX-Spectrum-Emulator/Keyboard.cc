/*
 * Keyboard.cc
 *
 *  Created on: 13 мая 2022 г.
 *      Author: unyuu
 */

#include "Keyboard.h"

void Keyboard::key_press(int row, int col)
{
        _key_matrix[row] &= ~(1 << col);
}

void Keyboard::key_release(int row, int col)
{
        _key_matrix[row] |= (1 << col);
}

uint8_t Keyboard::read(uint8_t mask) const
{
        uint8_t result = 0xff;
        for (unsigned i = 0; i < 8; ++i) {
                if ((mask & (1 << i)) == 0) {
                        result &= _key_matrix[i];
                }
        }
        return result;
}
