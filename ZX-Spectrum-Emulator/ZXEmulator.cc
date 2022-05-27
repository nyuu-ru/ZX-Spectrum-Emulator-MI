/*
 * ZXEmulator.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include "ZXEmulator.h"

#include <vector>
#include <fstream>
#include <map>

struct KeyInfo { int row, col; };
static const std::map<SDL_Scancode, KeyInfo> s_keymap {
	{ SDL_SCANCODE_1, { 3, 0 }},
	{ SDL_SCANCODE_2, { 3, 1 }},
	{ SDL_SCANCODE_3, { 3, 2 }},
	{ SDL_SCANCODE_4, { 3, 3 }},
	{ SDL_SCANCODE_5, { 3, 4 }},

	{ SDL_SCANCODE_Q, { 2, 0 }},
	{ SDL_SCANCODE_W, { 2, 1 }},
	{ SDL_SCANCODE_E, { 2, 2 }},
	{ SDL_SCANCODE_R, { 2, 3 }},
	{ SDL_SCANCODE_T, { 2, 4 }},

	{ SDL_SCANCODE_A, { 1, 0 }},
	{ SDL_SCANCODE_S, { 1, 1 }},
	{ SDL_SCANCODE_D, { 1, 2 }},
	{ SDL_SCANCODE_F, { 1, 3 }},
	{ SDL_SCANCODE_G, { 1, 4 }},

	{ SDL_SCANCODE_0, { 4, 0 }},
	{ SDL_SCANCODE_9, { 4, 1 }},
	{ SDL_SCANCODE_8, { 4, 2 }},
	{ SDL_SCANCODE_7, { 4, 3 }},
	{ SDL_SCANCODE_6, { 4, 4 }},

	{ SDL_SCANCODE_P, { 5, 0 }},
	{ SDL_SCANCODE_O, { 5, 1 }},
	{ SDL_SCANCODE_I, { 5, 2 }},
	{ SDL_SCANCODE_U, { 5, 3 }},
	{ SDL_SCANCODE_Y, { 5, 4 }},

	{ SDL_SCANCODE_LSHIFT, { 0, 0 }},
	{ SDL_SCANCODE_Z, { 0, 1 }},
	{ SDL_SCANCODE_X, { 0, 2 }},
	{ SDL_SCANCODE_C, { 0, 3 }},
	{ SDL_SCANCODE_V, { 0, 4 }},

	{ SDL_SCANCODE_RETURN, { 6, 0 }},
	{ SDL_SCANCODE_L, { 6, 1 }},
	{ SDL_SCANCODE_K, { 6, 2 }},
	{ SDL_SCANCODE_J, { 6, 3 }},
	{ SDL_SCANCODE_H, { 6, 4 }},

	{ SDL_SCANCODE_SPACE, { 7, 0 }},
	{ SDL_SCANCODE_RSHIFT, { 7, 1 }},
	{ SDL_SCANCODE_M, { 7, 2 }},
	{ SDL_SCANCODE_N, { 7, 3 }},
	{ SDL_SCANCODE_B, { 7, 4 }},
};

ZXEmulator::ZXEmulator(SDL_Renderer *r)
: _r(r)
{
	_bus = std::make_shared<BusInterface48k>();
	_video = std::make_shared<VideoOutput>(_bus, r);
	_cpu = std::make_shared<Z80CPU>(_bus);

	_cpu->reset();
	_bus->io_write(0xfe, 0);
	_video->update();
	_initialized = true;
}

void ZXEmulator::render()
{
	_video->render();
}

void ZXEmulator::update_line()
{
	if (not initialized()) return;

	_cpu->clock(VideoOutput::SCREEN_WIDTH / 2);

	if (_current_line >= VideoOutput::INACTIVE_LINES) {
		_video->update_line(_current_line - VideoOutput::INACTIVE_LINES);
	}

	_current_line++;
	if (_current_line >= VideoOutput::TOTAL_LINES) {
		_current_line = 0;
		_cpu->interrupt(0);

		if (--_flash_counter == 0) {
			_flash_counter = FLASH_PERIOD;
			_video->flash();
		}
	}

}

void ZXEmulator::key_press(SDL_Scancode key)
{
	if (key == SDL_SCANCODE_F1)  load_sna48("sna/jetpac.sna");
	if (key == SDL_SCANCODE_F2)  load_sna48("sna/river.sna");

	if (key == SDL_SCANCODE_F6)  save_z80v1("default.z80");
	if (key == SDL_SCANCODE_F9)  load_z80v1("default.z80");
	if (key == SDL_SCANCODE_F12) { _cpu->reset(); return; }

	auto info = s_keymap.find(key);
	if (info == s_keymap.end()) return;

	int row = info->second.row;
	int col = info->second.col;
	_bus->key_press(row, col);
}

void ZXEmulator::key_release(SDL_Scancode key)
{
	auto info = s_keymap.find(key);
	if (info == s_keymap.end()) return;

	int row = info->second.row;
	int col = info->second.col;
	_bus->key_release(row, col);
}

#pragma pack(push, 1)
struct SNAHeader
{
	uint8_t  I;
	uint16_t HL1, DE1, BC1, AF1;
	uint16_t HL, DE, BC, IY, IX;
	uint8_t  IFF2;
	uint8_t  R;
	uint16_t AF, SP;
	uint8_t  IM;
	uint8_t  BDR;
};
#pragma pack(pop)

void ZXEmulator::save_sna48(const char *filename)
{
	SNAHeader header {};
	std::vector<uint8_t> image(65536);
	for (unsigned i = 16384; i < 65536; ++i)
		image[i] = _bus->mem_read(i);
	header.I    = _cpu->_cpu.I;
	header.HL1  = _cpu->_cpu.HL1.W;
	header.DE1  = _cpu->_cpu.DE1.W;
	header.BC1  = _cpu->_cpu.BC1.W;
	header.AF1  = _cpu->_cpu.AF1.W;
	header.HL   = _cpu->_cpu.HL.W;
	header.DE   = _cpu->_cpu.DE.W;
	header.BC   = _cpu->_cpu.BC.W;
	header.IY   = _cpu->_cpu.IY.W;
	header.IX   = _cpu->_cpu.IX.W;
	header.IFF2 = !!(_cpu->_cpu.IFF & IFF_2) << 2;
	header.R    = _cpu->_cpu.R;
	header.AF   = _cpu->_cpu.AF.W;
	header.SP   = _cpu->_cpu.SP.W;
	if (_cpu->_cpu.IFF & IFF_IM2)      // 0x04 = 0b100
		header.IM = 2;
	else if (_cpu->_cpu.IFF & IFF_IM1) // 0x02 = 0b010
		header.IM = 1;
	else
		header.IM = 0;
	header.BDR  = _bus->border();
	header.SP -= 2;
	image[header.SP]   = _cpu->_cpu.PC.B.l;
	image[header.SP+1] = _cpu->_cpu.PC.B.h;

	std::ofstream file { filename, std::ios::binary };
	file.write(reinterpret_cast<char*>(&header), sizeof(header));
	file.write(reinterpret_cast<char*>(&image[16384]), 48*1024);
	file.close();

	/* Помещение значения на стек:
	 * SP -= размер
	 * память[SP] = значение
	 *
	 * Извлечение со стека:
	 * приёмник = память[SP]
	 * SP += размер
	 *
	 */
}

