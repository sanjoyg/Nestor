#ifndef NESTOR_ADS_H
#define NESTOR_ADS_H
#include <core/ESP8266PinMap.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

// Notes
// connect SDA to D3  - 0
// connect SCL to D4 - 2

// What I observed was that the spread between min and max reading on 1000 samples
// is what determines if current is on or now. When there is no current flowing
// the spread (diff of max and min) is between 10-20. With a 11W LED the spread
// was observed to be around 60 and with a 60W Bulb, the spread was 300

#define DEFAULT_SDA_PIN 	D3
#define	DEFAULT_SCL_PIN		D4

#define SPREAD_THRESHOLD	50

class NestorADS {

	public:
		NestorADS(int channel);

		static void setSDAPin(int pin) { _sda = pin; }
		static void setSCLPin(int pin) { _scl = pin; }

		bool isOn();

		int getChannel() { return _adsChannel; }
	private:
		int _adsChannel;

		static void initialize();		
		static bool _initialized;
		static int _sda;
		static int _scl;
		static Adafruit_ADS1115* _ads;
};


#endif