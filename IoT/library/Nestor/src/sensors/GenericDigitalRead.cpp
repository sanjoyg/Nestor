#include <sensors/GenericDigitalRead.h>

void GenericDigitalRead::init() {
	pinMode(_pin,INPUT);
}

float GenericDigitalRead::read() {
	return (float) digitalRead(_pin);
}

String GenericDigitalRead::toString() {
	String toRet = "GenericDigitalRead : ";
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