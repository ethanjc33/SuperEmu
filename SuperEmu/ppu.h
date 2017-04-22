//Ethan Clark <ejc49@zips.uakron.edu>
//Picture Processing Unit

#ifndef PPU_HPP
#define PPU_HPP

#include "instruction.h"
#include "mem.h"

struct sys;

#include "nes.h"

//Components of the PPU
struct PPU {

	//Note: The parts listed out here were much less obvious than the CPU
	//i.e. therefore subject to change if more or less pieces are necessary

	//Registers
	w16 tv;								//Temporary VRAM
	w16 sv;								//Saved VRAM
	w8  fs;								//Fine Scroll
	w8  gr;								//General Purpose

	//Flags
	bool parity;						//Parity flag - even, false & odd, true
	bool writer;						//Determines if write (i.e. during scroll) is in progress
	bool past;							//NMI signal - last / previous
	bool present;						//NMI signal - current / occurring
	bool future;						//NMI signal - coming out
	w8 stasis;							//NMI signal - delay timer

	//Counters & Such
	w8  buffer;							//When reading from $2007
	w64 frame;							//Frame Counter
	int scans;							//Scanline Counter (determines action)
	int cycles;							//Clock Cycle Counter

	//Storage Mechanisms
	w8 palette[0x20];					//Palette State
	w8 oam[0x100];						//OAM Data
	w8 names[0x800];					//Name Table
	ctrlP rolp;							//Access to boolean bits required by [$2000]
	maskP masp;							//Access to boolean bits required by [$2001]
	statP stap;							//Access to boolean bits required by [$2002]

	//Memory Representation
	picture pMem;						//PPU Memory
	w8 data;							//When reading from $2003

	//CPU
	CPU cc;								//For updating cycles and updating flags during processes


	//Constructors & Destructor
	PPU() = default;
	~PPU() { delete this; }


	//Memory mapped registers to the CPU
	//Reads -> return byte, Writes -> take byte

	//NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B),
	//sprite tile select (S), increment mode (I), nametable select (NN) [$2000]
	void PPUctrl(w8 out) {
		this->updateNMI();
		this->rolp.bUpper = (out >> 2) & 1;
		this->rolp.bSprites = (out >> 3) & 1;
		this->rolp.bNames = out & 3;
		this->rolp.bBack = (out >> 4) & 1;
		this->rolp.bMS = (out >> 6) & 1;
		this->rolp.bSize = (out >> 5) & 1;
		this->future = (((out >> 7) & 1) == 1);
		this->tv = ((this->tv & 0xF3FF) | ((w16(out) & 3) << 0x0A));
	}

	//color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M),
	//background left column enable (m), greyscale (G) [$2001]
	void PPUmask(w8 out) {
		this->masp.bBack = (out >> 3) & 1;
		this->masp.bSprites = (out >> 4) & 1;
		this->masp.green = (out >> 6) & 1;
		this->masp.blue = (out >> 7) & 1;
		this->masp.red = (out >> 5) & 1;
		this->masp.bLBack = (out >> 1) & 1;
		this->masp.bLSprite = (out >> 2) & 1;
		this->masp.bScale = out & 1;
	}

	//vblank (V), sprite 0 hit (S), sprite overflow (O), read resets write pair for $2005/2006 [$2002]
	w8 PPUstatus() {
		w8 hold = (this->gr) & 0x1F;
		hold |= this->stap.bOverflow << 5;
		hold |= this->stap.bZero << 6;

		if (this->present == true) hold |= (1 << 7);
		this->present = false;
		this->writer = false;
		this->updateNMI();

		return hold;
	}

	//OAM write address [$2003]
	void OAMaddr(w8 out) { this->data = out; }

	//OAM data read/write [$2004]
	w8 OAMrData() { return this->oam[this->data]; }

	void OAMwData(w8 out) {
		this->oam[this->data] = out;
		this->data++;
	}

