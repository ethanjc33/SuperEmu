//Ethan Clark <ejc49@zips.uakron.edu>
//Instruction Package


#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <stdint.h>
#include <cstdlib>

typedef uint_least8_t w8;		//Certain elements (i.e. registers) are all a byte wide
typedef uint_least16_t w16;		//Certain elements (i.e. program counter) are two bytes wide
typedef uint_least32_t w32;		//Certain elements (i.e. iNES identifiers) could be four bytes wide
typedef uint_least64_t w64;		//Certain elements (i.e. clock cycle counter) require more bytess


//Enums

//Interupt Modes for CPU (none, NMI, IRQ)
namespace interEnum { enum interMode { no, nm, ir };  }


//Mirror mode qualifiers
namespace mirEnum {
	enum mirMode {
		singleZero = 0, singleOne,
		vertical, horizontal
	};
}


//Mirror Modes Lookup
namespace mirrors {
	w16 mirLookup(w16 adr, w8 mirType);
}

#endif
