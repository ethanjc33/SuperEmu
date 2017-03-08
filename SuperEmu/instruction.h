//Ethan Clark <ejc49@zips.uakron.edu>
//Instruction Package

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <stdint.h>

typedef uint_least8_t w8;		//Certain elements (i.e. registers) are all a byte wide
typedef uint_least16_t w16;		//Certain elements (i.e. program counter) are two bytes wide
typedef uint_least32_t w32;		//Certain elements (i.e. iNES identifiers) could be four bytes wide
typedef uint_least64_t w64;		//Certain elements (i.e. clock cycle counter) require more bytess

//Instruction Package - Allows for simpler opcode functionality / Saves space
struct ins {
	w16 address;				//Address
	w16 procou;					//Program Counter
	w8  mode;					//Mode
};

#endif