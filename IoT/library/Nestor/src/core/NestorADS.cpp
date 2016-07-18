#include <core/NestorADS.h>
#include <utils/NestorLogger.h>

#define ADS_LOOP_TIMES			3
#define ADS_READ_ITERATIONS		30

int NestorADS::_sda = DEFAULT_SDA_PIN;
int NestorADS::_scl = DEFAULT_SCL_PIN;

bool NestorADS::_initialized = false;

Adafruit_ADS1115 *NestorADS::_ads = NULL;

NestorADS::NestorADS(int channel) {
	_adsChannel = channel;
	if (_ads == NULL) {
		_ads = new Adafruit_ADS1115(0x48);
	}
}

void NestorADS::initialize() {
	if (!_initialized) {
		_ads->begin();
		_initialized = true;
	}
}

bool NestorADS::isOn() {
	bool on = false;
	int value = 0;
	int max = -1;
	int min = 100000;
	int spread = 0;

	for (int outer=0; outer< ADS_LOOP_TIMES; outer++) {
		max = -1;
		min = 100000;

		for (int inner=0;inner < ADS_READ_ITERATIONS; inner++) {
			value = _ads->readADC_SingleEnded(_adsChannel);
			if (value < min) min = value;
			if (value > max) max = value;
		}

		spread = spread + (max - min);
		//yield();
	}

	spread = (int)(spread / 3);
	logger.println(); logger.print("ADS Read on channel : "); logger.print(_adsChannel);
	logger.print(" spread : "); logger.print(spread);
	logger.print(" thus isOn : "); logger.println((spread >SPREAD_THRESHOLD)? "True" : "False");
	
	return (spread > SPREAD_THRESHOLD);
}