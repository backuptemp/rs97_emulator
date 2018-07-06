// $Id: AY8910Periphery.hh 8352 2008-11-12 18:39:08Z m9710797 $

#ifndef AY8910PERIPHERY_HH
#define AY8910PERIPHERY_HH

#include "EmuTime.hh"
#include "openmsx.hh"

namespace openmsx {

/** Models the general purpose I/O ports of the AY8910.
  * The default implementation handles an empty periphery:
  * nothing is connected to the I/O ports.
  * This class can be overridden to connect peripherals.
  */
class AY8910Periphery
{
public:
	/** Reads the state of the peripheral on port A.
	  * Since the AY8910 doesn't have control lines for the I/O ports,
	  * a peripheral is not aware that it is read, which means that
	  * "peek" and "read" are equivalent.
	  * @param time The moment in time the peripheral's state is read.
	  *   On subsequent calls, the time will always be increasing.
	  * @return the value read; unconnected bits should be 1
	  */
	virtual byte readA(EmuTime::param time);

	/** Similar to readA, but reads port B. */
	virtual byte readB(EmuTime::param time);

	/** Writes to the peripheral on port A.
	  * @param value The value to write.
	  * @param time The moment in time the value is written.
	  *   On subsequent calls, the time will always be increasing.
	  */
	virtual void writeA(byte value, EmuTime::param time);

	/** Similar to writeA, but writes port B. */
	virtual void writeB(byte value, EmuTime::param time);

protected:
	AY8910Periphery();
	virtual ~AY8910Periphery();
};

} // namespace openmsx

#endif // AY8910PERIPHERY_HH
