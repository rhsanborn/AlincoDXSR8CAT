/*
  RadioHead.cpp - Status of head side of the radio
*/

#include "RadioHead.h"

RadioHead::RadioHead(HardwareSerial& serial)
  : _serial(serial)
{
  _phyHeadSWD = "";
}

void RadioHead::processSerial(char sendData){

  //Act as passthrough until we get to a command.
  //Don't want to interupt instructions mid-cycle.

  _phyHeadSWD += sendData;
  if(sendData == '\n'){
    //Act as passthrough until we get to a complete command.
    //Don't want to interupt instructions mid-cycle.
    if(_phyHeadSWD[1] != 'S')
      sendSWD(_phyHeadSWD);
    if(_phyHeadSWD[3] == 'S')
      _phyHeadSWD = validateSWDS( _phyHeadSWD);
    if(_phyHeadSWD[3] == 'R')
      _phyHeadSWD = validateSWDR( _phyHeadSWD);
    if(_phyHeadSWD[3] == 'V')
      _phyHeadSWD = radioSWDV( _phyHeadSWD);

     _phyHeadSWD = "";
  }
}

void RadioHead::sendSWD(String SWD){
  for(int i = 0; i < SWD.length(); i++)
      _serial.write(SWD[i]);
}

void RadioHead::sendButton(unsigned int buttons){
  sendSWD(createSWDS(buttons));  
}

int RadioHead::intFromAscii(char ascii){
    if(ascii <= 57)
        return ascii - '0';
    else
        return ascii - 55;
}

char RadioHead::asciiFromInt(int x){
    if(x < 10)
        return x + '0';
    else if (x >= 10 && x < 16)
        return x + 55;
    else
        return 0;
}

String RadioHead::createSWDS(unsigned int bitRegister){
    String SWDS = "SWDS";
    for(int i = 0; i < 7; i++){
        SWDS += asciiFromInt((bitRegister >> ((6-i)*4)) & 15);
    }
    SWDS += "00100\r\n";
    return SWDS;
}

String RadioHead::createSWDV(){
    String SWDV = "SWDV";
    SWDV += catRIT >> 4;
    SWDV += catRIT & 15;
    SWDV += catIF >> 4;
    SWDV += catIF & 15;
    SWDV += catSquelch >> 4;
    SWDV += catSquelch & 15;
    SWDV += catVol >> 4;
    SWDV += catVol & 15;

    SWDV += "00100\r\n";
    return SWDV;
}

String RadioHead::validateSWDS(String SWDS){
  //SWDS000000000100
  return SWDS;  
}

String RadioHead::validateSWDR(String SWDR){
  //SWDR800100
  return SWDR;
}

String RadioHead::radioSWDV(String SWDV){
  //SWDV000000000100
  radRIT = (intFromAscii(SWDV[4])<<4) | intFromAscii(SWDV[5]);
  radIF = (intFromAscii(SWDV[6])<<4) | intFromAscii(SWDV[7]);
  radSquelch = (intFromAscii(SWDV[8])<<4) | intFromAscii(SWDV[9]);
  radVol = (intFromAscii(SWDV[10])<<4) | intFromAscii(SWDV[11]);
  return SWDV;
}

int RadioHead::getVol(){
  return radVol;
}

void RadioHead::setVol(int vol){
    catVol = vol;
    sendSWD(createSWDV);
}