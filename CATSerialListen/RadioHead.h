/*
  RadioHead.h - Status of xcver side of the radio
*/

#ifndef RadioHead_h
#define RadioHead_h

#include <Arduino.h>

class RadioHead
{
  public:
    RadioHead(HardwareSerial& serial);
    String validateSWDS(String SWDS);
    String validateSWDR(String SWDR);
    String radioSWDV(String SWDV);
    void processSerial(char sendData);
    void sendButton(unsigned int buttons);
    String createSWDV();
    int getVol();
    int setVol(int vol);
  private:
    int intFromAscii(char ascii);
    char asciiFromInt(int x);
    void sendSWD(String SWD);
    String createSWDS(unsigned int bitRegister);

    HardwareSerial& _serial;
    String _phyHeadSWD = "";
    unsigned int head_buttons = 0;
    unsigned int send_buttons = 0;
    unsigned short catRIT = 0;
    bool cRITRcv = false;
    unsigned short catIF = 0;
    bool cIFRcv = false;
    unsigned short catSquelch = 0;
    bool cSqRcv = false;
    unsigned short catVol = 0;
    bool cVolRcv = false;
    unsigned short radRIT = 0;
    unsigned short radIF = 0;
    unsigned short radSquelch = 0;
    unsigned short radVol = 0;
};

#endif
