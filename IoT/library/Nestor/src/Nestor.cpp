#include "Nestor.h"
#include "core/NestorWIFI.h"
#include "core/NestorMQTT.h"
#include "core/Sensor.h"
#include "core/SensorList.h"
#include "nestor/Config.h"
#include <FS.h>
#include <utils/NestorLogger.h>
#include <utils/NestorTime.h>
#include <pgmspace.h>
#include <utils/NestorOLED.h>


Nestor* Nestor::_instance = NULL;

Nestor* Nestor::getInstance() {
	if (_instance == NULL)
		_instance = new Nestor();
	return _instance;
}

Nestor::Nestor() {
	_sensors = new SensorList();
	_sensorsRead = false;
	_lastReadTime = 0;
	_lastStatusTime = 0;
	_mqtt = NULL;
	_wifi = NULL;
	_enabled = true;
	sprintf(_systemTopic,"%s%d",SYSTEM_TOPIC_PREFIX,getId());
	sprintf(_statusTopic,"%s%s",_systemTopic,STATUS_TOPIC_SUFFIX);
	Serial.println(F(""));
	Serial.println(F("Nestor Construction Complete.."));
}

Nestor::~Nestor() {
	if (_mqtt != NULL)
		delete _mqtt;
	if (_wifi != NULL)
		delete _wifi;
}

SensorList* Nestor::getSensorList() { 
	return _sensors; 
}

const char* Nestor::getIP() {
	return (_wifi == NULL ? "NA" : _wifi->getIP());
}

int Nestor::getId() { 
	return ESP.getChipId(); 
}

void Nestor::reboot() {
	OLED_PRINT("Rebooting...");
	blink(20,100);
    delay(5000);
	ESP.restart();
}

bool Nestor::initialize() {
	OLED_PRINT("Init Start");
	_wifi = new NestorWIFI();

	if (!_wifi->initialize()) {
		OLED_PRINT("WiFi Fail.");

		logger.println(F("Failed to initialize WiFi"));
		return false;
	}
	
	OLED_PRINT(getIP());

	nestorTime.getTime();
	
	OLED_PRINT(nestorTime.formatTime());

	_mqtt = new NestorMQTT();
		
	if (!_mqtt->initialize()) {
		OLED_PRINT("MQTT Fail.");
		logger.println(F("Failed to initialize MQTT."));
		return false;
	}

	_mqtt->subscribe(_systemTopic);
	logger.print("Subscribed to ");
	logger.println(_systemTopic);
	publishStatus("Hail");
	logger.println(F("Nestor initialization complete."));
	OLED_PRINT("Init Done.");
	return true;
}

bool Nestor::sensorConfFileExists() {
	SPIFFS.begin();

	File fp = SPIFFS.open(SENSOR_CONFIG_FILE,"r");
	if (!fp)
		return false;
	fp.close();
	return true;
}

bool Nestor::readAndInitializeSensors() {

	SPIFFS.begin();

	File fp = SPIFFS.open(SENSOR_CONFIG_FILE, "r");
	if (!fp) {
		logger.print(F("Failed to open sensor config file : "));
		logger.println(SENSOR_CONFIG_FILE);
		return false;
	}
	
	char line[255];
	int lineNo = 0;
	int bytesRead = 0;
	bool anySensorAdded = false;
	Sensor* sensor = NULL;

	while((bytesRead = fp.readBytesUntil('\n', line, sizeof(line)-1)) != 0) {
		lineNo++;
		line[bytesRead]=0;

		if (line == NULL || strlen(line) == 0 || line[0] == '#')
			continue;

		sensor = SensorFactory::makeSensor(line);
		if (sensor == NULL) {
			logger.print(F("Failed to process sensor configuration file, at line #"));
			logger.println(lineNo);
			logger.println(F("Removing the file..."));
			SPIFFS.remove(SENSOR_CONFIG_FILE);
			return false;
		}
		
		_sensors->add(sensor);
		sensor->init();	
		anySensorAdded = true;
	}

	fp.close();

	if (!anySensorAdded) {
		logger.println(F("No sensors found in configuration file."));
		logger.println(F("Removing the file..."));
		SPIFFS.remove(SENSOR_CONFIG_FILE);
		delay(500);
		return false;
	}
	_sensorsRead = true;
	return true;
}

void Nestor::loop() {
	blink(2,200);
	
	if ((millis() - _lastStatusTime > STATUS_INTERVAL) || (millis() - _lastStatusTime <0))  {
		_lastStatusTime = millis();
		publishStatus("Alive");
		if (_enabled)  {
			OLED_PRINT("Alive @");
		}
		else {
			OLED_PRINT("Alive(Disabled)@");
		}
		
		OLED_PRINT(nestorTime.formatTime());
	}

	// Let WiFi handle any pending clients.
	if (_wifi != NULL) _wifi->loop();
	if (_mqtt != NULL) _mqtt->loop();

	if (_sensorsRead) {
		logger.println(F("Starting Read Loop."));
		readLoop();
		logger.println(F("Read Loop complete."));
	} else {
		if (sensorConfFileExists()) {
			if (!readAndInitializeSensors()) {
				logger.println(F("Failed to read sensor config file..."));
				delay(5000);
				reboot();
			}
			logger.println(F("Sensor config file found and read."));
			_mqtt->subscribeSensors();
		} else {
			OLED_PRINT("SensConf NotF");
			logger.println(F("Sensor config file not found yet."));
			delay(10000);
		}
	}
	_lastReadTime = millis();

	yield();
	delay(1000);
}

