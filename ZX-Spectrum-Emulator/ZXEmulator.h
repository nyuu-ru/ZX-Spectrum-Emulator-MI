/*
 * ZXEmulator.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef ZXEMULATOR_H_
#define ZXEMULATOR_H_

#include <memory>
#include "BusInterface.h"
#include "VideoOutput.h"
#include "Z80CPU.h"

class ZXEmulator
{
protected:
	std::shared_ptr<BusInterface> _bus;
	std::shared_ptr<VideoOutput>  _video;
	std::shared_ptr<Z80CPU>       _cpu;
	SDL_Renderer * _r;
	int _current_line { 0 };
	bool _initialized { false };
	// ...
	// TODO

	static constexpr int FLASH_PERIOD = 20;
	int _flash_counter { FLASH_PERIOD };

public:
	ZXEmulator(SDL_Renderer * r);
	virtual ~ZXEmulator() = default;

	void render();
	void update_line();

	bool initialized() const { return _initialized; }

	void key_press(SDL_Scancode key);
	void key_release(SDL_Scancode key);

	void save_sna48(const char * filename);
	void load_sna48(const char * filename);

	void save_z80v1(const char * filename);
	void load_z80v1(const char * filename);
};

#endif /* ZXEMULATOR_H_ */
