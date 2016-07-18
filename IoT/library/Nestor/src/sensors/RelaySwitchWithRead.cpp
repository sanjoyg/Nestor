#include <sensors/RelaySwitchWithRead.h>
#include <utils/NestorLogger.h>

RelaySwitchWithRead::~RelaySwitchWithRead() {
	if (_ads != NULL)
		delete _ads;
	_ads = NULL;
}

bool RelaySwitchWithRead::setMember(int index, char *value) {
	if (!GenericDigitalWrite::setMember(index,value)) {
		int channel = -1;
		if (index == 4) {
			channel = atoi(value);
			if (channel < 0 || channel > 3) {
				logger.println();
				logger.print(F("Invalid ADS channel specified (should be 0-3) : "));
				logger.println(value);
				return false;
			}
			_ads = new NestorADS(channel);
		}
	}
	return true;	
}

bool RelaySwitchWithRead::validateConfig() {
	if (_ads == NULL) {
		logger.println();
		logger.println(F("No ADS channel specified."));
		return false;
	}
	return true;
}

float RelaySwitchWithRead::read() {
	if (_ads->isOn()) return 1;
	return 0;
}

void RelaySwitchWithRead::write(float value) {
	bool sensorOn = _ads->isOn();
	logger.println(); logger.print("Turning "); logger.print(getName());
	if (value == 1 && !sensorOn) {
		GenericDigitalWrite::write(1);
		logger.println(" on");
	}
	if (value == 0 && sensorOn) {
		GenericDigitalWrite::write(0);
		logger.println("off");
	}
}

String RelaySwitchWithRead::toString() {
	String toRet = "RelaySwitchWithRead : ";
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