void Nestor::readLoop() {
	if (!_enabled) {
		logger.println(F("Read Loop skipping as controller disabled."));
		return;
	}

	SensorList::SensorNode *sensorNode = _sensors->getHead();
	Sensor *sensor = NULL;

	float readValue = 0;
	while (sensorNode != NULL) {
		sensor = sensorNode->obj;
		if (sensor == NULL) {
			continue;
		}

		if (sensor->isRead() && sensor->timeToRead()) {	
			readValue = sensor->read();
			// Msg Optimization
			if (readValue != sensor->getLastReadValue() || sensor->moreThanOptimizedInterval()) {			
				_mqtt->publish(sensor->getTopic(),sensor->read());
				sensor->updateLastSendTime();
			}
			sensor->updateLastReadTime();
			sensor->setLastReadValue(readValue);
		}
		sensorNode = sensorNode->next;
		
		yield();
	}
}

void Nestor::processSystemMsg(const char *topic, const char *value) {
	logger.println();
	logger.print(F("Got System Message : "));
	logger.print(value);

	if (stricmp(value,REBOOT_SYSTEM_CMD) == 0) {
		reboot();
	} else if (stricmp(value,ENABLE_SYSTEM_CMD) == 0) {
		enable();
	} else if (stricmp(value,DISABLE_SYSTEM_CMD) == 0) {
		disable();
	} else if (stricmp(value,DEBUG_ON_SYSTEM_CMD) == 0) {
		conf.setDebug(true);
		logger.enable();
	} else if (stricmp(value,DEBUG_OFF_SYSTEM_CMD) == 0) {
		conf.setDebug(false);
		logger.disable();
	} else if (stricmp(value,OLED_ENABLE) == 0) {
		conf.setOLED(true);
		NestorOLED::init();
		NestorOLED::clearDisplay();
		NestorOLED::displayOn();
		NestorOLED::print("Welcome");
	} else if (stricmp(value,OLED_DISABLE) == 0) {
		conf.setOLED(false);
		NestorOLED::displayOff();
	} else if (stricmp(value,STATUS_LED_ENABLE) == 0) {
		conf.setStatusLED(true);
	} else if (stricmp(value,STATUS_LED_DISABLE) == 0) {
		conf.setStatusLED(false);
	}
	publishStatus("Alive");
}

void Nestor::processSensorMsg(const char *topic, const char *value) {
	
	if (!_enabled) {
		logger.println(F("Got sensor message but ignoring as disabled."));
		return;
	}

	SensorList::SensorNode *sensorNode = _sensors->getHead();
	Sensor *sensor = NULL;

	while (sensorNode != NULL) {
		sensor = sensorNode->obj;
		if (sensor == NULL) continue;
		if (sensor->isWrite() && strstr(topic,sensor->getTopic()) != NULL) {
			logger.print("Acted on Sensor :");
			logger.print(sensor->getName());
			logger.print(" with value : ");
			logger.println(value);
			sensor->write(atof(value));
		}
		sensorNode = sensorNode->next;
	}
}

void Nestor::processIncomingMsg(const char *topic, const char *value) {
	logger.print(F("Got incoming message on topic : "));
	logger.print(topic);
	logger.print(" msg: ");
	logger.println(value);

	if (stricmp(topic,_systemTopic) == 0) {
		processSystemMsg(topic,value);
	} else {
		processSensorMsg(topic,value);
	}
}

void Nestor::warmup() {
	if (conf.oled()) {
		NestorOLED::init();
		NestorOLED::clearDisplay();
		NestorOLED::displayOn();
		NestorOLED::print("Nestor Start");
		NestorOLED::print("Warmup 10 secs.");
	}

	logger.println("Warming up for 10 seconds...");
	blink(20,500);
}

void Nestor::blink(int noTimes, int delayms, bool startOn) {
	if (conf.statusLED() != -1) {
		pinMode(conf.statusLED(), OUTPUT);
		digitalWrite(conf.statusLED(), (startOn?HIGH:LOW));
		bool on = !startOn;
		for (int i=0; i<noTimes; i++) {
			digitalWrite(conf.statusLED(),(on?HIGH:LOW));
			on = !on;
			delay(delayms);
		}
		digitalWrite(conf.statusLED(),LOW);
	}
}

void Nestor::publishStatus(const char *msg) {
	char buffer[230];
	
	sprintf(buffer,"{\"e\":\"%s\",\"t\":\"%s\",\"ip\":\"%s\",\"o\":\"%s\",\"d\":\"%s\",\"s\":\"%s\"}",
				(_enabled ? "ON" : "OFF"),
				nestorTime.formatCurrentTime(),
				getIP(),
				(conf.oled() ? "ON" : "OFF"),
				(conf.debug() ? "ON" : "OFF"),
				msg);
	
	if (_mqtt != NULL)
		_mqtt->publish(_statusTopic,buffer); 
	else 
		logger.println("MQTT IS NULL");
}