//Ethan Clark <ejc49@zips.uakron.edu>
//Central Processing Unit

#ifndef CPU_HPP
#define CPU_HPP

#include "instruction.h"
#include "mem.h"

//Note: Removed object oriented CPU structure as it had no useful impact on program
//Note: Specific opcode instructions achieved through documentation (posted elsewhere) in other implementations

struct sys;


//Global Constants - Necessary for execution of an instruction

//Represents an opcode's addressing mode ID
modeEnum::sourceMode modes[modeEnum::siz] = {
	modeEnum::imp, modeEnum::xid, modeEnum::imp, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::acc, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::abs, modeEnum::xid, modeEnum::imp, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::acc, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::imp, modeEnum::xid, modeEnum::imp, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::acc, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::imp, modeEnum::xid, modeEnum::imp, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::acc, modeEnum::imm,
	modeEnum::ind, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::imm, modeEnum::xid, modeEnum::imm, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::imp, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpy, modeEnum::zpy,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::aby, modeEnum::aby,
	modeEnum::imm, modeEnum::xid, modeEnum::imm, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::imp, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpy, modeEnum::zpy,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::aby, modeEnum::aby,
	modeEnum::imm, modeEnum::xid, modeEnum::imm, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::imp, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::imm, modeEnum::xid, modeEnum::imm, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::imp, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx
};


//Represents an opcode's number of cycles
const w8 rate[0x100] = {
	7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6, 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6, 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6, 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6, 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4, 2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
	2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4, 2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
	2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7
};


//Represents an opcode's page offset (add to cycles)
const w8 pageOffset[0x100] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0
};


//Represents an opcode's size
const w8 bytes[0x100] = {
	1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	3, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 1, 0, 3, 3, 3, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 0, 3, 0, 0,
	2, 2, 2, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
	2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0
};



//Components of the CPU
struct CPU {

	//Registers
	w16 pc;							//Program Counter - supports 65,536 direct memory locations
	w8  a = 0;						//Accumulator - supports carrying, overflow detection, etc.
	w8  x = 0;						//Index - used for several addressing modes
	w8  y = 0;						//Index - used for several addressing modes
	w8  s = 0;						//Stack Pointer - can be accessed using interupts, pulls, pushes, transfers
	w8  p = 0;						//Status Register - arithmetic, testing, and branch instructions

	//Status Flags					//Note: Bits 4 / 5 are not necessary here
	bool carry;						//Bit 0
	bool zero;						//Bit 1
	bool interupt;					//Bit 2 (Interupt disable on / off)
	bool decimal;					//Bit 3 (Exists for compatability)
	bool overflow;					//Bit 6
	bool negative;					//Bit 7

	//Counters & Such
	interEnum::interMode im;		//Interupt Mode - 3 possible values
	modeEnum::sourceMode am;		//Addressing Mode - 13 possible values
	w64 cycles;						//Clock Cycle Counter
	int staller;					//Number of stalled cycles

	//Memory Representation
	central cMem;					//CPU Memory


	//Constructors & Destructor
	CPU() = default;
	~CPU() { delete this; }


	//Flag Setter Functions - other flags set through indidvual opcode functions

	//Enables Zero flag if bit is 0, disables otherwise
	void sZero(w8 b) {
		if (b == 0) zero = true;
		else zero = false;
	}

	//Enables Negative flag if bit is 0, disables otherwise
	void sNegative(w8 b) {
		if ((b & 0x80) != 0) this->negative = true;
		else this->negative = false;
	}

	//Enables Carry flag if first bit is >= to second bit, disables otherwise
	void sCarry(w8 d, w8 b) {
		if (d < b) this->carry = false;
		else this->carry = true;
		w8 hold = d - b;
		sZero(hold);
		sNegative(hold);
	}

	//Resets Status Flags to Chosen State
	void set(w8 b) {
		this->zero = (b >> 1) & 1;
		this->interupt = (b >> 2) & 1;
		this->decimal = (b >> 3) & 1;
		this->overflow = (b >> 6) & 1;
		this->negative = (b >> 7) & 1;
		this->carry = (b >> 0) & 1;
	}

