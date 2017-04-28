//Ethan Clark <ejc49@zips.uakron.edu>
//Memory Mappers Implementation

#ifndef MAPS_HPP
#define MAPS_HPP

#include <cstdlib>

#include "cartridge.h"
#include "cpu.h"
#include "instruction.h"

struct sys;

/*Note: Throughout this header file, specific values are all taken for granted.
		That is, it became way too cluttered to fit every citation for every
		source on the exact value calculations, as research was conducted
		through various documentation sources and other working emulators'
		implementations. Some general citations can be found under the
		references page on the written paper alongside this project.
		
		Also, due to the difficulty of figuring these mappers out (they're rather
		lengthy processes to understand for this project's purposes), testing will
		mostly focus on ROMs that don't use mappers (i.e. Donkey Kong, Balloon Fighter,
		and Pac-Man). This said, the mappers only exist here for completeness.*/


//Implementation for individual memory mappers contained in iNES translation

//Memory Mapper 1 - MMC1
struct maps1 {

	/*All members here have values determined by the mapper type.
	  That is, these are important enough values to track, yet
	  they will differ between the other maps# since each ROM file
	  has its own set of values to keep track of. This goes for 
	  every maps# class here.*/

	cart * cp;				//Cartridge Pointer
	w8 co;					//Control byte
	w8 sr;					//Register Shifting
	w8 prgMode;				//PRG Mode
	w8 prgBank;				//PRG Bank
	w8 chrMode;				//CHR Mode
	w8 chrBOne;				//CHR Bank One
	w8 chrBTwo;				//CHR Bank Two
	int prgOff[2];			//Offsets used in value calculations for PRG
	int chrOff[2];			//Offsets used in value calculations for CHR


	//Constructors & Destructor

	maps1() = default;

	maps1(cart * r) {
		cp = r;
		sr = 0x10;
		prgOff[1] = prgOffset(-1);
	}

	~maps1() = default;

	//Basic Functions for Mapper Implementations

	w8 read(w16 adr) {
		//Determines values for members based on address location
		int hold1, hold2;
		
		if (adr < 0x2000) {
			hold1 = (adr / 0x1000);
			hold2 = (adr % 0x1000);
			return this->cp->chr.at(this->chrOff[hold1] + hold2);
		}

		else if (adr >= 0x8000) {
			adr -= 0x8000;
			hold1 = (adr / 0x4000);
			hold2 = (adr % 0x4000);
			return this->cp->prg.at(this->prgOff[hold1] + hold2);
		}

		//Unsupported mapper type - TODO: Figure out other cases, if there are any?
		else std::abort();

	}

	void write(w16 adr, w8 out) {
		//Determines values for members based on address location
		int hold1, hold2;

		if (adr < 0x2000) {
			hold1 = (adr / 0x1000);
			hold2 = (adr % 0x1000);
			out = this->cp->chr.at(this->chrOff[hold1] + hold2);
		}

		else if (adr >= 0x8000) {
			adr -= 0x8000;
			hold1 = (adr / 0x4000);
			hold2 = (adr % 0x4000);
			out = this->cp->prg.at(this->prgOff[hold1] + hold2);
		}

		//Unsupported mapper type - TODO: Figure out other cases, if there are any?
		else std::abort();

		this->loadReg(adr, out);
	}

	//Mapper 1 specific functions below...

	//Load to registers
	void loadReg(w16 adr, w8 out) {
		if ((out & 0x80) == 0x80) {
			this->sr = 0x10;
			this->writeCon(this->co | 0x0C);
		}

		else {
			bool hold = (this->sr & 1) == 1;
			this->sr >>= 1;
			this->sr |= (out & 1) << 4;
			if (hold == true) {
				writeReg(adr, this->sr);
				this->sr = 0x10;
			}
		}
	}

	//Write to registers
	void writeReg(w16 adr, w8 out) {
		//Control: between $8000, $9FFF
		if (adr <= 0x9FFF) this->writeCon(out);

		//CHR Bank One: between $A000, $BFFF
		else if (adr <= 0xBFFF) { this->chrBOne = out; this->update(); }

		//CHR Bank Two: between $C000, $DFFF
		else if (adr <= 0xDFFF) { this->chrBTwo = out; this->update(); }

		//PRG Bank: between $E000, $FFFF
		else if (adr <= 0xFFFF) { this->prgBank = out; this->update(); }
	}

	//Write to control field
	void writeCon(w8 out) {
		this->co = out;
		this->chrMode = (out >> 4) & 1;
		this->prgMode = (out >> 2) & 3;
		w8 hold = out & 3;

		//Update mirror values

		switch (hold) {
		case (0): this->cp->mir = mirEnum::mirMode::singleZero; break;		//Single 0 mode
		case (1): this->cp->mir = mirEnum::mirMode::singleOne; break;		//Single 1 mode
		case (2): this->cp->mir = mirEnum::mirMode::vertical; break;		//Vertical mode
		case (3): this->cp->mir = mirEnum::mirMode::horizontal; break;		//Horizontal mode
		}

		this->update();
	}


	//Determines the offset values, for updating the registers during writes
	int prgOffset(int i) {
		if (i >= 0x80) i -= 0x100;
		i %= (this->cp->prg.size() / 0x4000);
		int hold = i * 0x4000;
		if (hold < 0) hold += this->cp->prg.size();
		return hold;
	}

