// $Id: RomDRAM.cc 12528 2012-05-17 17:36:10Z m9710797 $

#include "RomDRAM.hh"
#include "PanasonicMemory.hh"
#include "MSXMotherBoard.hh"
#include "XMLElement.hh"
#include "Rom.hh"
#include "serialize.hh"

namespace openmsx {

static unsigned calcBaseAddr(const DeviceConfig& config)
{
	int base = config.getChild("mem").getAttributeAsInt("base");
	int first = config.getChild("rom").getChildDataAsInt("firstblock");
	return first * 0x2000 - base;
}

RomDRAM::RomDRAM(const DeviceConfig& config, std::auto_ptr<Rom> rom)
	: MSXRom(config, rom)
	, panasonicMemory(getMotherBoard().getPanasonicMemory())
	, baseAddr(calcBaseAddr(config))
{
	// ignore result, only called to trigger 'missing rom' error early
	panasonicMemory.getRomBlock(baseAddr);
}

byte RomDRAM::readMem(word address, EmuTime::param /*time*/)
{
	return *RomDRAM::getReadCacheLine(address);
}

const byte* RomDRAM::getReadCacheLine(word address) const
{
	unsigned addr = address + baseAddr;
	return &panasonicMemory.getRomBlock(addr >> 13)[addr & 0x1FFF];
}

REGISTER_MSXDEVICE(RomDRAM, "RomDRAM");

} // namespace openmsx
