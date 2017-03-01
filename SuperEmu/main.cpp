//Ethan Clark <ejc49@zips.uakron.edu>

/*The program SuperEmu serves as an emulator; that is, a piece of software that is designed to
behave like another system. In this case, we will be emulating the hardware seen in the NES.
Citation: https://nesdev.com/ - huge help when figuring specific values and representations.*/

#include <string>
#include <vector>
#include <cmath>

typedef uint_least8_t w8;		//Certain elements (i.e. registers) are all a byte wide
typedef uint_least16_t w16;		//Certain elements (i.e. program counter) are two bytes wide
typedef uint_least64_t w64;		//Certain elements (i.e. clock cycle counter) require more bytes

//Instruction Package - Allows for simpler opcode functionality / Saves space
struct ins {
	w16 address;				//Address
	w16 procou;					//Program Counter
	w8  mode;					//Mode
};

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

//Picture Processing Unit
struct PPU {
	PPU() = default;
	virtual ~PPU() = default;

	void cyc() {

	}
};


//Audio Processing Unit
struct APU {
	APU() = default;
	virtual ~APU() = default;

	void cyc() {

	}
};

//Memory Representation
struct mem {
	mem() = default;
	virtual ~mem() = default;

	//Read & Write Functions for the CPU, PPU, and APU each
	/*With the way inheritance is done here, it would make sense to make these functions
	  virtual and only have one algorithm that changes based on the type of pointer
	  using it. Not sure if this is possible or not here, and may not make a huge impact
	  on efficency, mainly impressive in saving lines of code...?*/

	w8 readC(w16 adr) {

	}

	w8 writeC(w16 adr, w8 out) {

	}

	w8 readP(w16 adr) {

	}

	w8 writeP(w16 adr, w8 out) {

	}

	w8 readA(w16 adr) {

	}

	w8 writeA(w16 adr, w8 out) {

	}
};

//CPU Representation in Memory
struct central : mem {
	mem * cm;


};

//PPU Representation in Memory
struct picture : mem {
	mem * pm;

};

//APU Representation in Memory
struct audio : mem {
	mem * am;

};


//ROM File Game Pack Representation
struct cart {

};


//Input & Output Representation
struct con {

};

//Represents the NES hardware
struct system {
	CPU cu;
	PPU pu;
	APU au;
	cart ridge;
	con one;
	con two;
	mem ram;

	//Updates the clock rate of the system - CPU, PPU, and APU all run simulataneously in this implementation
	/*virtual void clock() {
	//Picture Processing Unit - Runs 3x as fast as CPU
	for (int i = 0; i < 3; ++i) cu.cyc();
	//Audio Processing Unit - Runs 1x as fast as CPU
	for (int i = 0; i < 1; ++i) au.cyc();
	}*/
};

//Representation & Functionality for every opcode
struct ops : system {

	reg * cup;

	//Opcode Functions, mapped via the switch statement
	//TODO: Map functions via function table to achieve constant time

	//ADC   - Add Memory to Accumulator with Carry
	void adc(ins * u) {

	}


	//AND   - "AND" Memory with Accumulator
	void and(ins * u) {

	}


	//ASL   - Shift Left One Bit (Memory or Accumulator)
	void asl(ins * u) {

	}

	//BCC   - Branch on Carry Clear
	void bcc(ins * u) {
		if (cup->carry == false) {
			cup->pc = u->address;
			cup->branch(u);
		}
	}

	//BCS   - Branch on Carry Set
	void bcs(ins * u) {
		if (cup->carry == true) {
			cup->pc = u->address;
			cup->branch(u);
		}
	}

	//BEQ   - Branch on Result Zero
	void beq(ins * u) {
		if (cup->zero == true) {
			cup->pc = u->address;
			cup->branch(u);
		}
	}

	//BIT   - Test Bits in Memory with Accumulator
	void bit(ins * u) {

	}

	//BMI   - Branch on Result Minus
	void bmi(ins * u) {
		if (cup->negative == true) {
			cup->pc = u->address;
			cup->branch(u);
		}
	}

	//BNE   - Branch on Result not Zero
	void bne(ins * u) {
		if (cup->zero == false) {
			cup->pc = u->address;
			cup->branch(u);
		}
	}

	//BPL   - Branch on Result Plus
	void bpl(ins * u) {
		if (cup->negative == false) {
			cup->pc = u->address;
			cup->branch(u);
		}
	}

	//BRK   - Force Break
	void brk(ins * u) {

	}

	//BVC   - Branch on Overflow Clear
	void bvc(ins * u) {
		if (cup->overflow == false) {
			cup->pc = u->address;
			cup->branch(u);
		}
	}

	//BVS   - Branch on Overflow Set
	void bvs(ins * u) {
		if (cup->overflow == true) {
			cup->pc = u->address;
			cup->branch(u);
		}
	}

	//CLC   - Clear Carry Flag
	void clc(ins * u) { cup->carry = false; }

