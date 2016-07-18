#ifndef GENERIC_DIGITAL_WRITE_H
#define GENERIC_DIGITAL_WRITE_H

#include <sensors/DigitalOnePinSensor.h>

class GenericDigitalWrite : public DigitalOnePinSensor {
	public:
		GenericDigitalWrite() { setMode(WRITE); }
		virtual ~GenericDigitalWrite() {;}
		
		virtual void init();
		virtual void write(float);

		virtual String toString();
};
#endif