#include <core/SensorList.h>
#include <stdlib.h>

SensorList::SensorList() {
	_head = NULL;
	_tail = NULL;
	_first = 0;
}

SensorList::~SensorList() {;}

void SensorList::add(Sensor *sensor) {
	SensorNode *node = new SensorNode();
	node->obj = sensor;
	node->next = NULL;

	if (_head == NULL) {
		_head = node;
		_tail = _head;
	} else {
		_tail->next = node;
		_tail = node;
	}
}