#ifndef SENSOR_LIST_H
#define SENSOR_LIST_H

class SensorListIter;
class Sensor;

class SensorList {
	public:
		struct SensorNode {
			Sensor *obj;
			SensorNode *next;
		};

		SensorList();
		~SensorList();

		void add(Sensor *);
		SensorNode *getHead() { return _head; }

	private:
		SensorNode *_head;
		SensorNode *_tail;

		int _first;
};
#endif