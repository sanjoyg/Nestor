#include <sensors/TwoWaySwitch.h>
#include <utils/NestorLogger.h>

void TwoWaySwitch::write(float value) {
	
	bool isPowerOn = isADCOn();
	logger.println();
	logger.print("Current State of switch : "); logger.print(_on);

	if ((value == 1 && !isPowerOn) || (value == 0 && isPowerOn)) {
		_on = !_on;
	}

	logger.print(" Request is "); logger.print(value);
	logger.print(" while power is "); logger.print(isPowerOn);
	logger.print(" toggled state to "); logger.println(_on);

	GenericDigitalWrite::write(_on);
}

String TwoWaySwitch::toString() {
	String toRet = "TwoWaySwitch : ";
	toRet += getName();
	toRet += " : Publish Topic : ";
	toRet += getTopic();
	toRet += " : ";
	toRet += " Read Frequency : ";
	toRet += getReadFrequency();
	toRet += " Pin : "; 
	toRet += getPin();
	toRet += " : ADS Channel : ";
	toRet += _ads->getChannel();
	return toRet; 
}