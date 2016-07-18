#include <core/NestorWIFI.h>
#include <nestor/Config.h>
#include <ESP8266WiFi.h>
#include <Nestor.h>
#include <core/Sensor.h>
#include <core/SensorList.h>
#include <utils/NestorLogger.h>
#include <utils/HTMLUtil.h>
#include <ESP8266mDNS.h>
#include <utils/NestorTime.h>
#include <utils/NestorOLED.h>

static const char textTypeStr[]   PROGMEM  = "text/html";
static const char connStr[]       PROGMEM  = "Connection";
static const char closeStr[]      PROGMEM  = "close";
static const char accessCtrlStr[] PROGMEM  = "Access-Control-Allow-Origin";

NestorWIFI::NestorWIFI():_wifiServer(80),_otaUpdater(true) {
}

bool NestorWIFI::initialize() {
  if (WiFi.getMode() != WIFI_STA)
    WiFi.mode(WIFI_STA);
  sprintf(_hostName,"%d.ghoshnet.local",Nestor::getInstance()->getId());
	return connect();
}

bool NestorWIFI::connect() {
  OLED_PRINT("WiFi Attempt");
	logger.println();
	logger.print(F("Attempting to connect to : "));
	logger.print(WIFI_SSID);
	logger.print(F(" using "));
	logger.println(WIFI_PWD);

	WiFi.begin(WIFI_SSID,WIFI_PWD);
	
	int index = 0;
	while (!isConnected()) {
   	delay(500);
   	;
    yield();
    logger.print(".");
   	index++;
   	if (index == 100)
   		return false;
  }

  OLED_PRINT("WiFi Connected");

  logger.println();
  logger.print(F("WiFi connected : IP : "));
  logger.print(WiFi.localIP().toString().c_str());
  	  	
  MDNS.begin(_hostName);
  MDNS.addService("http", "tcp", 80);

  yield();
  wireHandlers();

  _otaUpdater.setup(&_wifiServer);
  yield();

  _wifiServer.begin();
  	
  logger.println();
  logger.print(F("OTA & WebSever started with local name "));
  logger.println(_hostName);
  	
  ;
  yield();

  Nestor::blink(4,500);
  return true;
}

void NestorWIFI::handleGet() {
  
   _wifiServer.send(200,F("textTypeStr"),"");
  
  _wifiServer.sendContent(HTMLUtil::getHomePreHTML());
  
  char buffer[250];
  sprintf(buffer,"Connected to %s with IP %s as %s since %s, System is %s.\r\n",
      WIFI_SSID,
      getIP(),
      getHostName(),
      nestorTime.formatTime(),
      (Nestor::getInstance()->isEnabled() ? "enabled" : "disabled")); 

  _wifiServer.sendContent(buffer);
  
  _wifiServer.sendContent(HTMLUtil::getHomeMidHTML());

  SensorList::SensorNode *sensorNode = Nestor::getInstance()->getSensorList()->getHead();
  Sensor *sensor = NULL;
  bool noSensor = true;
  while (sensorNode != NULL) {
    sensor = sensorNode->obj;
    if (sensor == NULL) {
      continue;
    }
    sprintf(buffer,HTMLUtil::getHomeSensorHTML(),sensor->toString().c_str());

    _wifiServer.sendContent(buffer);
    
    noSensor = false;
    sensorNode = sensorNode->next;
  }
  
  if (noSensor) {
    _wifiServer.sendContent(F("No Sensors Found!"));
  }
  
  _wifiServer.sendContent(HTMLUtil::getHomePostHTML());
  _wifiServer.sendContent("\r\n");
 }

