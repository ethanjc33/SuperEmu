//Ethan Clark <ejc49@zips.uakron.edu>
//Audio Processing Unit

#ifndef APU_HPP
#define APU_HPP

#include "instruction.h"


//Note: Due to time constraints, was not able to figure out this unit's implementation.
//		This was unfortunate, but would rather deal with no sound than suffer the entire project

//Audio Processing Unit
struct APU {

	APU() = default;
	~APU() = default;

	//Register Read
	w8 regRPrompt(w16 adr) {

	}

	//Register Write
	w8 regWPrompt(w16 adr, w8 out) {

	}


	//Process
	void execute() {

	}

};

#endif