	//Returns status flag states (based on bit shifts)
	w8 enable() {
		w8 hold;
		//Must contain << notation for shifting and interpreting boolean to binary
		hold |= this->carry << 0;
		hold |= this->zero << 1;
		hold |= this->interupt << 2;
		hold |= this->decimal << 3;
		hold |= this->overflow << 6;
		hold |= this->negative << 7;
		return hold;
	}


	//Initialization & CPU setup

	//Creation of CPU in system - used in opcodes
	CPU * createCPU(sys * s) {
		CPU * x = new CPU;
		x->cMem.createCentral(s);
		x->s = 253;
		x->pc = x->hilo(65532);
		x->set(0x24);
		return x;
	}

	
	//Misc. Functions...

	//Adds CPU cycles - for various instructions that need to update cycle count
	void branch(ins * u) {
		++cycles;
		if (test(u->procou, u->address)) ++cycles;
	}

	bool test(w16 p, w16 q) { return ((p & 0xFF00) != (q & 0xFF00)); }

	//Comparison Test (for ease of use in specific opcodes)
	void compare(w8 p, w8 q) {
		this->sZero(p - q);
		this->sNegative(p - q);
		if (p < q) this->carry = false;
		else this->carry = true;
	}

	//Returns hi and lo bytes, depending on situation
	w16 hilo(w16 adr) {
		w16 hi = this->cMem.read(adr + 1);
		w16 lo = this->cMem.read(adr);

		return (hi << 0x08) | lo;
	}

	//Emulates NES CPU bug found in original hardware
	w16 oops(w16 adr) {
		w16 hi = this->cMem.read(w16(w8(adr) + 1));
		w16 lo = this->cMem.read(adr);

		return (hi << 0x08) | lo;
	}


	//Process an instruction - returns CPU cycles
	int execute() {

		//Halts execution round if cycle is stalled
		if (this->staller > 0) { this->staller--; return 1; }
		w64 hold = this->cycles;

		//Trigger interupts for either NMI or IRQ
		switch (this->im) {
		case interEnum::nm: this->nmi(); break;
		case interEnum::ir: this->irq(); break;
		default: break;
		}

		//Set interupt enum type to none
		this->im = interEnum::no;

		//Fetch Instruction from PC & Match it to proper mode
		w8 todo = this->cMem.read(this->pc);
		this->am = modes[todo];

		//Address and Page Flag
		w16 adr = 0;
		bool page = false;

		//Solves for correct address location, stops if location does not exist
		switch (this->am) {
		case modeEnum::zop: adr = w16(this->cMem.read(1 + this->pc)); break;
		case modeEnum::zpx: adr = w16(this->cMem.read(1 + this->pc) + this->x); break;
		case modeEnum::zpy: adr = w16(this->cMem.read(1 + this->pc) + this->y); break;
		case modeEnum::abs: adr = this->hilo(1 + this->pc); break;
		case modeEnum::abx:
			adr = this->hilo(1 + this->pc) + w16(this->x);
			page = this->test(adr - w16(this->x), adr);
			break;
		case modeEnum::aby:
			adr = this->hilo(1 + this->pc) + w16(this->y);
			page = this->test(adr - w16(this->y), adr);
			break;
		case modeEnum::xid: adr = this->oops(w16(this->cMem.read(1 + this->pc) + this->x)); break;
		case modeEnum::ind: adr = this->oops(w16(this->cMem.read(1 + this->pc))); break;
		case modeEnum::idx:
			adr = this->oops(w16(this->cMem.read(1 + this->pc) + this->y));
			page = this->test(adr - w16(this->y), adr);
			break;
		case modeEnum::rel:
		{	w16 temp = w16(this->cMem.read(1 + this->pc));
			if (temp >= 80) adr = this->pc + temp + 2;
			else adr = this->pc + temp - 254;
			break; }
		case modeEnum::acc: adr = 0; break;
		case modeEnum::imm: adr = 1 + this->pc; break;
		case modeEnum::imp: adr = 0; break;
		default: std::abort();
		}

		//Opcode Sizes & Cycles
		this->pc += w16(bytes[todo]);
		this->cycles += w64(rate[todo]);
		if (page == true) this->cycles += w64(pageOffset[todo]);

		//Prompt an Opcode
		ins u;
		u.mode = this->am;
		u.address = adr;
		u.procou = this->pc;
		this->prompt(todo, &u);

		//Return the number of cycles we added during this execution
		return int((this->cycles - hold));

	}


