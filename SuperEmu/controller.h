//Ethan Clark <ejc49@zips.uakron.edu>
//Game Controller

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

/*The NES had two controller ports available: player one in $4016 and player two in $4017.
The controllers are both mapped to memory, and read in the order:
A, B, Select, Start, Up, Down, Left, Right
For our educational purposes, we will not be concerned with peripherals (ROB, Light Gun, etc.)*/

//It may be more viable (according to wikis) to define these as a series of bit flags
/*Like so:
  BUTTON_A        = 1 << 7
  BUTTON_B        = 1 << 6
  BUTTON_SELECT   = 1 << 5
  BUTTON_START    = 1 << 4
  BUTTON_UP       = 1 << 3
  BUTTON_DOWN     = 1 << 2
  BUTTON_LEFT     = 1 << 1
  BUTTON_RIGHT    = 1 << 0
*/

namespace conEnum { enum conPress { a, b, select, start, up, down, left, right }; }


//Input & Output Representation
struct con {

	w8 cur;								//Current button state
	w8 strobe;							//Strobe effect
	bool buttons[8];					//Boolean buttons representation

	//0 - a, 1 - b, 3 - select, 4 - start, 5 - up, 6 - down, 7 - left, 8 - right

	//TODO: Figure out how to incorporate Xbox gamepad within Visual Studio

	con() = default;
	~con() = default;


	//Set button states to current presses
	void set(bool presses[8]) {
		for (int i = 0; i < 8; ++i) buttons[i] = presses[i];
	}


	//TODO: Learn how to map to keyboard / Xbox controller (everything for it = difficult)

};

#endif
