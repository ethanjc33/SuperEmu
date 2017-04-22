//Ethan Clark <ejc49@zips.uakron.edu>
//Game Cartridge (ROM File Manipulation)

#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <iostream>
#include <fstream>
#include <valarray>
#include <string>
#include <vector>
#include "instruction.h"


//Container for ROM-specific details
struct cart {

	std::vector<w8> prg;	//Container for PRG-ROM banks
	std::vector<w8> chr;	//Container for CHR-ROM banks
	mirEnum::mirMode mir;	//Mirror Mode
	w8 bat;					//Contains battery
	w8 map;					//Mapper type - different ROMs use different types (see "maps" class)

	
	//Default
	cart() = default;

	//Constructor for iNES file format return
	cart(std::vector<w8> prg, std::vector<w8> chr, w8 map, mirEnum::mirMode mir, w8 bat)
		:prg(prg), chr(chr), map(map), mir(mir), bat(bat)
	{ }

	//Destructor
	~cart() { delete this; }


	//Creates a new cartridge when starting up the emulator
	cart * createCart(std::vector<w8> prg, std::vector<w8> chr, w8 map, mirEnum::mirMode mir, w8 bat) {
		cart * x = new cart(prg, chr, map, mir, bat);
		return x;
	}

};


//ROM File iNES Representation
#pragma pack(push, r1, 1)
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

};
#pragma pack(pop, r1)


/*The following function creates and returns a cart type. As mentioned, we will read through byte
  by byte since the beginning of every iNES file is kept in the same exact format, so we can
  actually handle parsing. After we extract all the necessary data, we are done with the file.
*/

cart * openFile() {

	rom contents = rom{};
	std::string fileName;
	std::fstream x;

	//Open File via user prompt
	std::cout << "/nHello and welcome to Super Emu . . .\n\n";
	std::cout << "Enter the name of a .nes file in this applications's directory:\n";
	std::getline(std::cin, fileName);
	x.open(fileName + ".nes", std::fstream::out | std::fstream::binary);
	x.write((char *)&contents, sizeof(rom));
	x.close();

	//Read ROM header piece by piece, since file contains a structured binary data representation
	x.open(fileName + ".nes", std::fstream::in | std::fstream::binary);

	x.read((char *)&contents, sizeof(contents.identify));
	x.read((char *)&contents, sizeof(contents.prgBank));
	x.read((char *)&contents, sizeof(contents.chrBank));
	x.read((char *)&contents, sizeof(contents.oneByte));
	x.read((char *)&contents, sizeof(contents.twoByte));
	x.read((char *)&contents, sizeof(contents.ramBank));

	x.close();

	//Now the ROM file header has been decoded into all the pieces we need to set up our cartridge:

	//Make sure that file is in the iNES file format using standard serial index
	if (contents.identify != 0x1A53454E) {
		std::cout << "\nFile not Valid\n";
		std::abort();
	}

	//Mapper Extraction
	w8 p = contents.oneByte >> 4;
	w8 q = contents.twoByte >> 4;
	w8 map = p | q << 1;

	//Mirror Extraction
	w8 r = contents.oneByte >> 4;
	w8 s = (contents.twoByte >> 3) & 1;
	w8 m = r | s << 1;
	mirEnum::mirMode mirror = mirEnum::mirMode(m);

	//Battery
	w8 bat = (contents.oneByte >> 1) & 1;

	//Trainer may be present, but we'll assume it's not for simplicty's sake
	//Would only throw an error anyway if it was there

	//Read PRG-ROM Banks
	std::vector<w8> prg;
	prg.resize(contents.prgBank * 0x4000);


	//Read CHR-ROM Banks (provide if not in file as to not set size to 0)
	std::vector<w8> chr;
	if (contents.chrBank == 0) { chr.resize(0x2000); }
	else { chr.resize(contents.chrBank * 0x2000); }


	//Return Cartridge representation
	cart hold;
	return (hold.createCart(prg, chr, map, mirror, bat));

}




#endif
