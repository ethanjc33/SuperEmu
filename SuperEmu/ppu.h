//Ethan Clark <ejc49@zips.uakron.edu>
//Picture Processing Unit

#ifndef PPU_HPP
#define PPU_HPP

#include <utility>

#include "CImg.h"
#include "instruction.h"
#include "mem.h"

struct sys;

#include "nes.h"


//Palette colors (based on hex code color values - only 64 colors, even fewer unique ones)
//You can also only show 25 on screen at a time - we have come a long ways!
//TODO: Incorporate color palette into CImg.h functionality
const w64 paletteColors[64] = {
	0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400,
	0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,
	0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10,
	0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000,
	0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
	0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000,
	0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
	0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000
};


//PPU Register Packages - contains elements necessary for those functions defined in ppu.h
//Most will be used as boolean (b) flags, necessary to be 8-Bit values since we shift bits

//$2000 (PPUCTRL)
struct ctrlP {
	w8 bNames;
	w8 bUpper;
	w8 bSprites;
	w8 bBack;
	w8 bSize;
	w8 bMS;
};

//$2001 (PPUMASK)
struct maskP {
	w8 bScale;
	w8 bLBack;
	w8 bLSprite;
	w8 bBack;
	w8 bSprites;
	w8 red;
	w8 green;
	w8 blue;
};

