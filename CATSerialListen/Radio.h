/*
  Radio.h - Status of xcver side of the radio
*/

#ifndef Radio_h
#define Radio_h

#include <Arduino.h>

class Radio
{
  public:
    Radio(HardwareSerial& radioSerial);
    void processLCSA(unsigned short recvLCSA[]);   
    void sendSWDV();
    
    int getFreq();
    int getVol();
    void setVol(unsigned short vol);
    void setRITOn();
    void setXitOn();
    void setITVal(int it);
    bool getRitOn();
    bool getXitOn();
    int getITVal();
    void setIFVal(int IF);
    void setIFOn();
    int getIFVal();
    bool getIFOn();
    void setSquelch(unsigned short squelch);
    int getSquelch();
    int getRFGain();
    char getAGC();
    bool getNarrow();
    bool getNB();
    unsigned short getToneType();
    unsigned short getTone();
    bool getSplitOn();

    void setRadVol(unsigned short vol);
    void setRadIT(int IT);
    void setRadIF(int IF);
    void setRadSquelch(unsigned short squelch);

    void PTTDown();
    void PTTUp();
    

  private:
    void sendSWD(String SWD);
    void sendButton(unsigned int buttons);
    String createSWDV();
    String createSWDS(unsigned int bitRegister);

    HardwareSerial& _radioSerial;
    RadioHead& _head;

    char asciiFromInt(int x)
    char lcdToChar(unsigned int lcd);
    bool _ready;
    int _freq;
    char _mode[3];
    short _rfGain;
    char _agc;
    bool _func;
    bool _lock;
    bool _multiFunc;
    bool _nar;
    bool _nb;
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
    bool _split;
    unsigned short _catIT = 0;
    bool _cITRcv = false;
    unsigned short _catIF = 0;
    bool _cIFRcv = false;
    unsigned short _catSquelch = 0;
    bool _cSqRcv = false;
    unsigned short _catVol = 0;
    bool _cVolRcv = false;


};

#endif
