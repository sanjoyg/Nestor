#include <utils/NestorTime.h>
#include <WiFiClient.h>
#include <utils/NestorLogger.h>
#include <ESP8266mDNS.h>

#define ONE_HOUR 1*60*60*1000
#define LOCAL_PORT 2390

NestorTime *NestorTime::_instance		= NULL;
const char *NestorTime::_ntpServerName 	= "time.nist.gov";


NestorTime::NestorTime() {
	_epoch = 0;
	_cpuTime = 0;
	memset(_buffer,0x00,sizeof(_buffer));
}

NestorTime *NestorTime::getInstance() {
	if (_instance == NULL)
		_instance = new NestorTime();
	return _instance;
}

 char *NestorTime::formatCurrentTime() {
 	return formatTime(getCurrentTime(true));
}

char *NestorTime::formatTime(long t) {
	struct tm *timeStruct = localtime(&t);
	sprintf(_buffer,"%02d/%02d %02d:%02d:%02d",
			timeStruct->tm_mday,
			timeStruct->tm_mon+1,
			timeStruct->tm_hour,
			timeStruct->tm_min,
			timeStruct->tm_sec);
	return _buffer;
}

char *NestorTime::formatTime() {
	return formatTime(getLastReadTime());
}

time_t NestorTime::getCurrentTime(bool refresh) {
	if (refresh || _epoch == 0) {
		long diff = millis() - _cpuTime;
		if (diff > ONE_HOUR || diff <0 || _epoch == 0) {
			getTime();
			return _epoch;
		}
	}
	time_t t = _epoch + ((int) (millis()-_cpuTime)/1000);
	return t;
}

struct tm* NestorTime::getTime() {
  logger.println("Starting UDP");
  WiFiUDP udp;
  udp.begin(LOCAL_PORT);
  logger.print("Local port: ");
  logger.println(udp.localPort());
  
  IPAddress timeServerIP;
  //get a random server from the pool
  WiFi.hostByName(_ntpServerName, timeServerIP); 

  makeNTPPacket(); // send an NTP packet to a time server
  
  udp.beginPacket(timeServerIP, 123); //NTP requests are to port 123
  udp.write(_packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
  logger.println("packet sent");
  logger.println(timeServerIP);
  delay(1000);
  
  int retry = 0;
  while (true) {
  	  int cb = udp.parsePacket();
	  if (!cb) {
	    logger.println("No Time packet yet");
	    retry++;
	    if (retry > 10) {
	    	logger.println("Retried 10 times to get time, quitting.");
	    	break;
	    }
	  }
	  else {
	    logger.print("packet received, length=");
	    logger.println(cb);
	    udp.read(_packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
	    unsigned long highWord = word(_packetBuffer[40], _packetBuffer[41]);
	    unsigned long lowWord = word(_packetBuffer[42], _packetBuffer[43]);

	    unsigned long secsSince1900 = highWord << 16 | lowWord;
	    logger.print("Seconds since Jan 1 1900 = " );
	    logger.println(secsSince1900);

	    // now convert NTP time into everyday time:
	    logger.print("Unix time = ");
	    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
	    const unsigned long seventyYears = 2208988800UL;
	    // subtract seventy years:
	   //  hack to add 5.5 hrs in seconds to get IST
	    _epoch = secsSince1900 - seventyYears + 19800;
	    _cpuTime = millis();
	    // print Unix time:
	    logger.println(_epoch);

	    // print the hour, minute and second:
	    logger.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
	    logger.print((_epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
	    logger.print(':');
	    if ( ((_epoch % 3600) / 60) < 10 ) {
	      // In the first 10 minutes of each hour, we'll want a leading '0'
	      logger.print('0');
	    }
	    logger.print((_epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
	    logger.print(':');
	    if ( (_epoch % 60) < 10 ) {
	      // In the first 10 seconds of each minute, we'll want a leading '0'
	      logger.print('0');
	    }
	    logger.println(_epoch % 60); // print the second
	    break;
	  }
	  delay(1000);
	}
	
	return localtime(&_epoch);
}

unsigned long NestorTime::makeNTPPacket() {
  logger.println("Making NTP packet...");
  memset(_packetBuffer, 0, NTP_PACKET_SIZE);
  _packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  _packetBuffer[1] = 0;     // Stratum, or type of clock
  _packetBuffer[2] = 6;     // Polling Interval
  _packetBuffer[3] = 0xEC;  // Peer Clock Precision
  _packetBuffer[12]  = 49;
  _packetBuffer[13]  = 0x4E;
  _packetBuffer[14]  = 49;
  _packetBuffer[15]  = 52;
}