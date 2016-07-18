#ifndef NESTOR_OLED_H
#define NESTOR_OLED_H

#define OLED_PRINT(x) if (conf.oled()) { NestorOLED::print(x); }

class NestorOLED {
	public:
		static void resetDisplay();
		static void displayOn();
		static void displayOff();
		static void clearDisplay();
		static void sendChar(unsigned char);
		static void sendCharXY(unsigned char, int , int);
		static void setXY(unsigned char, unsigned char);
		static void sendStr(unsigned char *);
		static void sendStrXY(const char *, int, int);
		static void init();
	
		static void print(const char *str);
		static void print(const __FlashStringHelper *line);
		
	private:
		static void sendCommand(unsigned char);

		static char _data[17*8];
		static int _line;
};

#endif
