/*
  RadioScreen.h - Process Alinco Screen Messages
*/

#ifndef RadioSerialHandler_h
#define RadioSerialHandler_h

#include <Arduino.h>

class RadioSerialHandler
{
    public:
        RadioSerialHandler(HardwareSerial& radioSerial, HardwareSerial& comSerial);
        //void addSerialCommand(String serial);
        bool readyToSend();
        void dialChange(int chgAmt);
        void sendNext();
        void sendButton(unsigned char button);
        void sendSWDV(unsigned short sendIT, unsigned short sendIF, unsigned short sendSquelch, unsigned short sendVol);
        void sendSWD(char *, int len);
        void sendSWDR(signed char swdr);
        void sendIncompleteSWD(char * incSt);

    private:
        HardwareSerial& _radioSerial;
        HardwareSerial& _comSerial;

        void createSWDS(unsigned char button);
        void createSWDV(unsigned short sendIT, unsigned short sendIF, unsigned short sendSquelch, unsigned short sendVol);
        char asciiFromInt(int x);
        void sendIncomSWD(char *);
        void createSWDR(signed char swdr);
      
        //SWDV7F85025A0100\r\n\0 - Len 19
        char _sendString[19] = {'\0'};
        char _incompleteString[30];
        unsigned long _lastSend = 0;
        static const int _comArrSize = 200;
        int _swdsAddLoc = 0;
        int _swdvAddLoc = 0;
        int _swdsNextLoc = 0;
        int _swdvNextLoc = 0;
        int _swdr = 0;
        bool _incStrRdy = false;
        int _swdsCnt = 0;
        int _swdvCnt = 0;
        int _offsetTime = 20;

        signed char _commandsSWDR[_comArrSize];
        int _swdrAddLoc = 0;
        int _swdrNextLoc = 0;
        int _swdrCnt = 0;
        
        //String _commandsSWDS[_comArrSize];
        //String _commandsSWDV[_comArrSize];

        unsigned char _commandsSWDS[_comArrSize];
        unsigned short _commandsSWDV[_comArrSize][4];
};

#endif

