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
    CAT(HardwareSerial& serial, Radio& radio);
    void processSerial(char serData);
    String qAFGain();
    void sAFGain(String gain);
    //String qRadioInfo();
  private:
    HardwareSerial& _serial;
    Radio& _radio;
    String serIn;
    int intFromAscii(char ascii);
};
#endif