	//Functions to Push and Pull the processor status from the stack
	//Note: "bytes" here refer to hi and lo

	void push_byte(w8 b) {
		this->cMem.write((256 | w16(this->s)), b);
		--s;
	}

	void push_bytes(w16 b) {
		w8 lo = w8(b & 0xFF);
		w8 hi = w8(b >> 8);
		this->push_byte(hi);
		this->push_byte(lo);
	}

	w8 pop_byte() {
		++s;
		return this->cMem.read((256 | w16(this->s)));
	}

	w16 pop_bytes() {
		w16 lo = w16(this->pop_byte());
		w16 hi = w16(this->pop_byte());
		return (hi << 0x08) | lo;
	}


	//Specific types of interupts featured on the 6502 processor

	//NMI	- Non-maskable interupt
	void nmi() {
		this->push_bytes(this->pc);
		this->php(nullptr);
		this->pc = this->hilo(0xFFFA);
		this->interupt = true;
		this->cycles += 7;
	}

	//IRQ	- Interupt IRQ
	void irq() {
		this->push_bytes(this->pc);
		this->php(nullptr);
		this->pc = this->hilo(0xFFFE);
		this->interupt = true;
		this->cycles += 7;
	}


	//Opcode instructions listed out below
	//Note: All of these required exact operation, check references for those citations

	//ADC   - Add Memory to Accumulator with Carry
	void adc(ins * u) {
		w8 x = this->cMem.read(u->address);
		w8 y = this->a;
		w8 z = this->carry;
		this->a = x + y + z;
		this->sZero(this->a);
		this->sNegative(this->a);

		//Update Carry Flag
		if ((int(x) + int(y) + int(z)) <= 256) this->carry = false;
		else this->carry = true;

		//Update Overflow Flag
		if ((((y^(this->a)) & 0x80) == 0) && (((y^(this->a)) & 0x80) != 0)) this->overflow = true;
		else this->overflow = false;
	}


	//AND   - "AND" Memory with Accumulator
	void and(ins * u) {
		this->a &= this->cMem.read(u->address);
		this->sZero(this->a);
		this->sNegative(this->a);
	}


	//ASL   - Shift Left One Bit (Memory or Accumulator)
	void asl(ins * u) {
		if (u->mode == modeEnum::sourceMode::acc) {
			this->carry = (this->a >> 7) & 1;
			this->a <<= 1;
			this->sZero(this->a);
			this->sNegative(this->a);
		}

		else {
			w8 hold = this->cMem.read(u->address);
			this->carry = (hold >> 7) & 1;
			hold <<= 1;
			this->cMem.write(u->address, hold);
			this->sZero(hold);
			this->sNegative(hold);
		}
	}

	//BCC   - Branch on Carry Clear
	void bcc(ins * u) {
		if (this->carry == false) {
			this->pc = u->address;
			this->branch(u);
		}
	}

	//BCS   - Branch on Carry Set
	void bcs(ins * u) {
		if (this->carry == true) {
			this->pc = u->address;
			this->branch(u);
		}
	}

	//BEQ   - Branch on Result Zero
	void beq(ins * u) {
		if (this->zero == true) {
			this->pc = u->address;
			this->branch(u);
		}
	}

	//BIT   - Test Bits in Memory with Accumulator
	void bit(ins * u) {
		w8 hold = this->cMem.read(u->address);
		this->overflow = (hold >> 6) & 1;
		this->sZero(this->a & hold);
		this->sNegative(hold);
	}

	//BMI   - Branch on Result Minus
	void bmi(ins * u) {
		if (this->negative == true) {
			this->pc = u->address;
			this->branch(u);
		}
	}

