#include <sensors/RelaySwitch.h>

void RelaySwitch::init() {
	digitalWrite(_pin,LOW);
	pinMode(_pin,OUTPUT);
}

void RelaySwitch::write(float value) {
	if (value == 0)
		digitalWrite(_pin,HIGH);
	else
		digitalWrite(_pin,LOW);
}