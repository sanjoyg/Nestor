#ifndef GENERIC_DIGITAL_READWRITE_H
#define GENERIC_DIGITAL_READWRITE_H

#include <sensors/RelaySwitch.h>
#include <core/nestorads.h>

class RelaySwitchWithRead : public RelaySwitch {
	public:
		RelaySwitchWithRead() { setMode(READWRITE); }
		virtual ~RelaySwitchWithRead();

		virtual float read();
		virtual void write(float value);

		virtual bool validateConfig();
		
		virtual bool isADCOn() { return _ads->isOn(); }

		virtual String toString();		
	protected:
		virtual bool setMember(int index, char *value);

		NestorADS *_ads = NULL;
};
#endif