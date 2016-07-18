#ifndef NESTOR_H
#define NESTOR_H


class NestorWIFI;
class NestorMQTT;
class NestorWIFIAP;
class SensorList;

class Nestor {
	// Lets make it a singleton
	public:
		~Nestor();

		static Nestor* getInstance();

		static void warmup();
		static void blink(int noTimes, int delay, bool startOn = true);
		
		bool initialize();
		void loop();

		int getId();
		
		inline const char *getIP();
		
		void reboot();
		void processIncomingMsg(const char *, const char *);
		
		SensorList* getSensorList();
		
		inline NestorMQTT* getMQ() { return _mqtt; }

		void enable() 	{ _enabled = true; }
		void disable() 	{ _enabled = false; }
		bool isEnabled() { return _enabled; }

		void publishStatus(const char *);

	protected:
		Nestor();

		bool sensorConfFileExists();
		bool readAndInitializeSensors();

		void readLoop();

		void processSystemMsg(const char *, const char *);
		void processSensorMsg(const char *, const char *);

		
		
		void setTime();
	private:

		static Nestor * _instance;	
		NestorWIFI *_wifi;
		NestorMQTT *_mqtt;
		SensorList* _sensors;
		
		bool _sensorsRead;
		long _lastReadTime;
		long _lastStatusTime;

		bool _enabled;
		char _systemTopic[50];
		char _statusTopic[56];
};
#endif