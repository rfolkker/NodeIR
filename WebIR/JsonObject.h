#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include <Arduino.h>
class JsonObject{
	private:
		int _size;
		int _maxSize;
		String _keys[10];
		String _values[10];
	public:
		JsonObject();
		bool Append(String newKey, String newValue);
		bool Delete(String byKey);
		bool Delete(int byIndex);
		bool Has(String byKeys);
		bool Set(String byKey, String newValue, bool concat);
		bool Set(int byIndex, String newValue);
		int  GetIndex(String byKey);
    String GetKey(int byIndex);
    String GetValue(int byIndex);
    String GetValue(String byKey);
    int GetSize(){return _size;};
		JsonObject Parse(String fromString);
    bool GetValueArray(String byKey, short unsigned int* toValues, int maxCount);
};

#endif
