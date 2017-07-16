#include "SimpleObjectNotation.h"

SimpleObjectNotation::SimpleObjectNotation()
{
    _maxSize = 0;
    _size = 0;
}
bool SimpleObjectNotation::Parse(String fromString)
{
  String aBuffer = "";
  // Walk through each character
  // For now if it is a { or } ignore
  // Same with Quotes, should add proper quote handling in the future

  for(int counter = 0; counter<fromString.length(); counter++){
    // If we run into a colon, we have a key, if we run into a comma or the end we have a value
    switch(fromString.charAt(counter)){
      case ':':
        OnKey(aBuffer);
        aBuffer = "";
        break;
      case ',':
        OnValue(aBuffer);
        aBuffer="";
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
      case '\\':
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
    OnValue(aBuffer);
  }

  return true;
}
