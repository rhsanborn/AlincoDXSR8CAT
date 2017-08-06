/*
  CAT.h - Status of xcver side of the radio
*/

#ifndef CAT_h
#define CAT_h

#include <Arduino.h>
#include "RadioHead.h"

class CAT
{
  public:
    CAT(HardwareSerial& serial, RadioHead& head);
    void processSerial(char serData);
  private:
    HardwareSerial& _serial;
    RadioHead& _head;
    String serIn;
    void PTTDown();
    void PTTUp();
    int intFromAscii(char ascii){
    
};

#endif