	//BNE   - Branch on Result not Zero
	void bne(ins * u) {
		if (this->zero == false) {
			this->pc = u->address;
			this->branch(u);
		}
	}

	//BPL   - Branch on Result Plus
	void bpl(ins * u) {
		if (this->negative == false) {
			this->pc = u->address;
			this->branch(u);
		}
	}

	//BRK   - Force Break
	void brk(ins * u) {
		this->push_bytes(this->pc);
		this->php(u);
		this->sei(u);
		this->pc = this->hilo(0xFFFE);
	}

	//BVC   - Branch on Overflow Clear
	void bvc(ins * u) {
		if (this->overflow == false) {
			this->pc = u->address;
			this->branch(u);
		}
	}

	//BVS   - Branch on Overflow Set
	void bvs(ins * u) {
		if (this->overflow == true) {
			this->pc = u->address;
			this->branch(u);
		}
	}

	//CLC   - Clear Carry Flag
	void clc(ins * u) { this->carry = false; }

	//CLD   - Clear Decimal Mode
	void cld(ins * u) { this->decimal = false; }

	//CLI   - Clear interrupt Disable Bit
	void cli(ins * u) { this->decimal = false; }

	//CLV   - Clear Overflow Flag
	void clv(ins * u) { this->overflow = false; }

	//CMP   - Compare Memory and Accumulator
	void cmp(ins * u) { this->compare(this->a, this->cMem.read(u->address)); }

	//CPX   - Compare Memory and Index X
	void cpx(ins * u) { this->compare(this->x, this->cMem.read(u->address)); }

	//CPY   - Compare Memory and Index Y
	void cpy(ins * u) { this->compare(this->y, this->cMem.read(u->address)); }

	//DEC   - Decrement Memory by One
	void dec(ins * u) {
		w8 hold = this->cMem.read(u->address);
		--hold;
		this->cMem.write(u->address, hold);
		this->sZero(hold);
		this->sNegative(hold);
	}

	//DEX   - Decrement Index X by One
	void dex(ins * u) {
		this->x--;
		this->sZero(this->x);
		this->sNegative(this->x);
	}

	//DEY   - Decrement Index Y by One
	void dey(ins * u) {
		this->y--;
		this->sZero(this->y);
		this->sNegative(this->y);
	}


	//EOR   - "Exclusive-Or" Memory with Accumulator
	void eor(ins * u) {
		this->a ^= this->cMem.read(u->address);
		this->sZero(this->a);
		this->sNegative(this->a);
	}

	//INC   - Increment Memory by One
	void inc(ins * u) {
		w8 hold = this->cMem.read(u->address);
		++hold;
		this->cMem.write(u->address, hold);
		this->sZero(hold);
		this->sNegative(hold);
	}

	//INX   - Increment Index X by One
	void inx(ins * u) {
		this->x++;
		this->sZero(this->x);
		this->sNegative(this->x);
	}

	//INY   - Increment Index Y by One
	void iny(ins * u) {
		this->y++;
		this->sZero(this->y);
		this->sNegative(this->y);
	}

	//JMP   - Jump to New Location
	void jmp(ins * u) { this->pc = u->address; }

	//JSR   - Jump to New Location Saving Return Address
	void jsr(ins * u) {
		this->push_bytes(this->pc - 1);
		this->pc = u->address;
	}

	//LDA   - Load Accumulator with Memory
	void lda(ins * u) {
		this->a = this->cMem.read(u->address);
		this->sZero(this->a);
		this->sNegative(this->a);
	}

	//LDX   - Load Index X with Memory
	void ldx(ins * u) {
		this->x = this->cMem.read(u->address);
		this->sZero(this->x);
		this->sNegative(this->x);
	}

	//LDY   - Load Index Y with Memory
	void ldy(ins * u) {
		this->y = this->cMem.read(u->address);
		this->sZero(this->y);
		this->sNegative(this->y);
	}

