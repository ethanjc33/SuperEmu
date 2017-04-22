//Ethan Clark <ejc49@zips.uakron.edu>
//Implementation of PPU for wherever 'sys' class is used

#include "nes.h"
#include "ppu.h"

//Initialization & PPU setup
PPU * PPU::createPPU(sys * s) {
	PPU * x = new PPU;
	x->cc = s->cu;
	x->pMem.createPM(s);
	//Front and Back images -> must extract them from some other resource?
	x->frame = 0;
	x->scans = 240;
	x->cycles = 340;
	return x;
}

//TODO: Dynamic Allocation of objects
