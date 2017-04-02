//Ethan Clark <ejc49@zips.uakron.edu>
//Nintendo Entertainment System representation

#ifndef NES_HPP
#define NES_HPP

#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "maps.h"

//Represents the NES hardware
struct sys {

	regC cu;					//Central Processing Unit
	regP pu;				//Picture Processing Unit
	APU au;					//Audio Processing Unit
	cart ridge;				//ROM file representation
	con one;				//Controller One
	con two;				//Controller Two
	maps<> mmc;				//Memory Mapper

	std::vector<w8> ram;	//RAM representation


	//Updates the clock rate of the system - CPU, PPU, and APU all run simulataneously in this implementation
	virtual void clock() {
		//Picture Processing Unit - Runs 3x as fast as CPU
		for (int i = 0; i < 3; ++i) pu.cyc();
		//Audio Processing Unit - Runs 1x as fast as CPU
		for (int i = 0; i < 1; ++i) au.cyc();
	}
};

#endif
