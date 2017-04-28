//Ethan Clark <ejc49@zips.uakron.edu>
//Memory Definitions

#include "mem.h"
#include "nes.h"


//CPU

central::central(sys * s)
	:cm(s) { }

//Overrides

//Determining which register to read based on the address
w8 central::read(w16 adr) {

	//RAM
	if (adr < 0x2000) return this->cm->ram[adr % 2048];

	//PPU - Register Case One
	else if (adr < 0x4000) return this->cm->pu->regRPrompt((adr % 8) + 8192);

	//PPU - Register Case Two
	else if (adr == 0x4014) return this->cm->pu->regRPrompt(adr);

	//APU
	else if (adr == 0x4015) return 0; //Add: this->cm->au.regRPrompt(adr);

	//Controller One
	else if (adr == 0x4016) return 0; //Add: this->cm->one.read();

	//Controller Two
	else if (adr == 0x4017) return 0; //Add: this->cm->two.read();

	//Input Output (???)
	else if (adr < 0x6000) return 0;

	//Mapper
	else if (adr >= 0x6000) return this->cm->mmc->read(adr, this->cm->ridge->map);

	else std::abort();

}


//Determining which register to write to based on address
void central:: write(w16 adr, w8 out) {

	//RAM
	if (adr < 0x2000) out = this->cm->ram[adr % 2048];

	//PPU - Register Case One
	else if (adr < 0x4000) this->cm->pu->regWPrompt((adr % 8) + 8192, out);

	//APU - Register Case One
	else if (adr < 0x4000); //Add: this->cm->au->regWPrompt(adr, out);

	//PPU - Register Case Two
	else if (adr == 0x4014) this->cm->pu->regWPrompt(out, adr);

	//APU - Register Case Two
	else if (adr == 0x4015); //Add: this->cm->au->regWPrompt(adr, out);

	//Controller One
	else if (adr == 0x4016) {
		//this->cm->one.write(adr, out);
		//this->cm->two.write(adr, out);
	}

	//APU - Register Case Three
	else if (adr == 0x4017); //Add: this->cm->au.regWPrompt(adr, out);

	//Input Output (???)
	else if (adr < 0x6000); //TODO

	//Mapper
	else if (adr >= 0x6000) this->cm->mmc->write(adr, out, this->cm->ridge->map);

	//Register did not exist - immediately stop
	else std::abort();

}


//PPU

picture::picture(sys * s)
	:pm(s) { }

//Overrides

w8 picture::read(w16 adr) {
	adr %= 0x4000;

	//Mapper
	if (adr < 0x2000) return this->pm->mmc->read(adr, this->pm->ridge->map);

	//Establishes mirror mode to be used
	else if (adr < 0x3F00) return 0; //Add: this->pm->pu->table[mirLookup(this->pm->ridge.mir, adr) % 2048];

	//Read from palette register
	else if (adr < 0x4000) return this->pm->pu->paletteRPrompt((adr % 32));

	else std::abort();

}


void picture::write(w16 adr, w8 out) {
	adr %= 0x4000;

	//Mapper
	if (adr < 0x2000) this->pm->mmc->read(adr, this->pm->ridge->map);

	//Establishes mirror mode to be used
	else if (adr < 0x3F00); //Add: out = this->pm->pu.table[mirLookup(adr, this->pm->ridge.mir) % 2048];

	//Write to palette register
	else if (adr < 0x4000) this->pm->pu->paletteWPrompt((adr % 32), out);

	//Register did not exist - immediately stop
	else std::abort();

}
