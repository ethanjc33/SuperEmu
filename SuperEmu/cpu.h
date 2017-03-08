//Ethan Clark <ejc49@zips.uakron.edu>
//Central Processing Unit

#ifndef CPU_HPP
#define CPU_HPP

#include "instruction.h"

//Central Processing Unit
struct CPU {
	CPU() = default;
	virtual ~CPU() = default;

	void cyc() {

	}
};

//Components of the CPU
struct reg : CPU {
	//Registers
	w16 pc;								//Program Counter - supports 65,536 direct memory locations
	w8  a = 0;							//Accumulator - supports carrying, overflow detection, etc.
	w8  x = 0;							//Index - used for several addressing modes
	w8  y = 0;							//Index - used for several addressing modes
	w8  s = 0;							//Stack Pointer - can be accessed using interupts, pulls, pushes, and transfers
	w8  p = 0;							//Status Register - arithmetic, testing, and branch instructions

										//Status Flags						//Note: Bits 4 / 5 are not necessary here
	bool carry;							//Bit 0
	bool zero;							//Bit 1
	bool interupt;						//Bit 2 (Interupt disable on / off)
	bool decimal;						//Bit 3 (Exists for compatability, has no function)
	bool overflow;						//Bit 6
	bool negative;						//Bit 7

	w64 cycles;							//Clock Cycle Counter
	int staller;						//Number of stalled cycles

										//Flag Setter Functions				//Other flags are set through individual opcode instructions
	void sZero(w8 b) {					//Enables Zero flag if bit is 0, disables otherwise
		if (b == 0) zero = true;
		else zero = false;
	}

	void sNegative(w8 b) {				//Enables Negative flag if bit is 0, disables otherwise
		if (b == 0) negative = false;
		else negative = true;
	}

	void sCarry(w8 d, w8 b) {			//Enables Carry flag if first bit is >= to second bit, disables otherwise
		if (d < b) carry = false;
		else carry = true;
		w8 hold = d - b;
		sZero(hold);
		sNegative(hold);
	}

	//Enables all flags
	void sEnable() {
		carry = true; zero = true; interupt = true;
		decimal = true; overflow = true; negative = true;
	}

	//Adds CPU cycles - for various instructions that need to update cycle count
	void branch(ins * u) {
		++cycles;
		if (u->procou != u->address) ++cycles;
		//TODO: Better way to compare addresses of different pages?
	}

	//Returns CPU to original state
	void reset() {

	}

	//Reads bytes
	void read() {
		//TODO: Figure out how
	}

	//Writes bytes
	void write() {
		//TODO: Figure out how
	}

	//Functions to Push and Pull the processor status from the stack
	void push_back(w8 b) {
		//Write()
		--s;
	}

	w8 pop() {
		++s;
		//Read()
	}

};

#endif