	//LSR   - Shift Right One Bit(Memory or Accumulator)
	void lsr(ins * u) {
		if (u->mode == modeEnum::sourceMode::acc) {
			this->carry = (this->a >> 7) & 1;
			this->a <<= 1;
			this->sZero(this->a);
			this->sNegative(this->a);
		}

		else {
			w8 hold = this->cMem.read(u->address);
			this->carry = (hold >> 7) & 1;
			hold <<= 1;
			this->cMem.write(u->address, hold);
			this->sZero(hold);
			this->sNegative(hold);
		}
	}

	//NOP   - No Operation
	void nop(ins * u) { }

	//ORA   - "OR" Memory with Accumulator
	void ora(ins * u) {
		this->a |= this->cMem.read(u->address);
		this->sZero(this->a);
		this->sNegative(this->a);
	}

	//PHA   - Push Accumulator on Stack
	void pha(ins * u) { this->push_byte(this->a); }

	//PHP   - Push Processor Status on Stack
	void php(ins * u) { this->push_byte(this->enable() | 0x10); }

	//PLA   - Pull Accumulator from Stack
	void pla(ins * u) {
		this->a = this->pop_byte();
		this->sZero(this->a);
		this->sNegative(this->a);
	}

	//PLP   - Pull Processor Status from Stack
	void plp(ins * u) { this->set((this->pop_byte() & 0xEF) | 0x20); }

	//ROL   - Rotate One Bit Left (Memory or Accumulator)
	void rol(ins * u) {
		w8 hold = w8(this->carry);

		if (u->mode == modeEnum::sourceMode::acc) {
			this->carry = (this->a >> 7) & 1;
			this->a = (this->a << 7) | hold;
			this->sZero(this->a);
			this->sNegative(this->a);
		}

		else {
			w8 hold2 = this->cMem.read(u->address);
			this->carry = (this->a >> 7) & 1;
			hold2 <<= 1;
			hold2 |= hold;
			this->cMem.write(u->address, hold2);
			this->sZero(hold2);
			this->sNegative(hold2);
		}
	}

	//ROR   - Rotate One Bit Right (Memory or Accumulator)
	void ror(ins * u) {
		w8 hold = w8(this->carry);

		if (u->mode == modeEnum::sourceMode::acc) {
			this->carry = this->a & 1;
			this->a = (this->a >> 1) | (hold << 7);
			this->sZero(this->a);
			this->sNegative(this->a);
		}

		else {
			w8 hold2 = this->cMem.read(u->address);
			this->carry = hold & 1;
			hold2 = (hold2 >> 1) | (hold << 7);
			this->cMem.write(u->address, hold2);
			this->sZero(hold2);
			this->sNegative(hold2);
		}
	}

	//RTI   - Return from Interrupt
	void rti(ins * u) {
		this->set((this->pop_byte() & 0xEF) | 0x20);
		this->pc = this->pop_bytes();
	}

	//RTS   - Return from Subroutine
	void rts(ins * u) {
		this->pc = this->pop_bytes();
		this->pc++;
	}

	//SBC   - Subtract Memory from Accumulator with Borrow
	void sbc(ins * u) {
		w8 hold1 = this->cMem.read(u->address);
		w8 hold2 = this->a;
		w8 hold3 = w8(this->carry);

		this->a = hold2 - hold1 - (1 - hold3);
		this->sZero(this->a);
		this->sNegative(this->a);

		if (int(hold2) - int(hold1) - int(1 - hold3) >= 0) this->carry = true;
		else this->carry = false;

		if ((((hold2^hold1) & 0x80) != 0) && (((hold2^this->a) & 0x80) != 0)) this->overflow = true;
		else this->overflow = false;
	}

	//SEC   - Set Carry Flag
	void sec(ins * u) { this->carry = true; }

	//SED   - Set Decimal Mode
	void sed(ins * u) { this->decimal = true; }

	//SEI   - Set Interrupt Disable Status
	void sei(ins * u) { this->interupt = true; }

	//STA   - Store Accumulator in Memory
	void sta(ins * u) { this->cMem.write(u->address, this->a); }

	//STX   - Store Index X in Memory
	void stx(ins * u) { this->cMem.write(u->address, this->x); }

	//STY   - Store Index Y in Memory
	void sty(ins * u) { this->cMem.write(u->address, this->y); }

