//Ethan Clark <ejc49@zips.uakron.edu>

#include "instruction.h"


//Mirror Modes Lookup
w16 mirrors::mirLookup(w16 adr, w8 mirType) {

	//Mirror Modes:
	//0 - Horiztonal, 1 - Vertical, 2 - Single (a), 3 - Single (b)
	w16 modeTable[5][4] = { { 0, 0, 1, 1 },{ 0, 1, 0, 1 },{ 0, 0, 0, 0 },{ 1, 1, 1, 1 },{ 0, 1, 2, 3 } };

	adr -= 0x2000;
	adr %= 0x1000;
	return (modeTable[mirType][(adr / 0x400)] * 0x400) + (adr % 0x400) + 0x2000;
}
