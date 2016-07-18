#ifndef DIGITAL_ONE_PIN_SENSOR_H
#define DIGITAL_ONE_PIN_SENSOR_H
#include <core/Sensor.h>

class DigitalOnePinSensor : public Sensor {
	public:
		DigitalOnePinSensor() { _pin = -1; }
		virtual ~DigitalOnePinSensor() {;}

		int getPin() { return _pin; }
		void setPin(int pin) { _pin = pin; }
			
		virtual bool validateConfig();
	protected:
		virtual bool setMember(int index, char *value);
		int _pin;	
};
#endif