	//TAX   - Transfer Accumulator to Index X
	void tax(ins * u) {
		this->x = this->a;
		this->sZero(this->x);
		this->sNegative(this->x);
	}

	//TAY   - Transfer Accumulator to Index Y
	void tay(ins * u) {
		this->y = this->a;
		this->sZero(this->y);
		this->sNegative(this->y);
	}

	//TSX   - Transfer Stack Pointer to Index X
	void tsx(ins * u) {
		this->x = this->s;
		this->sZero(this->x);
		this->sNegative(this->x);
	}

	//TXA   - Transfer Index X to Accumulator
	void txa(ins * u) {
		this->a = this->x;
		this->sZero(this->a);
		this->sNegative(this->a);
	}

	//TXS   - Transfer Index X to Stack Pointer
	void txs(ins * u) { this->s = this->x; }

	//TYA   - Transfer Index Y to Accumulator
	void tya(ins * u) {
		this->a = this->y;
		this->sZero(this->a);
		this->sNegative(this->a);
	}

	//Unofficial Opcodes:

	//AHX
	void ahx(ins * u) { }

	//ALR
	void alr(ins * u) { }

	//ANC
	void anc(ins * u) { }

	//ARR
	void arr(ins * u) { }

	//AXS
	void axs(ins * u) { }

	//DCP
	void dcp(ins * u) { }

	//ISC
	void isc(ins * u) { }

	//KIL
	void kil(ins * u) { }

	//LAS
	void las(ins * u) { }

	//LAX
	void lax(ins * u) { }

	//RLA
	void rla(ins * u) { }

	//RRA
	void rra(ins * u) { }

	//SAX
	void sax(ins * u) { }

	//SHX
	void shx(ins * u) { }

	//SHY
	void shy(ins * u) { }

	//SLO
	void slo(ins * u) { }

	//SRE
	void sre(ins * u) { }

	//TAS
	void tas(ins * u) { }

	//XAA
	void xaa(ins * u) { }


