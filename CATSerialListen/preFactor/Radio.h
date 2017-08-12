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
    int getRFGain();
    char getAGC();
    char getFunc();
    char getLock();
    char getMultiFunc();
    char getNB();
    char getTone();
    char getTune();
    char getSplit();

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
