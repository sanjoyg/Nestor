#ifndef HCSR04_SENSOR_H
#define HCSR04_SENSOR_H
#include <core/Sensor.h>

class HCS04Sensor : public Sensor {
	public:
		HCS04Sensor();
		virtual ~HCS04Sensor() {;}

		virtual void init();
		
		int getTriggerPin() { return _triggerPin; }
		void setTriggerPin(int triggerPin) { _triggerPin = triggerPin; }

		int getEchoPin() { return _echoPin; }
		void setEchoPin(int echoPin) { _echoPin = echoPin; }

		//virtual bool parse(const char *);

		virtual float read();

		virtual String toString();

		virtual bool validateConfig();

	protected:
		virtual bool setMember(int index, char *value);

	private:
		int _triggerPin;
		int _echoPin;	
		int _min;
		int _max;
};
#endif