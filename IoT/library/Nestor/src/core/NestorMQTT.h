#ifndef NESTOR_MQTT_H
#define NESTOR_MQTT_H
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class NestorMQTT {
	public:
			NestorMQTT();
			bool initialize();
			bool publish(const char *topic, const char *payload);
			bool publish(const char *topic, float payload);
			void loop();
			bool subscribe(const char *);
			void subscribeSensors();
			
	protected:
			bool connect();
			
	private:
		WiFiClient   _wifiClient;
		PubSubClient _mqClient;
};
#endif