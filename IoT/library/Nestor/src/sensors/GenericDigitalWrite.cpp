#include <sensors/GenericDigitalWrite.h>

void GenericDigitalWrite::init() {
	pinMode(_pin,OUTPUT);
	digitalWrite(_pin, LOW);
}

void GenericDigitalWrite::write(float value) {
	if (value == 0)
		digitalWrite(_pin,LOW);
	else
		digitalWrite(_pin,HIGH);
}

String GenericDigitalWrite::toString() {
	String toRet = "GenericDigitalWrite : ";
	toRet += getName();
	toRet += " : Publish Topic : ";
	toRet += getTopic();
	toRet += " : ";
	toRet += " Read Frequency : ";
	toRet += getReadFrequency();
	toRet += " Pin : "; 
	toRet += getPin();
	return toRet; 
}