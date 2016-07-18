#include <sensors/HCS04Sensor.h>
#include <string.h>
#include <stdlib.h>
#include <core/ESP8266PinMap.h>
#include <utils/NestorLogger.h>

HCS04Sensor::HCS04Sensor() {
	_triggerPin = -1;
	_echoPin = -1; 
	_min = -1;
	_max = -1;
	setMode(READ); 
}

bool HCS04Sensor::setMember(int index, char *value) {
	bool valueSet = true;
	int pin = -1;
	char buffer[20];
	char *token = NULL;
	switch (index) {
		case 3:
			pin = ESP8266PinMap::getPin(value);
			if (pin == -1) {
				logger.println();
				logger.print(F("Invalid Trigger Pin specified : "));
				logger.println(value);
				return false;
			} 
			
			setTriggerPin(pin);
			break;

		case 4:
			pin = ESP8266PinMap::getPin(value);
			if (pin == -1) {
				logger.println();
				logger.print(F("Invalid Echo Pin specified : "));
				logger.println(value);
				return false;
			} 
			setEchoPin(pin);
			break;

		case 5:
			// We have got min/max specifications
			strcpy(buffer,value);
			_min = atoi(strtok(buffer,","));
			_max = atoi(strtok(NULL,","));
			break;

		default:
			valueSet = Sensor::setMember(index,value);
			break;
	}
	return valueSet;
}

bool HCS04Sensor::validateConfig() {
	if (!Sensor::validateConfig()) return false;

	if (getTriggerPin() == -1 || getEchoPin() == -1) {
		logger.println(F("Pin not specified for Sensor trigger or/and echo"));
		return false;
	}
	if (_min != -1 && _min >= _max ) {
		logger.println(F("Min specified is greather than or equal to Max"));
		return false;
	}

	return true;
}

void HCS04Sensor::init() {
	pinMode(_triggerPin,OUTPUT);
	pinMode(_echoPin,INPUT);
}

float HCS04Sensor::read() {
  	digitalWrite(_triggerPin, LOW);
	delayMicroseconds(5);
	
	digitalWrite(_triggerPin, HIGH);
	delayMicroseconds(10);
	
	digitalWrite(_triggerPin, LOW);
 
 	// Read the signal from the sensor: a HIGH pulse whose
  	// duration is the time (in microseconds) from the sending
  	// of the ping to the reception of its echo off of an object.
  	pinMode(_echoPin, INPUT);
  	long duration = pulseIn(_echoPin, HIGH);

	yield();

  	// convert the time into a distance
  	long cm = (duration/2) / 29.1;
  	
  	logger.print("HCS04 Reading ");
  	logger.print(cm);
  	logger.print("cm ");
  	
  	if (_min != _max) {
  		// We are to send % values
  		if (cm <=_min) 
  			cm = 0;
  		else if (cm >= _max) 
  			cm = 100;
  		else {
  			cm = (((cm-_min)*100)/(_max-_min));
  		}
		logger.print(" \% calculated to "); logger.print(cm); 
		logger.print("\% using min : "); logger.print(_min);
  		logger.print(" & max: "); logger.print(_max);

 	}
  
  	logger.println();
  	
  	return (float) cm;
}

String HCS04Sensor::toString() {
	String toRet = "HCS04Sensor : ";
	toRet += getName();
	toRet += " : Topic : ";
	toRet += getTopic();
	toRet += " : Read Frequency : ";
	toRet += getReadFrequency();
	toRet += " : Trigger Pin : ";
	toRet += getTriggerPin();
	toRet += " : Echo Pin : ";
	toRet += getEchoPin();
	if (_min != _max) {
		toRet += " : Min : ";
		toRet += _min;
		toRet += " : Max : ";
		toRet += _max;
	}
	return toRet; 
}