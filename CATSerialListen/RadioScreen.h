/*
  RadioScreen.h - Process Alinco Screen Messages
*/

#ifndef RadioScreen_h
#define RadioScreen_h

#include <Arduino.h>

class RadioScreen
{
    public:
        RadioScreen();
        void processLCSA(unsigned short recvLCSA[])
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
        unsigned int getDigit_1(int first, int second);
        unsigned int getDigit_2(int first, int second);
        unsigned int getDigit_3(int first, int second);
        unsigned int getDigit_4(int first, int second);
        
        int RFPower(unsigned short recvLCSA[]);
        char AGC(unsigned short recvLCSA[]);
        char lcdToChar(unsigned int lcd);

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

