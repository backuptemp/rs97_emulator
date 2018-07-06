// $Id: RomSuperLodeRunner.cc 12528 2012-05-17 17:36:10Z m9710797 $

#include "RomSuperLodeRunner.hh"
#include "MSXCPUInterface.hh"
#include "Rom.hh"
#include "serialize.hh"
#include <cassert>

namespace openmsx {

RomSuperLodeRunner::RomSuperLodeRunner(
		const DeviceConfig& config, std::auto_ptr<Rom> rom)
	: Rom16kBBlocks(config, rom)
{
	getCPUInterface().registerGlobalWrite(*this, 0x0000);
	reset(EmuTime::dummy());
}

RomSuperLodeRunner::~RomSuperLodeRunner()
{
	getCPUInterface().unregisterGlobalWrite(*this, 0x0000);
}

void RomSuperLodeRunner::reset(EmuTime::param /*time*/)
{
	setUnmapped(0);
	setUnmapped(1);
	setRom(2, 0);
	setUnmapped(3);
}

void RomSuperLodeRunner::globalWrite(word address, byte value, EmuTime::param /*time*/)
{
	assert(address == 0);
	(void)address;
	setRom(2, value);
}

REGISTER_MSXDEVICE(RomSuperLodeRunner, "RomSuperLodeRunner");

} // namespace openmsx