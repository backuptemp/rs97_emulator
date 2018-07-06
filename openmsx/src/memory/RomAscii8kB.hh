// $Id: RomAscii8kB.hh 12527 2012-05-17 17:34:11Z m9710797 $

#ifndef ROMASCII8KB_HH
#define ROMASCII8KB_HH

#include "RomBlocks.hh"

namespace openmsx {

class RomAscii8kB : public Rom8kBBlocks
{
public:
	RomAscii8kB(const DeviceConfig& config, std::auto_ptr<Rom> rom);

	virtual void reset(EmuTime::param time);
	virtual void writeMem(word address, byte value, EmuTime::param time);
	virtual byte* getWriteCacheLine(word address) const;
};

} // namespace openmsx

#endif