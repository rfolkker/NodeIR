#ifndef SIMPLEOBJECTNOTATION_H
#define SIMPLEOBJECTNOTATION_H

#include <Arduino.h>
class SimpleObjectNotation{
	private:
		int _size;
		int _maxSize;
		String _keys[10];
		String _values[10];
	public:
		SimpleObjectNotation();
        bool Parse(String fromString);
        virtual bool OnKey(String newKey)=0;
        virtual bool OnValue(String newKey)=0;
};

#endif