#ifndef NESTOR_LOGGER_H
#define NESTOR_LOGGER_H

#include <Arduino.h>

#define DEBUG_LOG_TOPIC "home/nodemcu/debug/"

#define logger (*NestorLogger::getInstance())

class NestorLogger {
	public:

		class LogNode {
			public:
				LogNode() { }
				~LogNode() { ; }
				String line;
				LogNode *next;
		};

		static NestorLogger *getInstance(); 

		void print(const char *line);
		void println(const char *line);

		void print(int line);
		void println(int line);

		void print(const __FlashStringHelper *line);// { print((const char *)line); }
		void println(const __FlashStringHelper *line);// { println((const char *)line); }
		void println();
		
		void enable() 	{ _debug = true; }
		void disable()	{ _debug = false; }

		void enableWebLog() { _weblog = true; }
		void disableWebLog() { _weblog = false; }

		void setMaxLogLines(int maxLoglines) { _maxLogLines = maxLoglines; }
		NestorLogger::LogNode* getHead() { return _head; }

	private:
		NestorLogger();

		void printInternal(const char *line, bool);
		void printInternal(int, bool);

		bool _debug;
		bool _weblog;

		LogNode *_head = 0;
		LogNode *_tail = 0;

		int _nodeCount;

		int _maxLogLines;
		static NestorLogger* _instance;
};

#endif