void ZXEmulator::load_sna48(const char *filename)
{
	SNAHeader header {};

	std::ifstream file { filename, std::ios::binary };
	if (not file.good()) return; // TODO: обругаться

	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	for (unsigned i = 16384; i < 65536; ++i) {
		uint8_t byte = file.get();
		_bus->mem_write(i, byte);
	}

	_cpu->_cpu.I     = header.I;
	_cpu->_cpu.HL1.W = header.HL1;
	_cpu->_cpu.DE1.W = header.DE1;
	_cpu->_cpu.BC1.W = header.BC1;
	_cpu->_cpu.AF1.W = header.AF1;
	_cpu->_cpu.HL.W  = header.HL;
	_cpu->_cpu.DE.W  = header.DE;
	_cpu->_cpu.BC.W  = header.BC;
	_cpu->_cpu.IY.W  = header.IY;
	_cpu->_cpu.IX.W  = header.IX;
	_cpu->_cpu.R     = header.R;
	_cpu->_cpu.AF.W  = header.AF;
	_cpu->_cpu.SP.W  = header.SP;
	_cpu->_cpu.IFF   = 0;
	if (header.IFF2 & 0x04) _cpu->_cpu.IFF |= IFF_2;
	if (header.IM == 1)     _cpu->_cpu.IFF |= IFF_IM1;
	if (header.IM == 2)     _cpu->_cpu.IFF |= IFF_IM2;
	_bus->io_write(0xfe, header.BDR);

	_cpu->_cpu.PC.B.l = _bus->mem_read(_cpu->_cpu.SP.W);
	_cpu->_cpu.PC.B.h = _bus->mem_read(_cpu->_cpu.SP.W + 1);
	_cpu->_cpu.SP.W += 2;
}

