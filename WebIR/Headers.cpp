// Fixed width header object for simple use
#include "Headers.h"

Headers::Headers(){
	_Host = "";
	_UserAgent = "";
	_Accept = "";
	_AcceptLanguage = "";
	_AcceptEncoding = "";
	_ContentLength = 0;
	_ContentLengthText = "";
	_Connection = "";
};

bool Headers::Add(String newData){
	int index = -1;
	String useValue=newData;
  String compareValue = "";
	bool retVal = false;
  useValue.remove(0,1);
	// Serial.println("Adding ["+useValue+"] to Header");
	for(int counter = 0; counter < _KeyCount;counter++){
    compareValue = useValue.substring(0, _Keys[counter].length());
    // Serial.println("Searching for substring: "+_Keys[counter]);
    // Serial.println("Found: "+compareValue);
		if(_Keys[counter]== compareValue){
			index = counter;
			useValue.remove(0,_Keys[index].length());
			break;
		}
	}
	
	if(index==-1)
		return false;
	
	switch(index){
		case 0:
			_Host = useValue;
			retVal = true;
			break;
		case 1:
			_UserAgent = useValue;
			retVal = true;
			break;
		case 2:
			_Accept = useValue;
			retVal = true;
			break;
		case 3:
			_AcceptLanguage = useValue;
			retVal = true;
			break;
		case 4:
			_AcceptEncoding = useValue;
			retVal = true;
			break;
    case 5:
      _ContentType = useValue;
      break;
		case 6:
			_ContentLengthText = useValue;
			_ContentLength  = _ContentLengthText.toInt();
			retVal = true;
			break;
		case 7:
			_Connection = useValue;
			retVal = true;
			break;
	}
	return retVal;
};
