/*
  RadioScreen.h - Process Alinco Screen Messages
*/

#ifndef RadioScreen_h
#define RadioScreen_h

#include "Radio.h"
#include <Arduino.h>

class RadioScreen
{
    public:
        RadioScreen::RadioScreen(Radio& radio, HardwareSerial& comSerial);
        //RadioScreen(Radio& radio);
        void processLCSA(unsigned short recvLCSA[]);

    private:
        HardwareSerial& _comSerial;
    
        unsigned int getDigit_1(int first, int second);
        unsigned int getDigit_2(int first, int second);
        unsigned int getDigit_3(int first, int second);
        unsigned int getDigit_4(int first, int second);
        
        int RFPower(unsigned short recvLCSA[]);
        char AGC(unsigned short recvLCSA[]);
        char lcdToChar(unsigned int lcd);
        
        short ITVal(unsigned short first, unsigned short second);

        Radio& _radio;
  
        int _freq;
        char _mode[3];
        short _rfGain;
        char _agc;
        bool _func;
        bool _lock;
        bool _multiFunc;
        bool _nb;
        bool _tone;
        bool _tune;
        bool _split;

};

#endif

