/*
  Radio.h - Status of xcver side of the radio
*/

#ifndef RadioHead_h
#define RadioHead_h

#include "Radio.h"
#include <Arduino.h>

class RadioHead
{
  public:
    RadioHead(Radio& radio);
    String processSWDS(String SWDS);
    String processSWDR(String SWDR);
    String processSWDV(String SWDV);
    void processSerial(char sendData);
    unsigned short getVol();
    int getIF();
    int getIT();
    unsigned short getSquelch();
    

  private:
    int intFromAscii(char ascii);

    Radio& _radio;
    String _phyHeadSWD = "";
    unsigned int head_buttons = 0;
    unsigned int send_buttons = 0;
    unsigned short radIT = 0;
    unsigned short radIF = 0;
    unsigned short radSquelch = 0;
    unsigned short radVol = 0;

};

#endif

