#include <nestor/Config.h>
#include <Arduino.h>
#include <FS.h>
#include <core/ESP8266PinMap.h>

#define OLED_STATUS_FILE	"oled.1"
#define DEBUG_STATUS_FILE	"debug.1"
#define STATUS_LED_FILE		"statusled.1"

Config *Config::_instance = NULL;

Config::Config() {
	_statusLED	= -1;

	SPIFFS.begin();

	_oled = !doesFileExist(OLED_STATUS_FILE);
	_debug = !doesFileExist(DEBUG_STATUS_FILE);

	if (doesFileExist(STATUS_LED_FILE)) {
		_statusLED = D0;
	}
}

bool Config::doesFileExist(const char *fileName) {
	File fp = SPIFFS.open(fileName,"r");
	if (!fp)
		return false;
	fp.close();
	return true;
}

void Config::setOLED(bool value) {
	_oled = value;
	setFileBasedStatus(OLED_STATUS_FILE,value);
}

void Config::setDebug(bool value) {
	_debug = value;
	setFileBasedStatus(DEBUG_STATUS_FILE,value);
}

void Config::setStatusLED(bool value) {
	_statusLED = value;
	setFileBasedStatus(STATUS_LED_FILE,value);
}

void Config::setFileBasedStatus(const char* fileName, bool value) {
	if (!value) {
		File fp = SPIFFS.open(fileName,"w");
		if (fp) {
			fp.write(1);
			fp.close();
		}
	} else {
		SPIFFS.remove(fileName);
	}
}

Config *Config::getInstance() {
	if (_instance == NULL)
		_instance = new Config();
	return _instance;
}

void Config::parse(char *line) {
	if (line == NULL || strlen(line) == 0 || line[0] != '!')
		return;
	line++;
	char *token = strtok(line,"=");
	if (token == NULL) return;
}