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
	
	CPU * cu;					//Central Processing Unit
	PPU * pu;					//Picture Processing Unit
	APU * au;					//Audio Processing Unit
	cart * ridge;				//ROM file representation
	con * one;					//Controller One
	con * two;					//Controller Two
	maps * mmc;					//Memory Mapper
	sys * state;				//Current state of the system
	std::vector<w8> ram;		//RAM representation


	//Constructors & Destructor

	sys() {
		ram.resize(0x800);
		ridge = new cart();
		mmc = new maps(*ridge);
		one = new con();
		two = new con();
		state = new sys{ ridge, one, two, mmc, ram };
		cu = new CPU(state);
		pu = new PPU(state);
		//au new APU(state);		Not included at this time
	}

	sys(cart * a, con * b, con * c, maps * d, std::vector<w8> e)
		:ridge(a), one(b), two(c), mmc(d), ram(e) { }

	~sys() {
		delete ridge;
		delete mmc;
		delete one;
		delete two;
		delete state;
		delete cu;
		delete pu;
		//delete au		(if I have new)
	}


	//Updates the clock rate of the system - CPU, PPU, and APU all run simulataneously in this implementation
	void clock(int rateCPU, int ratePPU) {
		//Picture Processing Unit - Runs 3x as fast as CPU
		for (int i = 0; i < ratePPU; ++i) this->pu->execute();
		//Audio Processing Unit - Runs 1x as fast as CPU
		for (int i = 0; i < rateCPU; ++i) this->au->execute();
	}


	//Executes all processors
	int execute() {
		int rateCPU, ratePPU;
		rateCPU = this->cu->execute();
		ratePPU = 3 * rateCPU;				//Always three times faster
		this->clock(rateCPU, ratePPU);
		return rateCPU;
	}


	//Set player one's controller to buttons
	void pOne(bool buttons[8]) { this->one->set(buttons); }

	//Set player two's controller to buttons
	void pTwo(bool buttons[8]) { this->two->set(buttons); }


	//PPU Frame Clock - helps keep track of frame executions
	int frameClock() {
		int hold = 0;
		w64 begin = this->pu->frame;
		while (begin == this->pu->frame) hold += this->execute();
		return hold;
	}

	//System Buffer - pause buffer by returning current image
	cimg_library::CImg<w8> * buff() { return this->pu->front; }

	//Palette Colors (TODO)

};

#endif
