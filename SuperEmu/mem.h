//Ethan Clark <ejc49@zips.uakron.edu>
//Memory representation

#ifndef MEM_HPP
#define MEM_HPP

#include "nes.h"
#include <cstdlib>

//Much like the maps implementation, the calculations for the specific address values
//in the read and write functions came from various sources, listed in the references

//Memory Representation
struct mem {
	mem() = default;
	virtual ~mem() = default;

	//Read & Write Functions for the CPU and PPU - each to be overridden
	virtual w8 read(w16 adr) { }
	virtual w8 write(w16 adr, w8 out) { }

};

//CPU Representation in Memory
struct central : mem {

	//For creation of CPU Memory, must contain current state of the system
	sys * cm;

	central() = default;
	virtual ~central() = default;

	//Overrides

	//Determining which register to read based on the address
	w8 read(w16 adr) {

		//RAM
		if (adr < 0x2000) return this->cm->ram[adr % 2048];

		//PPU - Register Case One
		else if (adr < 0x4000) return; //Add: this->cm->pu.readReg((adr % 8) + 8192);

		//PPU - Register Case Two
		else if (adr == 0x4014) return; //Add: this->cm->pu.readReg(adr);

		//APU
		else if (adr == 0x4015) return; //Add: this->cm->au.readReg(adr);

		//Controller One
		else if (adr == 0x4016) return; //Add: this->cm->one.read();

		//Controller Two
		else if (adr == 0x4017) return; //Add: this->cm->two.read();

		//Input Output (apparently not necessary?)
		else if (adr < 0x6000) return;

		//Mapper
		else if (adr >= 0x6000) return this->cm->mmc.read(adr);

		//Register did not exist - immediately stop
		else std::abort();

	}

	//Determining which register to write to based on address
	w8 write(w16 adr, w8 out) {

		//RAM
		if (adr < 0x2000) out = this->cm->ram[adr % 2048];

		//PPU - Register Case One
		else if (adr < 0x4000); //this->cm->pu.writeReg((adr % 8) + 8192, out);

		//APU - Register Case One
		else if (adr < 0x4000); //this->cm->au.writeReg(adr, out);

		//PPU - Register Case Two
		else if (adr == 0x4014); //this->cm->pu.writeReg(adr, out);

		//APU - Register Case Two
		else if (adr == 0x4015) ; //this->cm->au.writeReg(adr, out);

		//Controller One
		else if (adr == 0x4016) {
			  //this->cm->one.write(adr, out);
			  //this->cm->two.write(adr, out);
		}

		//Controller Two
		else if (adr == 0x4017) ; //this->cm->au.write(adr, out);

		//Input Output (apparently not necessary?)
		else if (adr < 0x6000) ;

		//Mapper
		else if (adr >= 0x6000) this->cm->mmc.write(adr, out);

		//Register did not exist - immediately stop
		else std::abort();

	}


	//Creation of CPU Memory
	central * createCM(sys * s) {
		central * c = new central;
		c->cm = s;
		return c;
	}

};


//PPU Representation in Memory
struct picture : mem {

	//For creation of PPU Memory, must contain current state of the system
	sys * pm;

	picture() = default;
	virtual ~picture() = default;

	//Overrides

	w8 read(w16 adr) {
		adr %= 0x4000;

		//Mapper
		if (adr < 0x2000) return this->pm->mmc.read(adr);

		//Establishes mirror mode to be used
		else if (adr < 0x3F00) return; //Add: this->pm->pu.table[mirLookup(this->pm->ridge.mir, adr) % 2048];

		//Read from palette register
		else if (adr < 0x4000) return; this->pm->pu.palettePrompt((adr % 32));

		//Register did not exist - immediately stop
		else std::abort();

	}

	w8 write(w16 adr, w8 out) {
		adr %= 0x4000;

		//Mapper
		if (adr < 0x2000) this->pm->mmc.read(adr);

		//Establishes mirror mode to be used
		else if (adr < 0x3F00); //Add: out = this->pm->pu.table[mirLookup(adr, this->pm->ridge.mir) % 2048];

		//Write to palette register
		else if (adr < 0x4000); //Add: this->pm->pu.writePal((adr % 32), out);

		//Register did not exist - immediately stop
		else std::abort();

	}


	//Creation of PPU Memory
	picture * createPM(sys * s) {
		picture * p = new picture;
		p->pm = s;
		return p;
	}

};

#endif
