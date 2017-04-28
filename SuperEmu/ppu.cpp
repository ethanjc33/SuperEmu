//Ethan Clark <ejc49@zips.uakron.edu>
//Implementation of PPU for wherever 'sys' class is used

#include "nes.h"
#include "ppu.h"

//PPU Constructors

PPU::PPU() = default;

PPU::PPU(sys * s) {
	cc = s->cu;
	pMem = new picture(s);
	//256 x 240 image, 2D (depth = 1), 3 channels (RGB)
	front = new cimg_library::CImg<w8>(256, 240, 1, 3);
	back = new cimg_library::CImg<w8>(256, 240, 1, 3);
	frame = 0;
	scans = 240;
	cycles = 340;
}

//PPU Destructor
PPU::~PPU() {
	delete pMem;
	delete front;
	delete back;
}
