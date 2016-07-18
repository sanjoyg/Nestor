#ifndef SENSOR_H
#define SENSOR_H

#define DIGITAL_READ 			"genericdigitalread"
#define DIGITAL_WRITE 			"genericdigitalwrite"
#define RELAY_SWITCH 			"RelaySwitch"
#define RELAY_SWITCH_WITH_READ	"RelaySwitchWithRead"
#define HCSR04_SENSOR			"hcsr04sensor"

#define WATER_LEVEL_SENSOR		"WaterLevelSensor"
#define NESTOR_LED				"LED"
#define ELECTRIC_SWITCH			"ElectricSwitch"
#define TWO_WAY_SWITCH			"TwoWaySwitch"
#define BUZZER_SWITCH			"BuzzerSwitch"

#define SENSOR_READ_OPTIMIZATION 1*60*1000

#include <Arduino.h>

class Sensor {
	public:
		enum SENSORMODE {
			READ,
			WRITE,
			READWRITE
		};

		virtual bool isRead() 	{ return (_mode == READ || _mode == READWRITE); }
		virtual bool isWrite() 	{ return (_mode == WRITE || _mode == READWRITE); }

		virtual char *getName();
		virtual char *getTopic();
		virtual SENSORMODE getMode();

		void setName(const char *);
		void setTopic(const char *);
		void setMode(SENSORMODE);

		virtual void init() {;}
		virtual float read() { return -1; }
		virtual void write(float value) {;}

		virtual bool parse(const char *);
		virtual String toString() = 0;
		virtual ~Sensor();

		virtual void updateLastReadTime() { _lastReadTime = millis(); }
		virtual void updateLastSendTime() { _lastSendTime = millis(); }
		virtual bool timeToRead();

		virtual int getReadFrequency() { return _readFrequency; }

		virtual bool validateConfig();

		virtual bool moreThanOptimizedInterval();
		float getLastReadValue() { return _lastReadValue; }
		void setLastReadValue(float value) { _lastReadValue = value; }
	
	protected:
		Sensor();
	
		SENSORMODE _mode;
		int _lastReadTime;
		int _lastSendTime;
		int _readFrequency;
		
		float _lastReadValue;
		
		virtual bool setMember(int index, char *value);
		
	private:
		char *_name;
		char *_topic;
};

class SensorFactory {
	public:
		static Sensor* makeSensor(char *token);

	private:
		SensorFactory() {;} // Disallow instantiation
};

#endif