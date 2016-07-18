#ifndef WATER_FLOW_SENSOR_H
#define WATER_FLOW_SENSOR_H
#include <sensors/DigitalOnePinSensor.h>

class WaterFlowSensor : public DigitalOnePinSensor {
	public:
		WaterFlowSensor() { setMode(READ); }

		virtual void init();

		virtual float read();

		virtual String toString();		

	private:
		int _pulseCounter;
		int _lastReadTime;

};

#endif