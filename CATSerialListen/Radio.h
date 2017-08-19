/*
  Radio.h - Status of xcver side of the radio
*/

#ifndef Radio_h
#define Radio_h

#include "RadioSerialHandler.h"
#include <Arduino.h>

class Radio
{
  public:
    Radio(HardwareSerial& radioSerial, RadioSerialHandler& serialHandler);
    //Radio(HardwareSerial& radioSerial);
    void sendSWDV();
    void sendSWD(String SWD);
    String createSWDS(unsigned long bitRegister);
    
    long getFreq();
    int getVol();
    bool getRitOn();
    bool getXitOn();
    int getITVal();
    int getIFVal();
    bool getIFOn();
    char getMode();
    int getSquelch();
    int getRFGain();
    char getAGC();
    bool getNarrow();
    bool getNB();

    void setSquelch(unsigned short squelch);
    void setIFVal(int IF);
    void setIFOn();
    void setVol(unsigned short vol);
    void setRitOn(bool ritOn);
    void setXitOn(bool xitOn);
    void setITVal(int it);
    void setNBOn(bool nbOn);
    void setFreq(long freq);

    unsigned short getToneType();
    unsigned short getTone();
    bool getSplitOn();

    void setRadVol(unsigned short vol);
    void setRadIT(int IT);
    void setRadIF(int IF);
    void setRadSquelch(unsigned short squelch);

    void setRadFreq(long freq);
    void setRadRFGain(short rfGain);
    void setRadAGC(char agc);
    void setRadFunc(bool func);
    void setRadLock(bool lock);
    void setRadMultiFunc(bool mfunc);
    void setRadNB(bool nb);
    void setRadTone(bool tone);
    void setRadTune(bool tune);
    void setRadSplit(bool split);
    void setRadXIT(bool xit);
    void setRadRIT(bool rit);

    void Test();
    void PTTDown();
    void PTTUp();
    
  private:
    void sendButton(int button);
    void sendFuncButton(int button);
    String createSWDV();

    HardwareSerial& _radioSerial;
    RadioSerialHandler& _serialHandler;

    short comDelay = 30;

    char asciiFromInt(int x);
    char lcdToChar(unsigned int lcd);
    bool _ready;
    long _freq;
    char _mode[3];
    short _rfGain;
    char _agc;
    bool _func;
    bool _lock;
    bool _multiFunc;
    bool _nar;
    bool _nbOn;
    bool _tone;
    bool _tune;
    bool _split;
    int _radVol;
    bool _ritOn;
    bool _xitOn;
    int _radITVal;
    bool _ifOn;
    int _radIFVal;
    unsigned short _radSquelch;
    int _catIT = 0;
    bool _cITRcv = false;
    int _catIF = 0;
    bool _cIFRcv = false;
    unsigned short _catSquelch = 0;
    bool _cSqRcv = false;
    unsigned short _catVol = 0;
    bool _cVolRcv = false;

};

#endif
