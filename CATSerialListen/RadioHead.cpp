/*
  RadioHead.cpp - Status of head side of the radio
*/

#include "RadioHead.h"

RadioHead::RadioHead(Radio& radio)
  : _radio(radio)
{
  _phyHeadSWD = "";
}

void RadioHead::processSerial(char sendData){

  _phyHeadSWD += sendData;
  if(sendData == '\n'){
      //Act as passthrough until we get to a complete command.
      //Don't want to interupt instructions mid-cycle.
      if(_phyHeadSWD[3] == 'S')
          _phyHeadSWD = processSWDS( _phyHeadSWD);
      if(_phyHeadSWD[3] == 'R')
          _phyHeadSWD = processSWDR( _phyHeadSWD);
      if(_phyHeadSWD[3] == 'V')
          _phyHeadSWD = processSWDV( _phyHeadSWD);

      _radio.sendSWD(_phyHeadSWD);
      
      _phyHeadSWD = "";
  }
}

int RadioHead::intFromAscii(char ascii){
    if(ascii <= 57)
        return ascii - '0';
    else
        return ascii - 55;
}

String RadioHead::processSWDS(String SWDS){
  //SWDS000000000100
  return SWDS;  
}

String RadioHead::processSWDR(String SWDR){
  //SWDR800100
  return SWDR;
}

String RadioHead::processSWDV(String SWDV){
  //SWDV000000000100
  radIT = (intFromAscii(SWDV[4])<<4) | intFromAscii(SWDV[5]);
  radIF = (intFromAscii(SWDV[6])<<4) | intFromAscii(SWDV[7]);
  radSquelch = (intFromAscii(SWDV[8])<<4) | intFromAscii(SWDV[9]);
  radVol = (intFromAscii(SWDV[10])<<4) | intFromAscii(SWDV[11]);

  _radio.setRadIT(radIT);
  _radio.setRadVol(radVol);
  _radio.setRadVol(radVol);
  _radio.setRadIF(radIF);
  _radio.setRadSquelch(radSquelch);
  
  _radio.sendSWDV();
  return SWDV;
}

unsigned short RadioHead::getVol(){
    return radVol;
}

int RadioHead::getIF(){
    return radIF;
}

int RadioHead::getIT(){
    return radIT;
}

unsigned short RadioHead::getSquelch(){
    return radSquelch;
}
