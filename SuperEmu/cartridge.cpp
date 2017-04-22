//Ethan Clark <ejc49@zips.uakron.edu>

#include "cartridge.h"

/*The following function creates and returns a cart type. As mentioned, we will read through byte
by byte since the beginning of every iNES file is kept in the same exact format, so we can
actually handle parsing. After we extract all the necessary data, we are done with the file.
*/

cart * rom::openFile() {

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
	//Would only throw an error anyway if it was there, we don't pass it

	//Read PRG-ROM Banks
	std::vector<w8> prg;
	prg.resize(contents.prgBank * 0x4000);


	//Read CHR-ROM Banks (provide if not in file as to not set size to 0)
	std::vector<w8> chr;
	if (contents.chrBank == 0) { chr.resize(0x2000); }
	else { chr.resize(contents.chrBank * 0x2000); }


	//Return Cartridge representation
	cart * hold = new cart(prg, chr, map, mirror, bat);
	return hold;

}