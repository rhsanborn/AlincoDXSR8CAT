/*
  Radio.cpp - Process Alinco Screen Messages
*/

#include "Radio.h"
#include "Arduino.h"


Radio::Radio(HardwareSerial& radioSerial, RadioSerialHandler& serialHandler)
    : _radioSerial(radioSerial), _serialHandler(serialHandler)
{}

/*
Radio::Radio(HardwareSerial& radioSerial)
    : _radioSerial(radioSerial)
{}
*/
long Radio::getFreq(){
    return _freq;
}

int Radio::getVol(){
    if(_cVolRcv)
        return _catVol;
    else
        return _radVol;
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
int Radio::getIFVal(){
    return _radIFVal;
}

bool Radio::getIFOn(){
    return _ifOn;
}

void Radio::setSquelch(unsigned short squelch){
    _cSqRcv = true;
    _catSquelch = squelch;
    sendSWD(createSWDV());
}

int Radio::getSquelch(){
    return _radSquelch;
}

int Radio::getRFGain(){
    return _rfGain;
}

char Radio::getAGC(){
    return _agc;
}

char Radio::getMode(){
    return '2';
}

bool Radio::getNarrow(){
    return _nar;
}

bool Radio::getNB(){
    return _nbOn;
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

void Radio::setVol(unsigned short vol){
    _cVolRcv = true;
    _catVol = vol;
    sendSWD(createSWDV());
}

void Radio::setIFVal(int IF){
    _cIFRcv = true;
    _catIF = IF;
    sendSWD(createSWDV());
}

void Radio::setIFOn(){
   //May not exist
}

void Radio::setRitOn(bool ritOn){
    int ritBut = 6;

    if(ritOn){
        if(!_ritOn){
            if(_xitOn){
                sendButton(ritBut);
                sendButton(ritBut);
                sendButton(ritBut);
            } else {
                sendButton(ritBut);
            }
        }
    } else{
        if(_ritOn){
            if(_xitOn){
                sendButton(ritBut);
            } else {
                sendButton(ritBut);
                sendButton(ritBut);
                sendButton(ritBut);
            }
        }
    }
}

void Radio::setXitOn(bool xitOn){
    int xitBut = 6;

    if(xitOn){
        if(!_xitOn){
            if(_ritOn){
                sendButton(xitBut);
            } else {
                sendButton(xitBut);
                sendButton(xitBut);
                sendButton(xitBut);
            }
        }
    }
    else
        if(_xitOn){
            if(_ritOn){
                sendButton(xitBut);
                sendButton(xitBut);
                sendButton(xitBut);
            } else {
                sendButton(xitBut);
            }
        }
}

void Radio::setITVal(int it){
  _cITRcv = true;
  _catIT = it;
  sendSWD(createSWDV());
}

void Radio::setNBOn(bool nbOn){
    int nbBut = 18;

    if(nbOn){
        if(!_nbOn){
            sendFuncButton(nbBut);
        }
    }
    else
        if(_nbOn){
            sendFuncButton(nbBut);   
        }
}

void Radio::setFreq(long freq){
    /*
    int freqDiff = freq - _freq;
    if(abs(freqDiff) < 3000){
        _serialHandler.dialChange(freqDiff);
        return;
    }*/

    int mhz = freq / 1000000;

    switch(mhz){
      case 28:
          sendButton(27);
          break;
      case 24:
          sendButton(18);
          break;
      case 21:
          sendButton(19);
          break;
      case 18:
          sendButton(20);
          break;
      case 14:
          sendButton(21);
          break;
      case 10:
          sendButton(22);
          break;
      case 7:
          sendButton(23);
          break;
      case 5:
          sendButton(24);
          break;
      case 3:
          sendButton(25);
          break;
      case 1:
          sendButton(26);
          break;      
    }

    sendButton(17);

    long mult = 100000;
    for(int i = 0; i < 5; i++){
        int digit = freq / mult % 10;
        mult /= 10;

        switch(digit){
            case 0:
                sendButton(27);
                break;
            case 9:
                sendButton(18);
                break;
            case 8:
                sendButton(19);
                break;
            case 7:
                sendButton(20);
                break;
            case 6:
                sendButton(21);
                break;
            case 5:
                sendButton(22);
                break;
            case 4:
                sendButton(23);
                break;
            case 3:
                sendButton(24);
                break;
            case 2:
                sendButton(25);
                break;
            case 1:
                sendButton(26);
                break;   
        }
    }
}
//SWDS Button Indexes
//0,1,2,3,4,5,6,7,8  ,9 ,10,11,12,13 ,14,15,16,17   ,18 ,19  ,20,21 ,22 ,23  ,24,25,26  ,27

//27,26,25,24,23,22,21,20,19, 18,17,16,15, 14,13,12,11,10,     9,   8, 7,  6,  5,   4,3,2,1,   0
// 3, 2, 1, 0, 7, 6, 5, 4,ENT,* , 9, 8,PTT, _, _, _,RF,M/KHz,V/M,MODE,MF,KEY,RIT,FUNC,_,_,DOWN,UP

//0,1,2   ,3 ,4 ,5  ,6  ,7   ,8 ,9    ,10 ,11  ,12 ,13,14,15,16 ,17,18,19,20,21,22,23,24,25,26,27
//_,_,DOWN,UP,MF,KEY,RIT,FUNC,RF,M/KHz,V/M,MODE,PTT, _, _, _,ENT,* , 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

void Radio::Test(){
    int testBut = 17;
    sendButton(testBut);
}

void Radio::PTTUp(){
    int clearBut = -1;
    sendButton(clearBut);
}

void Radio::PTTDown(){
    int pttBut = 12;
    sendButton(pttBut);
}

void Radio::setRadVol(unsigned short vol){
    
    if(vol <= 255){
      //Check if user has turned knobs to override the CAT values
      if(abs(vol - _radVol) > 2)
        _cVolRcv = false;
      _radVol = vol;
    }
}

void Radio::setRadIT(int IT){
    if(IT <= 1200 || IT >= -1200)
      //Check if user has turned knobs ot override the CAT values
      if(abs(IT - _radITVal)>2)
        _cITRcv = false;
      _radITVal = IT;     
}

void Radio::setRadIF(int IF){
    if(IF <= 1500 || IF >= -1500)
      //Check if user has turned knobs ot override the CAT values
      if(abs(IF - _radIFVal)>2)
        _cIFRcv = false;
      _radIFVal = IF;
}

void Radio::setRadSquelch(unsigned short squelch){
    if(squelch <= 255)
      //Check if user has turned knobs ot override the CAT values
      if(abs(_radSquelch-_catSquelch)>2)
        _cSqRcv = false;
      _radSquelch = squelch;
}

void Radio::setRadFreq(long freq){
    _freq = freq;
}

void Radio::setRadRFGain(short rfGain){
    _rfGain = rfGain;
}

void Radio::setRadAGC(char agc){
    _agc = agc;
}

void Radio::setRadFunc(bool func){
    _func = func; 
}

void Radio::setRadLock(bool lock){
    _lock = lock;
}

void Radio::setRadMultiFunc(bool mfunc){
    _multiFunc = mfunc;
}

void Radio::setRadNB(bool nb){
    _nbOn = nb;
}

void Radio::setRadTone(bool tone){
    _tone = tone;
}

void Radio::setRadTune(bool tune){
    _tune = tune;
}

void Radio::setRadSplit(bool split){
    _split = split;
}
    
void Radio::setRadXIT(bool xit){
    _xitOn = xit;
}
    
void Radio::setRadRIT(bool rit){
    _ritOn = rit;
}

void Radio::sendSWDV(){
    sendSWD(createSWDV());
}

void Radio::sendSWD(String SWD){
  //for(int i = 0; i < SWD.length(); i++)
      //_radioSerial.write(SWD[i]);

  _serialHandler.addSerialCommand(SWD);
}

void Radio::sendFuncButton(int button){
    int funcBut = 7;
    
    sendButton(funcBut);
    sendButton(button);
}

void Radio::sendButton(int button){
  long bitRegister = 1;

  if(button == -1){
      //User wants to indicate no buttons pressed
      sendSWD(createSWDS(0));
  } else if(button == 12){
      //User wants to send PTT. Don't clear button until
      //asked to tdo so.
      bitRegister <<= button;
      sendSWD(createSWDS(bitRegister)); 
  } else{
      bitRegister <<= button;
      sendSWD(createSWDS(bitRegister));
      sendSWD(createSWDS(0));
  }  
}

String Radio::createSWDS(unsigned long bitRegister){
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

    sendIT = _cITRcv ? _catIT : _radITVal;
    sendIF = _cIFRcv ? _catIF : _radIFVal;
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

