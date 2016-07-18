#ifndef TWO_WAY_SWITCH_H
#define TWO_WAY_SWITCH_H

#include <sensors/RelaySwitchWithRead.h>

class TwoWaySwitch : public RelaySwitchWithRead {
	public:
		TwoWaySwitch() { _on = false; }
		virtual ~TwoWaySwitch() {;}

		virtual void write(float value);
		
		virtual String toString();		

	private:
		bool _on;
};

#endif