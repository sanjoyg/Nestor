#ifndef BUZZER_SWITCH_H
#define BUZZER_SWITCH_H

#include <sensors/RelaySwitch.h>

#define BUZZER_PERIOD	1

class BuzzerSwitch : public RelaySwitch {

	public:
		BuzzerSwitch() { _buzzerDuration = BUZZER_PERIOD;}
		virtual ~BuzzerSwitch() {;}

		bool setMember(int , char *);
		void write(float value);

		virtual String toString();
	private:
		int _buzzerDuration;
};

#endif