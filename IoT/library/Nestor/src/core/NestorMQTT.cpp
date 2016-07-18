#include <core/NestorMQTT.h>
#include <Nestor.h>
#include <nestor/config.h>
#include <core/Sensor.h>
#include <core/SensorList.h>
#include <utils/NestorLogger.h>
#include <utils/NestorOLED.h>

extern "C" {
	void onMessage(char* topic, byte* payload, unsigned int length) {
		payload[length]=0;
		Nestor::getInstance()->processIncomingMsg(topic,(const char *)payload);		
	}
}

NestorMQTT::NestorMQTT() : _mqClient(_wifiClient) {;}

bool NestorMQTT::initialize() {
	_mqClient.setServer(MQTT_HOST,MQTT_PORT);
	_mqClient.setCallback(onMessage);
	return connect();
}

bool NestorMQTT::connect() {
	int index = 0;
	
	while (!_mqClient.connected()) {
		OLED_PRINT("MQTT Attempt");
		logger.println();
		logger.print(F("Attempting MQTT connection to ... : "));
		logger.println(MQTT_HOST);
		
		char buffer[20];
		sprintf(buffer,"%d",Nestor::getInstance()->getId());

		if (_mqClient.connect((const char*)buffer)) {
      		logger.println(F("Connected to MQTT..."));
    	} else {
	      	logger.print(" failed, rc=");
	      	logger.print(_mqClient.state());
	      	logger.println(" try again in 5 seconds");
	      	// Wait 5 seconds before retrying
	      	;
			yield();
	      	delay(5000);
	    }
	    index++;
	    if (index > 100) {
	    	logger.println(F("NestorMQTT::connect() PANIC could not connect with MQTT for long..."));
	    	return false;
	    }
	}
	OLED_PRINT("MQTT Connected");
	subscribeSensors();
	Nestor::blink(6,500);
	return true;
}

bool NestorMQTT::subscribe(const char *topic) {
	return _mqClient.subscribe(topic);
}

void NestorMQTT::subscribeSensors() {
	SensorList::SensorNode *sensorNode = Nestor::getInstance()->getSensorList()->getHead();
	Sensor *sensor = NULL;
	char topicBuf[100];
	logger.println();
	while (sensorNode != NULL) {
		sensor = sensorNode->obj;
		if (sensor->isWrite()) {
			sprintf(topicBuf,"%s%s",sensor->getTopic(),WRITE_SUFFIX);
			logger.print(F("Subscribing to : "));
			logger.println(topicBuf);
			subscribe(topicBuf);
			;
			yield();
		}
		sensorNode = sensorNode->next;
	}
	logger.println(F("Subscription complete."));
}

void NestorMQTT::loop() {
	_mqClient.loop();
	yield();
}

bool NestorMQTT::publish(const char *topic, float payload) {
	char buffer[10];
	sprintf(buffer,"%d",(int)payload);
	return publish(topic,buffer);
}

bool NestorMQTT::publish(const char *topic, const char *value) {
	if (!_mqClient.connected()) {
		if (!connect())
			return false;
	}
	logger.println();
	logger.print(F("About to publish: "));	logger.print(value);
	logger.print(F(" to topic : ")); logger.println(topic);
	
	if (!_mqClient.publish(topic,value)) {
		// Try Once more
		if (!_mqClient.connected()) 
			if (!connect()) return false;
		_mqClient.publish(topic,value);
	}
	
	return true;
}