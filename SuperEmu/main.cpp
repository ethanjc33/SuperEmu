//Ethan Clark <ejc49@zips.uakron.edu>

/*The program SuperEmu serves as an emulator; that is, a piece of software that is designed to
behave like another system. In this case, we will be emulating the hardware seen in the NES.
Citation: https://nesdev.com/ - huge help when figuring specific values and representations.*/

#include "nes.h"

//Note: Although this emulator *almost* fully represents what I have seen to be the simplest
//		and closest representation of the NES' hardware, it still lacks a dedicated UI system
//		so the execution here in main is not complete / fully functional. Command line
//		UI has been used in the meantime.

//Main Application
int main() {

	sys nes;
	while (true) {
		nes.execute();
		nes.frameClock();
	}

	return 0;
}
