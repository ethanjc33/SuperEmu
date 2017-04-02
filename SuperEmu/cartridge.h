//Ethan Clark <ejc49@zips.uakron.edu>
//Game Cartridge (ROM File Manipulation)

#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <vector>
#include "instruction.h"


//Container for ROM-specific details
struct cart {

	std::vector<w8> prg;	//Container for PRG-ROM banks
	std::vector<w8> chr;	//Container for CHR-ROM banks
	w8 map;					//Mapper type - different ROMs use different types (see "maps" class)
	w8 mir;					//Mirror Mode
	w8 bat;					//Contains battery

	cart() = default;
	virtual ~cart() = default;

	cart(std::vector<w8> prg, std::vector<w8> chr, w8 map, w8 mir, w8 bat)
		:prg(prg), chr(chr), map(map), mir(mir), bat(bat)
	{ }

	//Creates a new instance of a cartridge when starting up the emulator
	cart * createCart(std::vector<w8> prg, std::vector<w8> chr, w8 map, w8 mir, w8 bat) {
		return &cart(prg, chr, map, mir, bat);
	}

};


//ROM File Game Pack Representation
struct rom {
	/*Reads from an iNES file (the 'standard' for emulator reading) and ultimately returns a cartridge
	All iNES files open with the same header format, so we can parse through that byte by byte.
	Actual NES cartridges had all the coded information located in different places, so this
	standard format solves the basic need of the emulator - actually having a game file.*/

	w32 identify;			//Three bytes: string "NES" and One byte: value $1A - Begins every iNES file
	w8  prgBank;			//Number of 16KB PRG-ROM banks - area of ROM that stores the program code
	w8  chrBank;			//Number of 8KB CHR-ROM / VROM banks - graphics & patterns information
	w8  oneByte;			//Control Byte One - each bit does something different
	w8  twoByte;			//Control Byte Two - each bit does something different
	w8  ramBank;			//Number of 8KB RAM banks - assumed to be one page of RAM when 0
	w8  blank[7];			//Reserved space - should always be 0



};

#endif
