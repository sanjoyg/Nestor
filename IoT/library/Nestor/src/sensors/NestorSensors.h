#ifndef NESTOR_SENSORS_H
#define NESTOR_SENSORS_H

#include <sensors/GenericDigitalRead.h>
#include <sensors/HCS04Sensor.h>

class NestorLED : public GenericDigitalWrite { public: NestorLED() {;} };

class NestorElectricSwitch : public RelaySwitch { public: NestorElectricSwitch() {;} };

class NestorWaterLevelSensor : public HCS04Sensor { public: NestorWaterLevelSensor() {;} };

#endif