#include <sensors/BuzzerSwitch.h>
#include <utils/NestorLogger.h>

void BuzzerSwitch::write(float value) {
	if (value == 1) {
		logger.print("Buzzing the switch for ");
		logger.print(_buzzerDuration); logger.print("secs"); logger.println();
		digitalWrite(_pin, HIGH);
		delay(_buzzerDuration * 1000);
		digitalWrite(_pin, LOW);
	}
}

bool BuzzerSwitch::setMember(int index, char *value) {
	if (!RelaySwitch::setMember(index,value)) {
		if (index == 4) {
			int buzzerDuration = atoi(value);
			if (buzzerDuration == -1) {
				logger.println();
				logger.print("Invalid Buzzer duration specified : ");
				logger.println(value);
				return false;
			} 
			_buzzerDuration = buzzerDuration;	
		} 
	}
	return true;
}

String BuzzerSwitch::toString() {
	String toRet = "BuzzerSwitch : ";
	toRet += getName();
	toRet += " : Publish Topic : ";
	toRet += getTopic();
	toRet += " : ";
	toRet += " Read Frequency : ";
	toRet += getReadFrequency();
	toRet += " Pin : "; 
	toRet += getPin();
	toRet += " Buzzer Period : ";
	toRet += _buzzerDuration;
	return toRet; 
}