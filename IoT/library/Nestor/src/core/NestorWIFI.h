#ifndef NESTOR_WIFI_H
#define NESTOR_WIFI_H
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>

class NestorWIFI {
	public:
		NestorWIFI();
		~NestorWIFI() {;}

		bool initialize();
		
		void loop();

		bool isConnected();

		const char *getIP() { return WiFi.localIP().toString().c_str(); }
		const char *getHostName() { return (const char *) _hostName; }

	protected:
		bool connect();
		void wireHandlers();

	private:
		void handleGet();
		void handlePost();
		void handleImage();
		void handleLogs();
		
		ESP8266WebServer 		_wifiServer;
		ESP8266HTTPUpdateServer	_otaUpdater;
		static const char *_confFormStr;
		static const char *_homeStr;
		File _uploadFP;
		char _hostName[25];
};

#endif