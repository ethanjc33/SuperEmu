//Ethan Clark <ejc49@zips.uakron.edu>
//Implementation of PPU for wherever 'sys' class is used

#include "nes.h"
#include "ppu.h"

//PPU Constructors

PPU::PPU() = default;

PPU::PPU(sys * s) {
	cc = s->cu;
	pMem = new picture(s);
	//TODO: Front and Back images
	frame = 0;
	scans = 240;
	cycles = 340;
}

//PPU Destructor
PPU::~PPU() {
	delete pMem;
}