	//fine scroll position(two writes : X, Y) [$2005]
	void PPUscroll(w8 out) {

		if (this->writer == true) {
			this->writer = false;
			this->tv = ((this->tv & 0x8FFF) | ((w16(out) & 0x07) << 12));
			this->tv = ((this->tv & 0xFC1F) | ((w16(out) & 0xF8) << 2));
		}

		else {
			this->writer = true;
			this->fs = out & 7;
			this->tv = ((this->tv & 0xFFE0) | (w16(out) >> 3));
		}
	}

	//PPU write address (two writes: MSB, LSB) [$2006]
	void PPUaddr(w8 out) {

		if (this->writer == true) {
			this->writer = false;
			this->tv = ((this->tv & 0xFF00) | w16(out));
			this->sv = this->tv;
		}

		else {
			this->writer = true;
			this->tv = ((this->tv & 0x80FF) | ((w16(out) & 0x3F) << 8));
		}
	}

	//PPU data read/write, handle buffers and addresses [$2007]
	w8 PPUrData() {
		w8 hold1 = this->pMem.read(this->sv);

		if ((this->sv % 0x4000) < 0x3F00) {
			w8 hold2;
			hold2 = this->buffer;
			this->buffer = hold1;
			hold1 = hold2;
		}

		else this->buffer = this->pMem.read(this->sv - 0x1000);

		if (this->rolp.bUpper == 0) this->sv += 1;
		else this->sv += 0x20;

		return hold1;
	}

	void PPUwData(w8 out) {
		this->pMem.write(this->sv, out);
		if (this->rolp.bUpper == 0) this->sv += 1;
		else this->sv += 0x20;
	}

	//OAM DMA high address [$4014]
	void OAMdma(w8 out) {
		w16 hold = (w16(out) << 8);

		for (int i = 0; i < 0x100; ++i) {
			this->oam[this->data] = this->cc.cMem.read(hold);
			++hold;
			this->data++;
		}

		//Note: 513 (along with most of these other values) are based off of documentation, not testing
		this->cc.staller += 513;

		if ((this->cc.cycles % 2) == 1) this->cc.staller++;
	}

	PPU * createPPU(sys * s);

	//Process a cycle in the PPU
	void execute() {
		//TODO: This function (should interact with rendering functions if planned correctly)

	}


	//Switches to the correct register to write to based on current state (read/write)
	void regWPrompt(w8 out, w16 adr) {
		this->gr = out;
		switch (adr) {
		case (0x2000): this->PPUctrl(out);		break;
		case (0x2001): this->PPUmask(out);		break;
		case (0x2003): this->OAMaddr(out);		break;
		case (0x2004): this->OAMwData(out);		break;
		case (0x2005): this->PPUscroll(out);	break;
		case (0x2006): this->PPUaddr(out);		break;
		case (0x2007): this->PPUwData(out);		break;
		case (0x4014): this->OAMdma(out);		break;
		}
	}

	w8 regRPrompt(w16 adr) {
		switch (adr) {
		case (0x2002): return this->PPUstatus();	break;
		case (0x2004): return this->OAMrData();		break;
		case (0x2007): return this->PPUrData();		break;
		default: return 0; break;
		}
	}


	//Read current palette state
	w8 paletteRPrompt(w16 adr) {
		if ((adr >= 0x10) && ((adr % 4) == 0)) adr -= 0x10;
		return this->palette[adr];
	}

	//Write current palette state
	void paletteWPrompt(w16 adr, w8 out) {
		if ((adr >= 16) && ((adr % 4) == 0)) adr -= 16;
		this->palette[adr] = out;
	}


	void updateNMI() {
		//Note: 14 is an arbitrary number here, setting for delays which could be changed
		if (!(this->past) && (this->present && this->future)) this->stasis = 14;
		this->past = (this->present && this->future);
	}

	
	void cyc() {

	}


	//TODO: Image rendering (figure out how...)

};

#endif
