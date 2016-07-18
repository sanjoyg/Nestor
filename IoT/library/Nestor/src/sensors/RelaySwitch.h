#ifndef RELAY_SWITCH_H
#define RELAY_SWITCH_H

#include <sensors/GenericDigitalWrite.h>

class RelaySwitch : public GenericDigitalWrite {
	public:
		RelaySwitch() { setMode(WRITE); }
		virtual ~RelaySwitch() {;}
		
		virtual void init();
		virtual void write(float);
};

#endif