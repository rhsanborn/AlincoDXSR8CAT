/*
  Radio.h - Status of xcver side of the radio
*/

#ifndef Radio_h
#define Radio_h

#include <Arduino.h>

class RadioHead
{
  public:
    RadioHead();
    String processSWDS(String SWDS);
    String processSWDR(String SWDR);
    String radioSWDV(String SWDV);
    void processSerial(char sendData);
    unsigned short getVol();
    int getIF();
    int getIT();
    unsigned short getSquelch();
    

  private:
    int intFromAscii(char ascii);

    String _phyHeadSWD = "";
    unsigned int head_buttons = 0;
    unsigned int send_buttons = 0;
    unsigned short radIT = 0;
    unsigned short radIF = 0;
    unsigned short radSquelch = 0;
    unsigned short radVol = 0;

};

#endif

