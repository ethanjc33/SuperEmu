//Ethan Clark <ejc49@zips.uakron.edu>
//Game Controller

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

//Input & Output Representation
struct con {
	/*The NES had two controller ports available: player one in $4016 and two in $4017.
	The controllers are both mapped to memory, and read in the order:
	A, B, Select, Start, Up, Down, Left, Right
	For our educational purposes, we will not be concerned with peripherals (ROB, Light Gun, etc.)*/

	bool a, b, select, start, up, down, left, right;

	//Read(), Write()
	//TODO: Learn how to map to keyboard / Xbox controller

};

#endif
