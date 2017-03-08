//Ethan Clark <ejc49@zips.uakron.edu>
//Memory representation

#ifndef MEM_HPP
#define MEM_HPP

#include "cpu.h"
#include "ppu.h"
#include "apu.h"


//Memory Representation
struct mem {
	mem() = default;
	virtual ~mem() = default;

	//Read & Write Functions for the CPU, PPU, and APU - each to be overwritten
	virtual w8 read(w16 adr) { }
	virtual w8 write(w16 adr, w8 out) { }

};

//CPU Representation in Memory
struct central : mem {
	mem * cm;

	w8 read(w16 adr) {

	}

	w8 write(w16 adr, w8 out) {

	}

};

//PPU Representation in Memory
struct picture : mem {
	mem * pm;

	w8 read(w16 adr) {

	}

	w8 write(w16 adr, w8 out) {

	}

};

//APU Representation in Memory
struct audio : mem {
	mem * am;

	w8 read(w16 adr) {

	}

	w8 write(w16 adr, w8 out) {

	}

};

#endif
