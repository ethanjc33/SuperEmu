//Ethan Clark <ejc49@zips.uakron.edu>
//Instruction Package

//Note: As this project has progressed, several elements were better suited to a confined
//		file such as this. So this contains more than the instruction class, and it's
//		used so frequently that it doesn't seem worthwhile to change it now.

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <stdint.h>

typedef uint_least8_t w8;		//Certain elements (i.e. registers) are all a byte wide
typedef uint_least16_t w16;		//Certain elements (i.e. program counter) are two bytes wide
typedef uint_least32_t w32;		//Certain elements (i.e. iNES identifiers) could be four bytes wide
typedef uint_least64_t w64;		//Certain elements (i.e. clock cycle counter) require more bytess


//Instruction Package - Allows for simpler opcode functionality / Saves space in repeating opcodes
struct ins {
	w16 address;				//Address
	w16 procou;					//Program Counter
	w8  mode;					//Mode
};


//Interupt Modes for CPU
typedef enum interMode { no, nm, ir } interMode;


//PPU Register Packages - contains elements necessary for those functions defined in ppu.h
//Most will be used as boolean (b) flags, necessary to be 8-Bit values since we shift bits

//$2000 (PPUCTRL)
struct ctrlP {
	w8 bNames;
	w8 bUpper;
	w8 bSprites;
	w8 bBack;
	w8 bSize;
	w8 bMS;
};

//$2001 (PPUMASK)
struct maskP {
	w8 bScale;
	w8 bLBack;
	w8 bLSprite;
	w8 bBack;
	w8 bSprites;
	w8 red;
	w8 green;
	w8 blue;
};

//$2002 (PPUSTATUS)
struct statP {
	w8 bZero;
	w8 bOverflow;
};


//Mirror Modes Lookup
w16 mirLookup(w16 adr, w8 mirType) {

	//Mirror Modes:
	//0 - Horiztonal, 1 - Vertical, 2 - Single (a), 3 - Single (b)
	w16 modeTable[5][4] = { { 0, 0, 1, 1 }, { 0, 1, 0, 1 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 1, 2, 3 } };

	adr -= 0x2000;
	adr %= 0x1000;
	return (modeTable[mirType][(adr / 0x400)] * 0x400) + (adr % 0x400) + 0x2000;
}

#endif