//$2002 (PPUSTATUS)
struct statP {
	w8 bZero;
	w8 bOverflow;
};


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

	//Rendering Members - Sprites
	int pixels;							//Number of sprites showing
	w32 patterns[8];					//Sprite patterns
	w8  positions[8];					//Sprite positions
	w8  priorities[8];					//Sprite priorities
	w8  indexes[8];						//Sprite indexes

	//Rendering Members - Tiles
	w64 tiles;							//Tile information
	w8 lo;								//Low byte
	w8 hi;								//High byte
	w8 at;								//Attributes
	w8 nt;								//Name table

	//Storage Mechanisms
	w8 palette[0x20];					//Palette State
	w8 oam[0x100];						//OAM Data
	w8 names[0x800];					//Name Table
	ctrlP rolp;							//Access to boolean bits required by [$2000]
	maskP masp;							//Access to boolean bits required by [$2001]
	statP stap;							//Access to boolean bits required by [$2002]

	//Memory Representation
	picture * pMem;						//PPU Memory
	w8 data;							//When reading from $2003

	//CPU
	CPU * cc;							//For updating cycles and updating flags during processes

	//Front & Back images - made available through CImg.h dependency
	cimg_library::CImg<w8> * front;
	cimg_library::CImg<w8> * back;


	//Constructors & Destructor
	PPU();
	PPU(sys * s);
	~PPU();


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

	//Fine scroll position(two writes : X, Y) [$2005]
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
		w8 hold1 = this->pMem->read(this->sv);

		if ((this->sv % 0x4000) < 0x3F00) {
			w8 hold2;
			hold2 = this->buffer;
			this->buffer = hold1;
			hold1 = hold2;
		}

		else this->buffer = this->pMem->read(this->sv - 0x1000);

		if (this->rolp.bUpper == 0) this->sv += 1;
		else this->sv += 0x20;

		return hold1;
	}

	void PPUwData(w8 out) {
		this->pMem->write(this->sv, out);
		if (this->rolp.bUpper == 0) this->sv += 1;
		else this->sv += 0x20;
	}

	//OAM DMA high address [$4014]
	void OAMdma(w8 out) {
		w16 hold = (w16(out) << 8);

		for (int i = 0; i < 0x100; ++i) {
			this->oam[this->data] = this->cc->cMem->read(hold);
			++hold;
			this->data++;
		}

		//Note: 513 (along with most of these other values) are based off of documentation, not testing
		this->cc->staller += 513;

		if ((this->cc->cycles % 2) == 1) this->cc->staller++;
	}

	//Process a cycle in the PPU
	void execute() {

		//NMI Check
		if (this->stasis > 0) {
			this->stasis--;
			if (this->present == true && this->future == true && this->stasis == 0) this->cc->nmi();
		}

		//Flush if necessary
		if ((this->masp.bBack != 0 || this->masp.bSprites != 0) &&
			(this->parity == true && this->scans == 261 && this->cycles == 339)) {
			this->scans = 0;
			this->cycles = 0;
			this->frame++;
			this->parity ^= 1;
		}

		this->cycles++;
		if (this->cycles > 340) {
			this->cycles = 0;
			this->scans++;

			if (this->scans > 261) {
				this->scans = 0;
				this->parity ^= 1;
				this->frame++;
			}
		}

		//Render background and sprites based on current scanline states
		//Diagrams and Bounds: https://wiki.nesdev.com/w/index.php/PPU_rendering

		//Determine if rendering is enabled
		bool rEnable = ((this->masp.bBack != 0) && (this->masp.bSprites != 0));

		//Determine if scans has reached a certain threshold
		bool scansHold = (this->scans == 261);
		//Determine if scanlines are visible
		bool visibleScan = (this->scans <= 239);
		//Determine if a line needs to be rendered
		bool lineTest = (visibleScan || scansHold);

		//Determine if the tiles on the next scan need to be fetched / loaded
		bool tilesHold = (this->cycles > 320 && this->cycles < 337);
		//Determine if cycles is in a certain threshold
		bool visibleTile = (this->cycles > 0 && this->cycles < 257);
		//Determine if a fetch must occur
		bool tileTest = (tilesHold || visibleTile);

		if (rEnable == true) {
			//Depending on boolean values, switch off to rendering functions
			if (tileTest && visibleScan) this->renderSprite();

			//Switch off here - cases refer to rendering members and formulas to find them
			if (lineTest && tileTest) {
				this->tiles <<= 4;

				w32 hold = 0;
				w16 save = this->sv;
				w16 adr = 0x23C0 | (save & 0x0C00) | ((save >> 4) & 0x38) | ((save >> 2) & 0x07);
				w16 c = hold + (w16(this->rolp.bBack) * 4096) + (w16(this->nt) * 16);
				w16 d = hold + (w16(this->rolp.bBack) * 4096) + (w16(this->nt) * 16);
				w16 res = 0x2000 | (this->sv & 0x0FFF);
				w8 x, y;

				switch (this->cycles % 8) {
				case (0):
					for (int i = 0; i < 8; ++i) {
						x = (this->lo & 128) >> 7;
						y = (this->hi & 128) >> 6;
						this->lo <<= 1;
						this->hi <<= 1;
						hold <<= 4;
						hold |= w32(this->at | x | y);
					}
					this->tiles |= w64(hold);
				case (1): this->nt = this->regRPrompt(res);
				case (3):
					this->at = (((this->regRPrompt(adr) >> (((save >> 4) & 4) | (save & 2))) & 3) << 2);
				case (5): this->lo = this->regRPrompt(c);
				case (7): this->lo = this->regRPrompt(8 + d);
				}
			}

			if ((scansHold == true) && ((this->cycles >= 280) && (this->cycles < 305))) {
				this->sv = (this->sv & 0x841F) | (this->tv & 0x7BE0);
			}

			//Vertical shifts

			if (lineTest == true) {
				if (this->cycles == 257) this->sv = (this->sv & 0xFBE0) | (this->tv & 0x041F);

				else if (this->cycles == 256) {
					if ((this->sv & 0x7000) != 0x7000) this->sv += 0x1000;
					else {
						this->sv &= 0x8FFF;
						w8 a = (this->sv & 0x03E0) >> 5;

						if (a == 29) {
							a = 0;
							this->sv ^= 0x800;
						}

						else if (a == 31) a = 0;
						else ++a;

						this->sv &= 0xFC1F;
						this->sv |= (a << 5);
					}
				}

				else if ((tileTest == true) && ((this->cycles % 8) == 0)) {

					if ((this->sv & 0x1F) == 31) {
						this->sv &= 0xFFE0;
						this->sv ^= 0x400;
					}

					else this->sv++;
				}
			}
		}

			//Establish sprite information based on basic sprite rules

			if ((rEnable == true) && (this->cycles == 257)) {
				if (visibleScan == false) this->pixels = 0;
				else {
					int b, n = 0;
					if (this->rolp.bSize == 0) b = 8;
					else b = 16;

					for (int i = 0; i < 64; ++i) {
						int place = this->scans - int(this->oam[i * 4]);
						//Skip if necessary:
						if ((place >= b) || (place < 0)) continue;

						//NES can only hold 8 sprites on a line at a time

						if (n < 8) {
							this->indexes[n] = w8(i);
							this->patterns[n] = this->renderPattern(i, place);
							this->positions[n] = this->oam[(i * 4) + 3];
							this->priorities[n] = ((this->oam[(i * 4) + 2]) >> 5) & 1;
						}
						++n;
					}

					if (n >= 9) { this->stap.bOverflow = 1; n = 8; }
					this->pixels = n;
				}
			}

			//NMI updates

			if ((this->cycles == 1) && (this->scans == 241)) {
				std::swap(this->front, this->back);
				this->present = true;
				this->updateNMI();
			}

			if ((this->cycles == 1) && (scansHold == true)) {
				this->present = false;
				this->updateNMI();
				this->stap.bOverflow = 0;
				this->stap.bZero = 0;
			}
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


	//Rendering Functions

	//Renders paired pixel patterns to the background image
	void renderSprite() {

		int p = this->scans;
		int q = this->cycles - 1;
		w8 bg, pos, pix, color;			//Background pattern, sprite positions, sprite rep, color

		if (this->masp.bBack == 0) bg = 0;
		else {
			w32 hold = w32(tiles >> 32);
			hold >>= ((7 - this->fs) * 4);
			bg = w8(hold & 15);
		}

		pos = 0;
		pix = 0;

		if (this->masp.bSprites != 0) {
			for (int i = 0; i < this->pixels; ++i) {
				int temp = (this->cycles - 1) - int(this->positions[i]);
				//Skip if necessary:
				if (temp < 0 || temp >= 8) continue;
				temp = 7 - temp;
				w8 color = w8((this->patterns[i] >> w8(4 * temp)) & 15);
				//Skip if necessary:
				if ((color % 4) == 0) continue;
				pos = w8(i);
				pix = color;
				break;
			}
		}

		if (q <= 7) {
			if (this->masp.bLSprite == 0) pix = 0;
			if (this->masp.bLBack == 0) bg = 0;
		}

		//Testing Sequence to discover color

		bool hold1 = ((bg % 4) != 0);
		bool hold2 = ((pix % 4) != 0);

		if (!hold1 && !hold2) color = 0;
		else if (hold1 && !hold2) color = bg;
		else if (!hold1 && hold2) color = pix | 16;
		else {
			if ((this->indexes[pos] == 0) && (this->fs <= 254)) this->stap.bZero = 1;
			if (this->priorities[pos] == 0) color = pix | 16;
			else color = bg;
		}

		//Read from palette
		w32 extract = this->palette[this->paletteRPrompt(w16(color)) % 64];

		//Fill background image with current data
		this->back->fill(q, p, extract);

	}
	

	//Render patterns
	w32 renderPattern(int a, int b) {

		//For usage throughout:
		w8 hold1 = this->oam[(a * 4) + 1];
		w8 hold2 = this->oam[(a * 4) + 2];
		w16 adr;							//Address - we'll calculate it now
		w32 ret = 0;						//Return value - we'll calculate it later

		//Calculate Address through bit manipulations, sign extensions
		if (this->rolp.bSize == 0) {
			if ((hold2 & 0x80) == 0x80) b = 7 - b;
			adr = (w16(this->rolp.bSprites) * 0x1000) + (w16(hold1) * 0x10) + w16(b);
		}
		else {
			if ((hold2 & 0x80) == 0x80) b = 15 - b;
			w16 temp = w16(hold1 & 1);
			hold1 &= 0xFE;
			if (b >= 8) { ++hold1; b -= 8; }
			adr = (temp * 0x1000) + (w16(hold1) * 0x10) + w16(b);
		}

		//Record high and low bytes using calculated address
		w8 lb = this->pMem->read(adr);
		w8 hb = this->pMem->read(8 + adr);

		//Track these during loop
		w8 temp = (hold2 & 3) << 2;
		w8 pattern1, pattern2;

		//Loop through to calculate ret - note that these calculations were provided
		for (int i = 0; i < 8; ++i) {

			if ((hold2 & 0x40) == 0x40) {
				pattern1 = lb & 1;
				pattern2 = hb & 1;
				pattern2 <<= 1;
				lb >>= 1;
				hb >>= 1;
			}

			else {
				pattern1 = lb & 128;
				pattern1 >>= 7;
				pattern2 = hb & 128;
				pattern2 >>= 6;
				lb <<= 1;
				hb <<= 1;
			}

			ret <<= 4;
			ret |= w32(temp | pattern1 | pattern2);
		}

		return ret;
	}


};

#endif
