//Ethan Clark <ejc49@zips.uakron.edu>
//Memory representation

#ifndef MEM_HPP
#define MEM_HPP

struct sys;

#include "instruction.h"

//Much like the maps implementation, the calculations for the specific address values
//in the read and write functions came from various sources, listed in the references

//Memory Representation
struct mem {
	mem() = default;
	virtual ~mem() = default;

	//Read & Write Functions for the CPU and PPU - each to be overridden
	virtual w8 read(w16 adr) = 0;
	virtual void write(w16 adr, w8 out) = 0;

};


//CPU Representation in Memory
struct central : mem {

	//For creation of CPU Memory, must contain current state of the system
	sys * cm;

	//Constructors & Destructor
	central() = default;
	central(sys * s);
	virtual ~central() = default;

	//central * createCentral(sys * s);
	w8 read(w16 adr);
	void write(w16 adr, w8 out);

};


//PPU Representation in Memory
struct picture : mem {

	//For creation of PPU Memory, must contain current state of the system
	sys * pm;

	//Constructors & Destructor
	picture() = default;
	picture(sys * s);
	virtual ~picture() = default;

	//picture * createPM(sys * s);
	w8 read(w16 adr);
	void write(w16 adr, w8 out);

};

#endif
