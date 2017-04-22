//Ethan Clark <ejc49@zips.uakron.edu>

#include "maps.h"
#include "nes.h"

//Creates a new memory mapper based on ROM being used
void * maps::createMaps(sys * s) {
	cart r = s->ridge;

	//The cartridge's memory map ID will be:
	//0 - Mapper 2 (actually none from what the documentation suggests but others do this so...?)
	//1 - Mapper 1, 2 - Mapper 2, and so on

	switch (r.map) {
	case '0': return (m2 = new maps2(r)); break;
	case '1': return (m1 = new maps1(&r));
	case '2': return (m2 = new maps2(r)); break;
	case '3': return m3.createM3(r); break;
	case '4': return m4.createM4(r); break;
	case '7': return m7.createM7(r); break;
	default: return nullptr; break;
	}

}