	int chrOffset(int i) {
		if (i >= 0x80) i -= 0x100;
		i %= (this->cp->chr.size() / 0x4000);
		int hold = i * 0x4000;
		if (hold < 0) hold += this->cp->chr.size();
		return hold;
	}


	//Uses above functions in combination - note specific values are based off references
	//PRG & CHR Bank Mode switches between banks at certain addresses based on indexes calculated above
	void update() {

		//PRG
		//Case 0 - Switch 32KB at $8000
		//Case 1 - Same as Case 0
		//Case 2 - First bank at $8000, switch 16 KB at $C000
		//Case 3 - Opposite of Case 2

		switch (this->prgMode) {
		case '0': this->prgOff[0] = this->prgOffset(int(this->prgBank & 0xFE));
				  this->prgOff[1] = this->prgOffset(int(1 & this->prgBank));
				  break;
		case '1': this->prgOff[0] = this->prgOffset(int(this->prgBank & 0xFE));
				  this->prgOff[1] = this->prgOffset(int(1 & this->prgBank));
				  break;
		case '2': this->prgOff[0] = 0; this->prgOff[1] = this->prgOffset(int(this->prgBank)); break;
		case '3': this->prgOff[0] = this->prgOffset(int(this->prgBank)); this->prgOff[1] = this->prgOffset(-1); break;
		}

		//CHR
		//Case 0: switch 8KB
		//Case 1: switch separate 4KB banks

		switch (this->chrMode) {
		case '0': this->chrOff[0] = this->chrOffset(int(this->chrBOne & 0xFE));
				  this->chrOff[1] = this->chrOffset(int(this->chrBOne | 1));
				  break;
		case '1': this->chrOff[0] = this->chrOffset(int(chrBOne)); this->chrOff[1] = this->chrOffset(int(chrBTwo)); break;
		}
	}

};


//Memory Mapper 2 - MMC2
struct maps2 {

	cart * cp;							//Cartridge pointer
	int prgBOne, prgBTwo, prgBThree;	//PRG Banks 1, 2, 3


	//Constructors & Destructor

	maps2() = default;

	maps2(cart r) {
		int hold1 = (r.prg.size() / 0x4000);
		int hold2 = hold1 - 1;
		cp = &r;
		prgBOne = hold1;
		prgBTwo = hold2;
	}

	~maps2() = default;


	//Overrides

	w8 read(w16 adr) {

		if (adr < 0x2000) return this->cp->chr[adr];
		
		else if (adr >= 0xC000) {
			int hold = (adr - 0xC000) + (this->prgBThree * 0x4000);
			return this->cp->prg[hold];
		}

		else if (adr >= 0x8000) {
			int hold = (adr - 0x8000) + (this->prgBTwo * 0x4000);
			return this->cp->prg[hold];
		}

		//Unsupported mapper type - TODO: Figure out other cases, if there are any?
		else std::abort();
	}

	void write(w16 adr, w8 out) {

		if (adr < 0x2000) out = this->cp->chr[adr];

		else if (adr >= 0x8000) this->prgBTwo = out % prgBOne;

		//Unsupported mapper type - TODO: Figure out other cases, if there are any?
		else std::abort();
	}

};


//TODO: Figure out other mappers as to emulate more ROMs

//Memory Mapper 3
struct maps3 {
	maps3 * createM3(cart r) { return nullptr; }
	w8 read(w16 adr) { return 0; }
	void write(w16 adr, w8 out) { }
};

//Memory Mapper 4
struct maps4 {
	maps4 * createM4(cart r) { return nullptr; }
	w8 read(w16 adr) { return 0; }
	void write(w16 adr, w8 out) { }
};

//Memory Mapper 7
struct maps7 {
	maps7 * createM7(cart r) { return nullptr; }
	w8 read(w16 adr) { return 0; }
	void write(w16 adr, w8 out) { }
};



//Memory Mapper
union maps {

	maps1 * m1;
	maps2 * m2;
	maps3 m3;
	maps4 m4;
	maps7 m7;

	maps(cart r) {
		switch (r.map) {
		case (0): m2 = new maps2;
		case (1): m1 = new maps1;
		case (2): m2 = new maps2;
		default: std::abort();
		}
	}

	//Switches to designated map type for whatever member the union currently holds
	w8 read(w16 adr, w8 mapType) {
		switch (mapType) {
		case '0': return m2->read(adr); break;
		case '1': return m1->read(adr); break;
		case '2': return m2->read(adr); break;
		case '3': return m3.read(adr); break;
		case '4': return m4.read(adr); break;
		case '7': return m7.read(adr); break;
		default: std::abort();
		}
	}

	//Switches to designated map type for whatever member the union currently holds
	void write(w16 adr, w8 out, w8 mapType) {
		switch (mapType) {
		case '0': m2->write(adr, out); break;
		case '1': m1->write(adr, out); break;
		case '2': m2->write(adr, out); break;
		case '3': m3.write(adr, out); break;
		case '4': m4.write(adr, out); break;
		case '7': m7.write(adr, out); break;
		}
	}

};


#endif
