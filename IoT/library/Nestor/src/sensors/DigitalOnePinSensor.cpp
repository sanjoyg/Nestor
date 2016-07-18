#include <sensors/DigitalOnePinSensor.h>
#include <string.h>
#include <stdlib.h>
#include <core/ESP8266PinMap.h>
#include <utils/NestorLogger.h>

bool DigitalOnePinSensor::setMember(int index, char *value) {
	if (!Sensor::setMember(index,value)) {
		if (index == 3) {
			int pin = ESP8266PinMap::getPin(value);
			if (pin == -1) {
				logger.println();
				logger.print("Invalid Pin specified : ");
				logger.println(value);
				return false;
			} 
			setPin(pin);	
		} else {
			return false;
		}
	}
	return true;
}

bool DigitalOnePinSensor::validateConfig() {
	if (!Sensor::validateConfig()) return false;
	if (_pin == -1) {
		logger.println("Invalid/No Pin specified for sensor.");
		return false;
	}
	return true;
}