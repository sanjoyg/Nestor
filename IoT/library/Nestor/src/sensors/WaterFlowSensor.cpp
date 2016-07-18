#include <sensors/WaterFlowSensor.h>
#include "Arduino.h"

extern "C" {
	void interruptFx() {
		
	}
}


void WaterFlowSensor::init() {
	_pulseCounter = 0;
	_lastReadTime = 0;
	pinMode(getPin(),INPUT);
	attachInterrupt(getPin(), [&]() { uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS); }, FALLING);
}

float WaterFlowSensor::read() {
	detachInterrupt(getPin());
	_lastReadTime = millis();
	float toReturn = _pulseCounter;
	_pulseCounter = 0;
	attachInterrupt(getPin(),[&]() { ; }, FALLING);	
}

String WaterFlowSensor::toString() {
	String toRet = "WaterFlowSensor : ";
	toRet += getName();
	toRet += " : ";
	toRet += getTopic();
	toRet += " : ";
	toRet += getPin();
	return toRet; 
}