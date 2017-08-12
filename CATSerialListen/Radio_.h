/*
  Radio.h - Status of xcver side of the radio
*/

#ifndef Radio_h
#define Radio_h

#include <Arduino.h>

class Radio
{
  public:
    Radio();
    void processLCSA(unsigned short recvLCSA[]);   
    int getFreq();
    int getVol();
    void setRITOn();
    void setXitOn();
    void setITVal(int vol);
    bool getRitOn();
    bool getXitOn();
    int getRitVal();
    void setIFVal(int vol);
    void setIFOn();
    int getIFVal();
    bool getIFOn();
    void setSquelch(int vol);
    int getSquelch();
    int setVol(int vol);
    int getRFGain();
    char getAGC();
    char getFunc();
    //char getLock();
    //char getMultiFunc();
    bool getNarrow();
    bool getNB();
    unsigned short getToneType();
    unsigned short getTone();
    //char getTune();
    bool getSplitOn();
    

  private:
    
    int RFPower(unsigned short recvLCSA[]);
    char AGC(unsigned short recvLCSA[]);
    unsigned int getDigit_1(int first, int second);
    unsigned int getDigit_2(int first, int second);
    unsigned int getDigit_3(int first, int second);
    unsigned int getDigit_4(int first, int second);
    char lcdToChar(unsigned int lcd);
    int freq;
    char mode[3];
    short rfGain;
    char agc;
    bool func;
    bool lock;
    bool multiFunc;
    bool nb;
    bool tone;
    bool tune;
    bool split;

};

#endif





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