	//Switch statement maps a ROM file prompt to a specific opcode
	void prompt(w8 hex, ins * u) {
		switch (hex) {
		case '0x00': brk(u); break;
		case '0x01': ora(u); break;
		case '0x02': kil(u); break;
		case '0x03': slo(u); break;
		case '0x04': nop(u); break;
		case '0x05': ora(u); break;
		case '0x06': asl(u); break;
		case '0x07': slo(u); break;
		case '0x08': php(u); break;
		case '0x09': ora(u); break;
		case '0x0A': asl(u); break;
		case '0x0B': anc(u); break;
		case '0x0C': nop(u); break;
		case '0x0D': ora(u); break;
		case '0x0E': asl(u); break;
		case '0x0F': slo(u); break;

		case '0x10': bpl(u); break;
		case '0x11': ora(u); break;
		case '0x12': kil(u); break;
		case '0x13': slo(u); break;
		case '0x14': nop(u); break;
		case '0x15': ora(u); break;
		case '0x16': asl(u); break;
		case '0x17': slo(u); break;
		case '0x18': clc(u); break;
		case '0x19': ora(u); break;
		case '0x1A': nop(u); break;
		case '0x1B': slo(u); break;
		case '0x1C': nop(u); break;
		case '0x1D': ora(u); break;
		case '0x1E': asl(u); break;
		case '0x1F': slo(u); break;

		case '0x20': jsr(u); break;
		case '0x21': and (u); break;
		case '0x22': kil(u); break;
		case '0x23': rla(u); break;
		case '0x24': bit(u); break;
		case '0x25': and (u); break;
		case '0x26': rol(u); break;
		case '0x27': rla(u); break;
		case '0x28': plp(u); break;
		case '0x29': and (u); break;
		case '0x2A': rol(u); break;
		case '0x2B': anc(u); break;
		case '0x2C': bit(u); break;
		case '0x2D': and (u); break;
		case '0x2E': rol(u); break;
		case '0x2F': rla(u); break;

		case '0x30': bmi(u); break;
		case '0x31': and (u); break;
		case '0x32': kil(u); break;
		case '0x33': rla(u); break;
		case '0x34': nop(u); break;
		case '0x35': and (u); break;
		case '0x36': rol(u); break;
		case '0x37': rla(u); break;
		case '0x38': sec(u); break;
		case '0x39': and (u); break;
		case '0x3A': nop(u); break;
		case '0x3B': rla(u); break;
		case '0x3C': nop(u); break;
		case '0x3D': and (u); break;
		case '0x3E': rol(u); break;
		case '0x3F': rla(u); break;

		case '0x40': rti(u); break;
		case '0x41': eor(u); break;
		case '0x42': kil(u); break;
		case '0x43': sre(u); break;
		case '0x44': nop(u); break;
		case '0x45': eor(u); break;
		case '0x46': lsr(u); break;
		case '0x47': sre(u); break;
		case '0x48': pha(u); break;
		case '0x49': eor(u); break;
		case '0x4A': lsr(u); break;
		case '0x4B': alr(u); break;
		case '0x4C': jmp(u); break;
		case '0x4D': eor(u); break;
		case '0x4E': lsr(u); break;
		case '0x4F': sre(u); break;

		case '0x50': bvc(u); break;
		case '0x51': eor(u); break;
		case '0x52': kil(u); break;
		case '0x53': sre(u); break;
		case '0x54': nop(u); break;
		case '0x55': eor(u); break;
		case '0x56': lsr(u); break;
		case '0x57': sre(u); break;
		case '0x58': cli(u); break;
		case '0x59': eor(u); break;
		case '0x5A': nop(u); break;
		case '0x5B': sre(u); break;
		case '0x5C': nop(u); break;
		case '0x5D': eor(u); break;
		case '0x5E': lsr(u); break;
		case '0x5F': sre(u); break;

		case '0x60': rts(u); break;
		case '0x61': adc(u); break;
		case '0x62': kil(u); break;
		case '0x63': rra(u); break;
		case '0x64': nop(u); break;
		case '0x65': adc(u); break;
		case '0x66': ror(u); break;
		case '0x67': rra(u); break;
		case '0x68': pla(u); break;
		case '0x69': adc(u); break;
		case '0x6A': ror(u); break;
		case '0x6B': arr(u); break;
		case '0x6C': jmp(u); break;
		case '0x6D': adc(u); break;
		case '0x6E': ror(u); break;
		case '0x6F': rra(u); break;

		case '0x70': bvs(u); break;
		case '0x71': adc(u); break;
		case '0x72': kil(u); break;
		case '0x73': rra(u); break;
		case '0x74': nop(u); break;
		case '0x75': adc(u); break;
		case '0x76': ror(u); break;
		case '0x77': rra(u); break;
		case '0x78': sei(u); break;
		case '0x79': adc(u); break;
		case '0x7A': nop(u); break;
		case '0x7B': rra(u); break;
		case '0x7C': nop(u); break;
		case '0x7D': adc(u); break;
		case '0x7E': ror(u); break;
		case '0x7F': rra(u); break;

		case '0x80': nop(u); break;
		case '0x81': sta(u); break;
		case '0x82': nop(u); break;
		case '0x83': sax(u); break;
		case '0x84': sty(u); break;
		case '0x85': sta(u); break;
		case '0x86': stx(u); break;
		case '0x87': sax(u); break;
		case '0x88': dey(u); break;
		case '0x89': nop(u); break;
		case '0x8A': txa(u); break;
		case '0x8B': xaa(u); break;
		case '0x8C': sty(u); break;
		case '0x8D': sta(u); break;
		case '0x8E': stx(u); break;
		case '0x8F': sax(u); break;

		case '0x90': bcc(u); break;
		case '0x91': sta(u); break;
		case '0x92': kil(u); break;
		case '0x93': ahx(u); break;
		case '0x94': sty(u); break;
		case '0x95': sta(u); break;
		case '0x96': stx(u); break;
		case '0x97': sax(u); break;
		case '0x98': tya(u); break;
		case '0x99': sta(u); break;
		case '0x9A': txs(u); break;
		case '0x9B': tas(u); break;
		case '0x9C': shy(u); break;
		case '0x9D': sta(u); break;
		case '0x9E': shx(u); break;
		case '0x9F': ahx(u); break;

		case '0xA0': ldy(u); break;
		case '0xA1': lda(u); break;
		case '0xA2': ldx(u); break;
		case '0xA3': lax(u); break;
		case '0xA4': ldy(u); break;
		case '0xA5': lda(u); break;
		case '0xA6': ldx(u); break;
		case '0xA7': lax(u); break;
		case '0xA8': tay(u); break;
		case '0xA9': lda(u); break;
		case '0xAA': tax(u); break;
		case '0xAB': lax(u); break;
		case '0xAC': ldy(u); break;
		case '0xAD': lda(u); break;
		case '0xAE': ldx(u); break;
		case '0xAF': lax(u); break;

		case '0xB0': bcs(u); break;
		case '0xB1': lda(u); break;
		case '0xB2': kil(u); break;
		case '0xB3': lax(u); break;
		case '0xB4': ldy(u); break;
		case '0xB5': lda(u); break;
		case '0xB6': ldx(u); break;
		case '0xB7': lax(u); break;
		case '0xB8': clv(u); break;
		case '0xB9': lda(u); break;
		case '0xBA': tsx(u); break;
		case '0xBB': las(u); break;
		case '0xBC': ldy(u); break;
		case '0xBD': lda(u); break;
		case '0xBE': ldx(u); break;
		case '0xBF': lax(u); break;

		case '0xC0': cpy(u); break;
		case '0xC1': cmp(u); break;
		case '0xC2': nop(u); break;
		case '0xC3': dcp(u); break;
		case '0xC4': cpy(u); break;
		case '0xC5': cmp(u); break;
		case '0xC6': dec(u); break;
		case '0xC7': dcp(u); break;
		case '0xC8': iny(u); break;
		case '0xC9': cmp(u); break;
		case '0xCA': dex(u); break;
		case '0xCB': axs(u); break;
		case '0xCC': cpy(u); break;
		case '0xCD': cmp(u); break;
		case '0xCE': dec(u); break;
		case '0xCF': dcp(u); break;

		case '0xD0': bne(u); break;
		case '0xD1': cmp(u); break;
		case '0xD2': kil(u); break;
		case '0xD3': dcp(u); break;
		case '0xD4': nop(u); break;
		case '0xD5': cmp(u); break;
		case '0xD6': dec(u); break;
		case '0xD7': dcp(u); break;
		case '0xD8': cld(u); break;
		case '0xD9': cmp(u); break;
		case '0xDA': nop(u); break;
		case '0xDB': dcp(u); break;
		case '0xDC': nop(u); break;
		case '0xDD': cmp(u); break;
		case '0xDE': dec(u); break;
		case '0xDF': dcp(u); break;

		case '0xE0': cpx(u); break;
		case '0xE1': sbc(u); break;
		case '0xE2': nop(u); break;
		case '0xE3': isc(u); break;
		case '0xE4': cpx(u); break;
		case '0xE5': sbc(u); break;
		case '0xE6': inc(u); break;
		case '0xE7': isc(u); break;
		case '0xE8': inx(u); break;
		case '0xE9': sbc(u); break;
		case '0xEA': nop(u); break;
		case '0xEB': sbc(u); break;
		case '0xEC': cpx(u); break;
		case '0xED': sbc(u); break;
		case '0xEE': inc(u); break;
		case '0xEF': isc(u); break;

		case '0xF0': beq(u); break;
		case '0xF1': sbc(u); break;
		case '0xF2': kil(u); break;
		case '0xF3': isc(u); break;
		case '0xF4': nop(u); break;
		case '0xF5': sbc(u); break;
		case '0xF6': inc(u); break;
		case '0xF7': isc(u); break;
		case '0xF8': sed(u); break;
		case '0xF9': sbc(u); break;
		case '0xFA': nop(u); break;
		case '0xFB': isc(u); break;
		case '0xFC': nop(u); break;
		case '0xFD': sbc(u); break;
		case '0xFE': inc(u); break;
		case '0xFF': isc(u); break;
		}
	}

};


#endif