	//CLD   - Clear Decimal Mode
	void cld(ins * u) { cup->decimal = false; }

	//CLI   - Clear interrupt Disable Bit
	void cli(ins * u) { cup->decimal = false; }

	//CLV   - Clear Overflow Flag
	void clv(ins * u) { cup->overflow = false; }

	//CMP   - Compare Memory and Accumulator
	void cmp(ins * u) {

	}

	//CPX   - Compare Memory and Index X
	void cpx(ins * u) {

	}

	//CPY   - Compare Memory and Index Y
	void cpy(ins * u) {

	}

	//DEC   - Decrement Memory by One
	void dec(ins * u) {

	}

	//DEX   - Decrement Index X by One
	void dex(ins * u) {
		cup->x--;
		cup->sZero(cup->x);
		cup->sNegative(cup->x);
	}

	//DEY   - Decrement Index Y by One
	void dey(ins * u) {
		cup->y--;
		cup->sZero(cup->y);
		cup->sNegative(cup->y);
	}


	//EOR   - "Exclusive-Or" Memory with Accumulator
	void eor(ins * u) {

	}

	//INC   - Increment Memory by One
	void inc(ins * u) {

	}

	//INX   - Increment Index X by One
	void inx(ins * u) {
		cup->x++;
		cup->sZero(cup->x);
		cup->sNegative(cup->x);
	}

	//INY   - Increment Index Y by One
	void iny(ins * u) {
		cup->y++;
		cup->sZero(cup->y);
		cup->sNegative(cup->y);
	}

	//JMP   - Jump to New Location
	void jmp(ins * u) { cup->pc = u->address; }

	//JSR   - Jump to New Location Saving Return Address
	void jsr(ins * u) {

	}

	//LDA   - Load Accumulator with Memory
	void lda(ins * u) {

	}

	//LDX   - Load Index X with Memory
	void ldx(ins * u) {

	}

	//LDY   - Load Index Y with Memory
	void ldy(ins * u) {

	}

	//LSR   - Shift Right One Bit(Memory or Accumulator)
	void lsr(ins * u) {

	}

	//NOP   - No Operation
	void nop(ins * u) { }

	//ORA   - "OR" Memory with Accumulator
	void ora(ins * u) {

	}

	//PHA   - Push Accumulator on Stack
	void pha(ins * u) {

	}

	//PHP   - Push Processor Status on Stack
	void php(ins * u) {

	}

	//PLA   - Pull Accumulator from Stack
	void pla(ins * u) {

	}

	//PLP   - Pull Processor Status from Stack
	void plp(ins * u) {

	}

	//ROL   - Rotate One Bit Left (Memory or Accumulator)
	void rol(ins * u) {

	}

	//ROR   - Rotate One Bit Right (Memory or Accumulator)
	void ror(ins * u) {

	}

	//RTI   - Return from Interrupt
	void rti(ins * u) {

	}

	//RTS   - Return from Subroutine
	void rts(ins * u) {

	}

	//SBC   - Subtract Memory from Accumulator with Borrow
	void sbc(ins * u) {

	}

	//SEC   - Set Carry Flag
	void sec(ins * u) { cup->carry = true; }

	//SED   - Set Decimal Mode
	void sed(ins * u) { cup->decimal = true; }

	//SEI   - Set Interrupt Disable Status
	void sei(ins * u) { cup->interupt = true; }

	//STA   - Store Accumulator in Memory
	void sta(ins * u) {

	}

	//STX   - Store Index X in Memory
	void stx(ins * u) {

	}

	//STY   - Store Index Y in Memory
	void sty(ins * u) {

	}

	//TAX   - Transfer Accumulator to Index X
	void tax(ins * u) {
		cup->x = cup->a;
		cup->sZero(cup->x);
		cup->sNegative(cup->x);
	}

	//TAY   - Transfer Accumulator to Index Y
	void tay(ins * u) {
		cup->y = cup->a;
		cup->sZero(cup->y);
		cup->sNegative(cup->y);
	}

	//TSX   - Transfer Stack Pointer to Index X
	void tsx(ins * u) {
		cup->x = cup->s;
		cup->sZero(cup->x);
		cup->sNegative(cup->x);
	}

	//TXA   - Transfer Index X to Accumulator
	void txa(ins * u) {
		cup->a = cup->x;
		cup->sZero(cup->a);
		cup->sNegative(cup->a);
	}

	//TXS   - Transfer Index X to Stack Pointer
	void txs(ins * u) { cup->s = cup->x; }

	//TYA   - Transfer Index Y to Accumulator
	void tya(ins * u) {
		cup->a = cup->y;
		cup->sZero(cup->a);
		cup->sNegative(cup->a);
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


	//Switch statement maps a ROM file prompt to a specific operation
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


//Main Application
int main() {

	//TODO: Load the ROM file, start system to operate upon it forever

	return 0;
}