void NestorWIFI::handlePost() {
  HTTPUpload& upload = _wifiServer.upload();
  if (upload.status == UPLOAD_FILE_START) {
    SPIFFS.begin();
    SPIFFS.remove(SENSOR_CONFIG_FILE);
    logger.println();
    logger.print(F("Upload writing start for : "));
    logger.println(upload.filename.c_str());
    _uploadFP = SPIFFS.open(upload.filename.c_str(),"w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    logger.println(F("Writing to file..."));
    if (_uploadFP) {
      _uploadFP.write(upload.buf,upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    logger.println(F("Upload complete..."));
    if (_uploadFP) {
      _uploadFP.flush();
      _uploadFP.close();
    }
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    SPIFFS.remove(SENSOR_CONFIG_FILE);
    logger.println(F("Aborted, file removed."));
  }
}

void NestorWIFI::handleLogs() {
  NestorLogger::LogNode *head = logger.getHead();
  if (head == NULL) {
    _wifiServer.send(200,textTypeStr,F("<html><h2>No Logs!</h2></html>"));
    return;
  }
  
  _wifiServer.send(200,textTypeStr,"");

  String logStr;
  _wifiServer.sendContent(HTMLUtil::getLogPreHTML());  
  
  while (head != NULL) {
    logStr = logStr + "\n";
    logStr = logStr + head->line;
    
    if (logStr.length() > 2048) {
      logStr = "";
      _wifiServer.sendContent(logStr);
    } 

    head = head->next;  
  }

  if (logStr.length() > 0)
      _wifiServer.sendContent(logStr);
    
  _wifiServer.sendContent(HTMLUtil::getLogPostHTML());
  _wifiServer.sendContent("\r\n");
}

void NestorWIFI::wireHandlers() {
  _wifiServer.onNotFound([&]() {handleGet();});
	_wifiServer.on("/",HTTP_GET,[&]() {handleGet();});

  _wifiServer.on("/log", HTTP_GET,[&]() {handleLogs();} );
  _wifiServer.on("/logs",HTTP_GET,[&]() {handleLogs();} );

  _wifiServer.on("/enable", HTTP_GET,[&]() {
    _wifiServer.sendHeader(connStr,closeStr);
    _wifiServer.sendHeader(accessCtrlStr,"*");
    _wifiServer.send(200,textTypeStr,F("<html><h1>Node Enabled.</h1></html>"));
    Nestor::getInstance()->enable();
  });

  _wifiServer.on("/disable", HTTP_GET,[&]() {
    _wifiServer.sendHeader(connStr,closeStr);
    _wifiServer.sendHeader(accessCtrlStr,"*");
    _wifiServer.send(200,textTypeStr,F("<html><h1>Node Disabled.</h1></html>"));
    Nestor::getInstance()->disable();
  });

_wifiServer.on("/enabledebug", HTTP_GET,[&]() {
    _wifiServer.sendHeader(connStr,closeStr);
    _wifiServer.sendHeader(accessCtrlStr,"*");
    _wifiServer.send(200,textTypeStr,F("<html><h1>Debug Enabled.</h1></html>"));
    NestorLogger::getInstance()->enable();
  });

  _wifiServer.on("/disabledebug", HTTP_GET,[&]() {
    _wifiServer.sendHeader(connStr,closeStr);
    _wifiServer.sendHeader(accessCtrlStr,"*");
    _wifiServer.send(200,textTypeStr,F("<html><h1>Debug Disabled.</h1></html>"));
    NestorLogger::getInstance()->disable();
  });

  _wifiServer.on("/reboot",HTTP_GET,[&]() {
    _wifiServer.sendHeader(connStr,closeStr);
    _wifiServer.sendHeader(accessCtrlStr,"*");
    _wifiServer.send(200,textTypeStr,F("<html><head><META http-equiv=\"refresh\" content=\"30;URL=/home\"></head><body><h1>Rebooting....</h1></body></html>"));
    Nestor::getInstance()->reboot();
  });

	_wifiServer.on("/conf",HTTP_GET,[&](){
		_wifiServer.sendHeader(connStr,closeStr);
		_wifiServer.sendHeader(accessCtrlStr,"*");
		_wifiServer.send(200,textTypeStr,HTMLUtil::getConfUploadHTML());		
	});
	
	_wifiServer.on("/conf", HTTP_POST,[&]() {
		_wifiServer.sendHeader(connStr,closeStr);
		_wifiServer.sendHeader(accessCtrlStr,"*");
		_wifiServer.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
    	},[&](){ handlePost(); });
}

bool NestorWIFI::isConnected() { 
	return WiFi.status() == WL_CONNECTED; 
}

void NestorWIFI::loop() {
	if (!isConnected()) {
		connect();
    Nestor::getInstance()->publishStatus("WiFi Reconnected");
	} else {
		_wifiServer.handleClient();
	}
}