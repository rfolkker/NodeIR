#include "JsonObject.h"

JsonObject::JsonObject(){
	_size = 0;
  _maxSize=10;
	// Zero the array
	// for(int counter = 0; counter < _maxSize; counter++){
	//	_keys[counter] = "";
	//	_values[counter] = "";
	//}
}

bool JsonObject::Append(String newKey, String newValue){
	if(_size<_maxSize){
		_keys[_size] = newKey;
		_values[_size] = newValue;
		_size++;
		return true;
	}
	return false;
}
bool JsonObject::Delete(String byKey){
	return Delete(GetIndex(byKey));
}

int JsonObject::GetIndex(String byKey){
  // Serial.println("Finding key: "+byKey);
	for(int counter= 0; counter< _size;counter++){
    // Serial.println("Checking key: "+_keys[counter]);
		if(byKey==_keys[counter]){
      // Serial.println("Found key");
			return counter;
		}
	}
 Serial.println("Key was not found");
	return -1; // Not found
}

bool JsonObject::Delete(int byIndex){
	if(byIndex < _size){
		// move everything down by 1, and decrease the size
		for(int counter = byIndex;counter<_size-1;counter++){
			_keys[counter] = _keys[counter+1];
			_values[counter] = _values[counter+1];
		}
		_keys[_size-1] = "";
		_values[_size-1] = "";
		_size--;
		return true;
	}
	return false;
}

bool JsonObject::Has(String byKeys){
	return GetIndex(byKeys) > -1;
}

bool JsonObject::Set(String byKey, String newValue, bool concat){
	int checkIndex = GetIndex(byKey);
 	if(checkIndex > -1){
        if(concat)
          return Set(checkIndex, GetValue(byKey)+","+newValue);
        else
          return Set(checkIndex, newValue);
 	}
	else{ // we have a new one, check for concat or replace
  		return Append(byKey, newValue);
    
	}
}

bool JsonObject::Set(int byIndex, String newValue){
	if(byIndex < 0)
		return false;
	if(byIndex < _maxSize){
		_values[byIndex] = newValue;
		return true;
	}
	return false;
}
JsonObject JsonObject::Parse(String fromString){
	// Json object starts with a 
  String aKey = "";
  String aValue = "";
  String aBuffer = "";
  JsonObject retValue;
  // Walk through each character
  // For now if it is a { or } ignore
  // Same with Quotes, should add proper quote handling in the future

  for(int counter = 0; counter<fromString.length();counter++){
    // If we run into a colon, we have a key, if we run into a comma or the end we have a value
    switch(fromString.charAt(counter)){
      case ':':
        aKey=aBuffer;
        Serial.println("Found Key: "+aKey);
        aBuffer = "";
        break;
      case ',':
        aValue=aBuffer;
        aBuffer="";
        // Add the value to the json object
        retValue.Set(aKey, aValue,true);
        break;
      case '\r':
        break;
      case '\n':
        break;
      case '{':
        // Do nothing
        break;
      case ' ':
        // Do nothing
        break;
      case '}':
        // Do nothing
        break;
      case ']':
        // Do nothing
        break;
      case '[':
        // Do nothing
        break;
      case '"':
        // Do nothing
        break;
      default:
        aBuffer+=fromString.charAt(counter);
        break;
    }
  }
  if(fromString.length()> 1){
    Serial.println("Found Value: "+aBuffer);
    retValue.Set(aKey, aBuffer, true);
  }
  return retValue;
}

String JsonObject::GetKey(int byIndex){
  if(byIndex < 0)
    return "";
  if(byIndex>=_size)
    return "";
  return _keys[byIndex];
}
String JsonObject::GetValue(int byIndex){
  if(byIndex < 0)
    return "";
  if(byIndex>=_size)
    return "";
  return _values[byIndex];
}
String JsonObject::GetValue(String byKey){
  return GetValue(GetIndex(byKey));
}
bool JsonObject::GetValueArray(String byKey, short unsigned int* toValues, int maxCount){
  // take the value at the key, split it, and assign it by int type
  String parseValue = GetValue(byKey);
  String aBuffer = "";
  int index = 0;
  for(int counter = 0; counter<parseValue.length();counter++){
    if(parseValue.charAt(counter)==','){
      // we have an integer to parse
      toValues[index++] = aBuffer.toInt();
      if(index>=maxCount){
        return false;
      }
      aBuffer = "";
    }
    else if(parseValue.charAt(counter)!=' '){
      aBuffer+=parseValue.charAt(counter);
    }
  }
  toValues[index] = aBuffer.toInt();
  return true;
}