#pragma pack(push, 1)
struct Z80Header
{
	uint8_t  A, F;
	uint16_t BC, HL, PC, SP;
	uint8_t  I, R;
	uint8_t  options;
	uint16_t DE, BC1, DE1, HL1;
	uint8_t  A1, F1;
	uint16_t IY, IX;
	uint8_t  IF, IFF2;
	uint8_t  mode;
};
#pragma pack(pop)

void ZXEmulator::save_z80v1(const char *filename)
{
	Z80Header header {};
	header.A   = _cpu->_cpu.AF.B.h;
	header.F   = _cpu->_cpu.AF.B.l;
	header.BC  = _cpu->_cpu.BC.W;
	header.HL  = _cpu->_cpu.HL.W;
	header.PC  = _cpu->_cpu.PC.W;
	header.SP  = _cpu->_cpu.SP.W;
	header.I   = _cpu->_cpu.I;
	header.R   = _cpu->_cpu.R & 0x7f;
	header.DE  = _cpu->_cpu.DE.W;
	header.BC1 = _cpu->_cpu.BC1.W;
	header.DE1 = _cpu->_cpu.DE1.W;
	header.HL1 = _cpu->_cpu.HL1.W;
	header.A1  = _cpu->_cpu.AF1.B.h;
	header.F1  = _cpu->_cpu.AF1.B.l;
	header.IY  = _cpu->_cpu.IY.W;
	header.IX  = _cpu->_cpu.IX.W;
	header.IF  = !!(_cpu->_cpu.IFF & IFF_EI);
	header.IFF2= !!(_cpu->_cpu.IFF & IFF_2);
	header.options =
			(_cpu->_cpu.R >> 7)   |
			(_bus->border() << 1);
	header.mode =
			((_cpu->_cpu.IFF >> 1) & 0x03);
	std::ofstream file { filename, std::ios::binary };
	file.write(reinterpret_cast<char*>(&header), sizeof(header));
	for (unsigned i = 16384; i < 65536; ++i)
		file.put(_bus->mem_read(i));
	file.close();
}

void ZXEmulator::load_z80v1(const char *filename)
{
	Z80Header header {};
	std::ifstream file { filename, std::ios::binary };
	if (not file.good()) return; // TODO: обругаться

	file.read(reinterpret_cast<char*>(&header), sizeof(header));

	_cpu->_cpu.AF.B.h = header.A;
	_cpu->_cpu.AF.B.l = header.F;
	_cpu->_cpu.BC.W   = header.BC;
	_cpu->_cpu.HL.W   = header.HL;
	_cpu->_cpu.PC.W   = header.PC;
	_cpu->_cpu.SP.W   = header.SP;
	_cpu->_cpu.I      = header.I;
	_cpu->_cpu.R      = header.R | ((header.options & 1) << 7);
	_cpu->_cpu.DE.W   = header.DE;
	_cpu->_cpu.BC1.W  = header.BC1;
	_cpu->_cpu.DE1.W  = header.DE1;
	_cpu->_cpu.HL1.W  = header.HL1;
	_cpu->_cpu.AF1.B.h= header.A1;
	_cpu->_cpu.AF1.B.l= header.F1;
	_cpu->_cpu.IY.W   = header.IY;
	_cpu->_cpu.IX.W   = header.IX;
	_cpu->_cpu.IFF    = 0;
	if (header.IF)   _cpu->_cpu.IFF |= IFF_EI;
	if (header.IFF2) _cpu->_cpu.IFF |= IFF_2;
	_cpu->_cpu.IFF |= (header.mode & 0x03) << 1;
	_bus->io_write(0xfe, (header.options >> 1) & 0x07);
	if ((header.options & (1 << 5)) == 0) {
		for (unsigned i = 16384; i < 65536; ++i) {
			uint8_t byte = file.get();
			_bus->mem_write(i, byte);
		}
	} else {
		// TODO: реализовать RLE
	}
}
