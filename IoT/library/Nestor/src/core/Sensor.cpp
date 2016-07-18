#include <core/Sensor.h>
#include <stdlib.h>
#include <string.h>
#include <sensors/GenericDigitalRead.h>
#include <sensors/GenericDigitalWrite.h>
#include <sensors/RelaySwitch.h>
#include <sensors/RelaySwitchWithRead.h>
#include <sensors/BuzzerSwitch.h>
#include <sensors/HCS04Sensor.h>
#include <sensors/WaterFlowSensor.h>
#include <nestor/Config.h>
#include <sensors/NestorSensors.h>
#include <utils/NestorLogger.h>
#include <sensors/TwoWaySwitch.h>

Sensor::Sensor() {
	_name  = 0;
	_topic = 0;
	_mode = READ;
	_lastReadTime = 0;
	_readFrequency = 0;
	_lastSendTime = 0;
	_lastReadValue = -123456;
}

Sensor::~Sensor() {
	if (_name != 0)
		free(_name);
	if (_topic != 0)
		free(_topic);
}

char *Sensor::getName() {
	return _name;
}

void Sensor::setName(const char *name) {
	if (name != 0 && strlen(name) != 0) {
		_name = (char *)malloc(strlen(name));
		strcpy(_name,name);
	}
}

char * Sensor::getTopic() {
	return _topic;
}

void Sensor::setTopic(const char *topic) {
	if (topic != 0 && strlen(topic) != 0) {
		_topic = (char *)malloc(strlen(topic));
		strcpy(_topic,topic);
	}
}

Sensor::SENSORMODE Sensor::getMode() {
	return _mode;
}

void Sensor::setMode(Sensor::SENSORMODE mode) {
	_mode = mode;
}

bool Sensor::timeToRead() {
	if (_lastReadTime == 0)
		return true;
	return (((millis() - _lastReadTime) > _readFrequency*1000) || ((millis() - _lastReadTime) < 0));
}

bool Sensor::moreThanOptimizedInterval() {
	return ((millis() - _lastSendTime > SENSOR_READ_OPTIMIZATION) || (millis() - _lastSendTime < 0));
}

bool Sensor::setMember(int index, char *value) {
	bool valueSet = true;
	int intValue = 0;
	switch (index) {
		case 0:
			setName(value);
			break;
		case 1:
			setTopic(value);
			break;
		case 2:
			if (value != NULL) {
				if (stricmp(value,"default") == 0)
					intValue = SENSOR_READ_INTERVAL;
				else 
					intValue = atoi(value);
			}

			if (intValue <= 0) {
				logger.println(F("Invalid/No Read Frequency mentioned"));
				return false;
			}

			_readFrequency = intValue;

			break;
		default:
			valueSet = false;
			break;
	}
	return valueSet;
}

bool Sensor::parse(const char *line) {
	if (line == 0 || strlen(line) == 0) {
		logger.println();
		logger.print(F("parse() Got an empty sensor line, un-expected."));
		return false;
	}
	
	// Lets tokenize using | as the separator
	char *token = strtok((char *)line,"|");
	setMember(0,token);
	int index = 1;
	
	while ((token = strtok(0,"|")) != 0) {
		if (!setMember(index,token))
			return false;		
		index++;
	}

	return validateConfig();
}

bool Sensor::validateConfig() {
	if (getTopic() == NULL || strlen(getTopic()) == 0) {
		logger.println(F("Topic not specified for Sensor"));
		return false;
	}
	if (getName() == NULL || strlen(getName()) == 0) {
		logger.println(F("Name not specified for Sensor"));
		return false;
	}
	if (_readFrequency < 0 ) {
		logger.println(F("Read frequency not specified.."));
		return false;
	}
	
	return true;
}

Sensor* SensorFactory::makeSensor(char *line) {
	Sensor *sensor = 0;
	char *token = strtok(line,"=");

	if (token != 0 && strlen(token) != 0 ) {
		if (stricmp(token,DIGITAL_READ) ==0)
			sensor = new GenericDigitalRead();
		else if (stricmp(token,DIGITAL_WRITE) == 0)
			sensor = new GenericDigitalWrite();
		else if (stricmp(token,HCSR04_SENSOR) == 0)
			sensor = new HCS04Sensor();
		else if (stricmp(token,WATER_LEVEL_SENSOR)==0)
			sensor = new NestorWaterLevelSensor();
		else if (stricmp(token,NESTOR_LED) == 0)
			sensor = new NestorLED();
		else if (stricmp(token,ELECTRIC_SWITCH)==0)
			sensor = new NestorElectricSwitch();
		else if (stricmp(token,TWO_WAY_SWITCH) == 0)
			sensor = new TwoWaySwitch();
		else if (stricmp(token,RELAY_SWITCH) == 0)
			sensor = new RelaySwitch();
		else if (stricmp(token,RELAY_SWITCH_WITH_READ) == 0)
			sensor = new RelaySwitchWithRead();
		else if (stricmp(token,BUZZER_SWITCH) == 0)
			sensor = new BuzzerSwitch();
		else {
			logger.println();
			logger.print(F("SensorFactory::makeSensor() Unrecognized token :"));
			logger.println(token);
			return NULL;
		}
		
		if (sensor != NULL) {
			if (!sensor->parse(strtok(NULL,"="))) {
				logger.println(F("Invalid Sensor configuration missing or extra parameters"));
				return NULL;
			}
		}

	} else {
		logger.println(F("Invalid/0 token passed to Sensor Factory"));
	}
	return sensor;
}