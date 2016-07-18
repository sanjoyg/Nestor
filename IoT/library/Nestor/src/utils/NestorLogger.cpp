#include <utils/NestorLogger.h>
#include <Nestor.h>
#include <core/NestorMQTT.h>
#include <nestor/config.h>

#define DEFAULT_MAX_LOG_LINES 10

NestorLogger *NestorLogger::_instance = NULL;

NestorLogger::NestorLogger() {
	_head = NULL;
	_tail = NULL;
	_debug = conf.debug();
	_nodeCount = 0;
	disableWebLog();
	_maxLogLines = DEFAULT_MAX_LOG_LINES;
}

NestorLogger* NestorLogger::getInstance() {
	if (_instance == NULL) {
		_instance = new NestorLogger();
	}
	return _instance;
}

void NestorLogger::print(const char *line) {
	if (!_debug) return;
	printInternal(line,false);
}

void NestorLogger::print(int line) {
	if (!_debug) return;
	printInternal(line,false);
}

void NestorLogger::println(const char *line) {
	if (!_debug) return;
	printInternal(line,true);
}

void NestorLogger::println(int line) {
	if (!_debug) return;
	printInternal(line,true);
}

void NestorLogger::println() {
	if (!_debug) return;
	Serial.println();
}

void NestorLogger::print(const __FlashStringHelper *line) {
	if (!_debug) return;
	PGM_P p = reinterpret_cast<PGM_P>(line);
    while (1) {
        uint8_t c = pgm_read_byte(p++);
        if (c == 0) break;
        Serial.print((char)c);
    }
}

void NestorLogger::println(const __FlashStringHelper *line) {
	if (!_debug) return;
	print(line);
    Serial.println();
}

void NestorLogger::printInternal(int line, bool newLine) {
	char buf[10];
	sprintf(buf,"%d",line);
	printInternal(buf,newLine);
}

void NestorLogger::printInternal(const char *line, bool newLine) {
	if (line == NULL)
		return;

	if (newLine) {
		Serial.println(line);
		if (_weblog) {
			_nodeCount++;
			NestorLogger::LogNode *node = new NestorLogger::LogNode();
			node->line = line;		
			node->next = NULL;
			
			if (_head == NULL) {
				_head = node;
				_tail = node;
			} else {
				_tail->next = node;
				_tail = node;
			}

			if (_nodeCount > _maxLogLines) {
				_nodeCount--;
				LogNode *toDel = _head;
				_head = _head->next;
				delete toDel;
				toDel = NULL;
			}
		}
	} else {
		Serial.print(line);
		if (_weblog) {
			if (_head == NULL) {
				_nodeCount++;
				NestorLogger::LogNode *node = new NestorLogger::LogNode();
				node->line=line;
				node->next = NULL;	
				_head = _tail = node;		
			} else {
				_tail->line += line;
			}
		}
	}
}