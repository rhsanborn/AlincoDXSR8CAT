/*
  Radio.cpp - Process Alinco Screen Messages
*/

#include "Radio.h"
#include "RadioScreen.h"
#include "Arduino.h"


Radio::Radio(HardwareSerial& radioSerial)
    : _radioSerial(radioSerial)
{

}

int Radio::getFreq(){
    return _freq;
}

int Radio::getVol(){
    return _radVol;
}

void Radio::setVol(unsigned short vol){

}

void Radio::setRITOn(){

}

void Radio::setXitOn(){

}

void Radio::setITVal(int it){
  cITRcv = true;
  catIT = it;
  sendSWD(createSWDV());
}

bool Radio::getRitOn(){
    return _ritOn;
}

bool Radio::getXitOn(){
    return _xitOn;
}

int Radio::getITVal(){
    return _radITVal;
}

void Radio::setIFVal(int IF){
    _cIFRcv = true;
    _catIF = IF;
    _sendSWD(createSWDV());
}

void Radio::setIFOn(){

}

int Radio::getIFVal(){
    return _radIFVal;
}

bool Radio::getIFOn(){
    return _ifOn;
}

void Radio::setSquelch(unsigned short squelch){
    _cSqRcv = true;
    _catSquelch = squelch;
    _sendSWD(createSWDV());
}

int Radio::getSquelch(){
    return _radSquelch;
}

int Radio::setVol(unsigned short vol){
    _cVolRcv = true;
    _catVol = vol;
    sendSWD(createSWDV());
}

int Radio::getRFGain(){
    return _rfGain;
}

char Radio::getAGC(){
    return _agc;
}

bool Radio::getNarrow(){
    return _nar;
}

bool Radio::getNB(){
    return _nb;
}

unsigned short Radio::getToneType(){
    return 0;
}

unsigned short Radio::getTone(){
    return 0;
}

bool Radio::getSplitOn(){
    return _split;
}

void Radio::PTTDown(){
    unsigned int head_buttons = 0;
    sendButton(head_buttons);
}

void Radio::PTTUp(){
    unsigned int head_buttons = 0;
    head_buttons |= (1<<12);
    sendButton(head_buttons);
}

void Radio::setRadVol(unsigned short vol){
    if(vol <= 255)
        _radVol = vol;
    if(abs(_radVol-_catVol)>2)
        _cVolRcv = false;
}

void Radio::setRadIT(int IT){
    if(IT <= 1200 || IT >= -1200)
        _radITVal = IT;
    if(abs(_radITVal-_catIT)>2)
        _cITRcv = false;
}

void Radio::setRadIF(int IF){
    if(IF <= 1500 || IF >= -1500)
        _radIFVal = IF;
    if(abs(_radIFVal-_catIF)>2)
        _cIFRcv = false;
}

void Radio::setRadSquelch(unsigned short squelch){
    if(squelch <= 255)
        _radSquelch = squelch;
    if(abs(_radSquelch-_catSquelch)>2)
        _cSqRcv = false;
}

void Radio::sendSWDV(){
    sendSWD(createSWDV);
}

void Radio::sendSWD(String SWD){
  for(int i = 0; i < SWD.length(); i++)
      _serial.write(SWD[i]);
}

void Radio::sendButton(unsigned int buttons){
  sendSWD(createSWDS(buttons));  
}

String Radio::createSWDS(unsigned int bitRegister){
    String SWDS = "SWDS";
    for(int i = 0; i < 7; i++){
        SWDS += asciiFromInt((bitRegister >> ((6-i)*4)) & 15);
    }
    SWDS += "00100\r\n";
    return SWDS;
}

String Radio::createSWDV(){
    String SWDV = "SWDV";
    unsigned short sendIT, sendIF, sendSquelch, sendVol;

    //Check if user has turned knobs ot override the CAT values
    if(abs(_head.getIT() - _radIT) > 2)
        _cITRcv = false;
    if(abs(_head.getIF() - _radIF) > 2)
        _cIFRcv = false;
    if(abs(_head.getSquelch() - _radSquelch) > 2)
        _cSqRcv = false;
    if(abs(_head.getVol() - _radVol) > 2)
        _cVolRcv = false;


    sendIT = _cITRcv ? _catIT : _radIT;
    sendIF = _cIFRcv ? _catIF : _radIF;
    sendSquelch = _cSqRcv ? _catSquelch : _radSquelch;
    sendVol = _cVolRcv ? _catVol : _radVol;
    
    SWDV += sendIT >> 4;
    SWDV += sendIT & 15;
    SWDV += sendIF >> 4;
    SWDV += sendIF & 15;
    SWDV += sendSquelch >> 4;
    SWDV += sendSquelch & 15;
    SWDV += sendVol >> 4;
    SWDV += sendVol & 15;

    SWDV += "00100\r\n";
    return SWDV;
}

char Radio::asciiFromInt(int x){
    if(x < 10)
        return x + '0';
    else if (x >= 10 && x < 16)
        return x + 55;
    else
        return 0;
}

