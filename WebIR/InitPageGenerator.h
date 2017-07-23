#ifndef INITPAGEGENERATOR_H
#define INITPAGEGENERATOR_H

#include <Arduino.h>
class InitPageGenerator{
private:
    String pageFile="";
public:
    InitPageGenerator();
    String GetDefaultPageFile();
};

#endif