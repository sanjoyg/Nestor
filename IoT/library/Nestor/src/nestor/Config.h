#ifndef CONFIG_H
#define CONFIG_H
#include <core/esp8266pinmap.h>

#define WIFI_SSID	"ghoshnet"
#define WIFI_PWD	"9810002485"

//#define WIFI_SSID	"Sanjoy's iPhone"
//#define WIFI_PWD	"hua9ce0e5a3ex"

#define MQTT_HOST	"192.168.1.10"
//#define MQTT_HOST	"test.mosquitto.org"
#define MQTT_PORT	1883

#define SYSTEM_TOPIC_PREFIX	"c503/system/"
#define STATUS_TOPIC_SUFFIX	"/status"
#define WRITE_SUFFIX		"/write"

#define WIFI_CHECK_INTERVAL		30000
#define WIFI_IP_ACQ_WAIT		2500 
#define SENSOR_READ_INTERVAL	10000
#define STATUS_INTERVAL			2*60*1000		//Every Two Mins

// System Messages payload option
#define REBOOT_SYSTEM_CMD		"reboot"
#define DISABLE_SYSTEM_CMD		"disable"
#define ENABLE_SYSTEM_CMD		"enable"
#define DEBUG_ON_SYSTEM_CMD		"debug-on"
#define DEBUG_OFF_SYSTEM_CMD	"debug-off"
#define OLED_ENABLE				"oled-enable"
#define OLED_DISABLE			"oled-disable"
#define STATUS_LED_ENABLE		"statusled-enable"
#define STATUS_LED_DISABLE		"statusled-disable"

// Configuration file name
#define SENSOR_CONFIG_FILE "sensor.conf"


#define conf 	(*Config::getInstance())

class Config {

	public:
		inline bool oled() { return _oled; }
		inline bool debug() { return _debug; }
		inline int statusLED() { return _statusLED; }

		static Config *getInstance();
		void parse(char *);

		void setOLED(bool);
		void setDebug(bool);
		void setStatusLED(bool);
	private:
		Config();
		
		void setFileBasedStatus(const char *, bool );
		bool doesFileExist(const char *);
		
		static Config *_instance;
		bool _oled;
		bool _debug;
		int _statusLED;
};
#endif