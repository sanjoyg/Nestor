#include <core/ESP8266PinMap.h>
#include <unistd.h>
#include <string.h>
#include <Arduino.h>

int ESP8266PinMap::getPin(char *pinStr) {
	if (pinStr == NULL || strlen(pinStr) == 0)
		return -1;
	int len = strlen(pinStr);
	if (pinStr[len-1] == '\r' || pinStr[len-1] == '\n') 
		pinStr[len-1] = NULL;
	if (stricmp(pinStr,"D0") == 0)
		return D0;
	if (stricmp(pinStr,"D1") == 0)
		return D1;
	if (stricmp(pinStr,"D2") == 0)
		return D2;
	if (stricmp(pinStr,"D3") == 0) 
		return D3;
	if (stricmp(pinStr,"D4") == 0)
		return D4;
	if (stricmp(pinStr,"D5") == 0)
		return D5;
	if (stricmp(pinStr,"D6") == 0)
		return D6;
	if (stricmp(pinStr,"D7") == 0)
		return D7;
	if (stricmp(pinStr,"D8") == 0)
		return D8;
	if (stricmp(pinStr,"D9") == 0)
		return D9;
	if (stricmp(pinStr,"D10") == 0)
		return D10;
	return -1;
}