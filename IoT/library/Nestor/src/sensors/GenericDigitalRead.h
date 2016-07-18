#ifndef GENERIC_DIGITAL_READ_H
#define GENERIC_DIGITAL_READ_H

#include <sensors/DigitalOnePinSensor.h>

class GenericDigitalRead : public DigitalOnePinSensor {
	public:
		GenericDigitalRead() { setMode(READ); }
		virtual ~GenericDigitalRead() {;}
		
		virtual void init();
		virtual float read();

		virtual String toString();		
};
#endif