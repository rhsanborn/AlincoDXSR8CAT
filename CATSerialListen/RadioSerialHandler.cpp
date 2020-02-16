/*
  RadioSerialHandler.cpp - Process Alinco Screen Messages
*/

#include "RadioSerialHandler.h"
#include "Arduino.h"


RadioSerialHandler::RadioSerialHandler(HardwareSerial& radioSerial, HardwareSerial& comSerial)
    : _radioSerial(radioSerial), _comSerial(comSerial)
{
}

/*
void RadioSerialHandler::addSerialCommand(String serial){
    if(serial[3] == 'S'){
        _commandsSWDS[_swdsAddLoc++] = serial;
        _swdsCnt++;
        if(_swdsAddLoc >= _comArrSize)
            _swdsAddLoc = 0;
    } else {
        _commandsSWDV[_swdvAddLoc++] = serial;
        _swdvCnt++;

        if(_swdvAddLoc >= _comArrSize)
            _swdvAddLoc = 0;
    }
}
*/

void RadioSerialHandler::dialChange(int chgAmt){
    _swdr += chgAmt;
}

bool RadioSerialHandler::readyToSend(){
    if(_swdrCnt > 0)
        return true;
    if(_swdvCnt > 0)
        return true;
    if(_swdsCnt > 0 && (millis() - _lastSend) > _offsetTime)
        return true;
    return false;
}

void RadioSerialHandler::sendNext(){
    if(_incStrRdy){
        _radioSerial.write(_incompleteString);
        //_comSerial.print("Sent incStr: ");
        //_comSerial.println(_incompleteString);

        _incStrRdy = false;
    }

    if(_swdrCnt > 0){
          while(_swdrCnt > 0){
             createSWDR(_commandsSWDR[_swdrNextLoc]);

             _radioSerial.print(_sendString);
             //_comSerial.print("Sending ");
             //_comSerial.print(_sendString);
             _swdrNextLoc++;
             _swdrCnt--;
             if(_swdrNextLoc >= _comArrSize)
                    _swdrNextLoc = 0;
          }
    }
    
    if(_swdvCnt > 0){
      
        while(_swdvCnt > 0){
            createSWDV(
                _commandsSWDV[_swdvNextLoc][0],
                _commandsSWDV[_swdvNextLoc][1],
                _commandsSWDV[_swdvNextLoc][2],
                _commandsSWDV[_swdvNextLoc][3]
            );
            _swdvNextLoc++;
            _radioSerial.print(_sendString);
            _swdvCnt--;
            if(_swdvNextLoc >= _comArrSize)
                _swdvNextLoc = 0;
        }
    }

    if(_swdsCnt > 0 && (millis() - _lastSend) > _offsetTime){
        createSWDS(_commandsSWDS[_swdsNextLoc++]);
        _radioSerial.print(_sendString);
        _swdsCnt--;
        if(_swdsNextLoc >= _comArrSize)
            _swdsNextLoc = 0;
        _lastSend = millis();
    }

}

void RadioSerialHandler::sendButton(unsigned char button){
    _commandsSWDS[_swdsAddLoc++] = button;
    _swdsCnt++;

    if(_swdsAddLoc >= _comArrSize)
        _swdsAddLoc = 0;
}

void RadioSerialHandler::sendIncompleteSWD(char * incSt){
    if(strlen(incSt) <= 30)
      _radioSerial.write(incSt);
      //strcpy(_incompleteString, incSt);

    //_radioSerial.write(_incompleteString);
    //_incStrRdy = true;
}

void RadioSerialHandler::sendSWDV(unsigned short sendIT, unsigned short sendIF, unsigned short sendSquelch, unsigned short sendVol){
    _commandsSWDV[_swdvAddLoc][0] = sendIT;
    _commandsSWDV[_swdvAddLoc][1] = sendIF;
    _commandsSWDV[_swdvAddLoc][2] = sendSquelch;
    _commandsSWDV[_swdvAddLoc][3] = sendVol;

    _swdvAddLoc++;    
    _swdvCnt++;

    if(_swdvAddLoc >= _comArrSize)
        _swdvAddLoc = 0;
}

void RadioSerialHandler::sendSWDR(signed char swdr){
    _commandsSWDR[_swdrAddLoc++] = swdr;
    _swdrCnt++;

    //_comSerial.println(swdr);
    //_comSerial.println(millis());
    //10 Millis between send and zero.
    
    if(_swdrAddLoc >= _comArrSize)
        _swdrAddLoc = 0;
}

void RadioSerialHandler::createSWDS(unsigned char button){
    long bitRegister = 1;
    if(button == 0)
        bitRegister = 0;
    bitRegister <<= button;

    _sendString[0] = 'S';
    _sendString[1] = 'W';
    _sendString[2] = 'D';
    _sendString[3] = 'S';

    for(int i = 0; i < 7; i++){
        _sendString[i+4] = asciiFromInt((bitRegister >> ((6-i)*4)) & 15);
    }

    //SWDS100000000100\r\n\0 - Len 19
    _sendString[11] = '0';
    _sendString[12] = '0';
    _sendString[13] = '1';
    _sendString[14] = '0';
    _sendString[15] = '0';
    _sendString[16] = '\r';
    _sendString[17] = '\n';
    _sendString[18] = '\0';

    //_comSerial.println(_sendString);
}

void RadioSerialHandler::createSWDR(signed char swdr){
    _sendString[0] = 'S';
    _sendString[1] = 'W';
    _sendString[2] = 'D';
    _sendString[3] = 'R';


    //if(swdr == -1){
    if(swdr >= 'A'){
        _sendString[4] = '7';
        _sendString[5] = swdr;
    //}else if(swdr == 1){
    }else if(swdr > '0' && swdr < 'A'){
        _sendString[4] = '8';
        _sendString[5] = swdr;
    }else{
        _sendString[4] = '8';
        _sendString[5] = '0';
    }

    _sendString[6] = '0';
    _sendString[7] = '1';
    _sendString[8] = '0';
    _sendString[9] = '0';
    _sendString[10] = '\r';
    _sendString[11] = '\n';
    _sendString[12] = '\0';
}

void RadioSerialHandler::createSWDV(unsigned short sendIT, unsigned short sendIF, unsigned short sendSquelch, unsigned short sendVol){
    _sendString[0] = 'S';
    _sendString[1] = 'W';
    _sendString[2] = 'D';
    _sendString[3] = 'V';

    //SWDV7E85025A0100\r\n\0 - Len 19
    _sendString[4] = asciiFromInt(sendIT >> 4);
    _sendString[5] = asciiFromInt(sendIT & 15);
    _sendString[6] = asciiFromInt(sendIF >> 4);
    _sendString[7] = asciiFromInt(sendIF & 15);
    _sendString[8] = asciiFromInt(sendSquelch >> 4);
    _sendString[9] = asciiFromInt(sendSquelch & 15);
    _sendString[10] = asciiFromInt(sendVol >> 4);
    _sendString[11] = asciiFromInt(sendVol & 15);

    _sendString[12] = '0';
    _sendString[13] = '1';
    _sendString[14] = '0';
    _sendString[15] = '0';
    _sendString[16] = '\r';
    _sendString[17] = '\n';
    _sendString[18] = '\0';
}



char RadioSerialHandler::asciiFromInt(int x){
    if(x < 10)
        return x + '0';
    else if (x >= 10 && x < 16)
        return x + 55;
    else
        return 0;
}

