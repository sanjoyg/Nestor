#ifndef NESTOR_TIME_H
#define NESTOR_TIME_H
#define NTP_PACKET_SIZE 48

#include <Arduino.h>
#include <time.h>

#define nestorTime (*NestorTime::getInstance())
class NestorTime {
	public:
		struct tm* getTime();
		char *formatTime();
		char *formatTime(long);
		char *formatCurrentTime();
		long getLastReadTime() { return _epoch; }

		time_t getCurrentTime(bool refresh=true);

		static NestorTime* getInstance();

	protected:
		unsigned long makeNTPPacket();

	private:
		NestorTime();

		static NestorTime *_instance;
		static const char *_ntpServerName;

		byte _packetBuffer[NTP_PACKET_SIZE];	
		char _buffer[16];
		long _epoch;
		long _cpuTime;
};

#endif