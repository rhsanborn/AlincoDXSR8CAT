#include "CAT.h"


CAT::CAT(HardwareSerial& serial, Radio& radio)
  : _serial(serial), _radio(radio)
{
  serIn = "";
}

void CAT::processSerial(char serData){
  serIn += serData;

  if(serData == ';'){

    //PTTDown and Up Commands
    if(serIn[0] == 'D' && serIn[1] == 'N'){
      _radio.PTTDown();
      serIn = "";
      return;
    }
    if(serIn[0] == 'U' && serIn[1] == 'P'){
      _radio.PTTUp();
      serIn = "";
      return;
    }

    //AFGain Command
    if(serIn[0] == 'A' && serIn[1] == 'G'){
      //Sanity check, this should always be 0
      if(serIn[2] == '0'){
        //Query
        if(serIn[3] = ';')
          qAFGain();
        else if (serIn.length() == 6)
          sAFGain(serIn);
      serIn = "";
      return;
      }
    }
    serIn = "";
  }
}

//SWDS Button Indexes
//0,1,2,3,4,5,6,7,8  ,10,11,12,13 ,14,15,16,17,18   ,19 ,20  ,21,22 ,23 ,24  ,25,26,27  ,28
//3,2,1,0,7,6,5,4,ENT,* ,9 ,8 ,PTT,_ ,_ ,_ ,RF,M/KHz,V/M,MODE,MF,KEY,RIT,FUNC,_ ,_ ,DOWN,UP

String CAT::qAFGain(){
    String gainResp = "";
    int gain = _radio.getVol();

    for(int i = 0; i < 3; i++){
        gainResp = gainResp + (gain % 10);
        gain /= 10;
    }

    gainResp = "AG0" + gainResp;
    return gainResp;
}

void CAT::sAFGain(String serIn){
    unsigned short gain = 0;

    for(int i = 3; i < 6; i++)
        gain = gain * 10 + intFromAscii(serIn[i]);

    if(gain <= 255)
        _radio.setVol(gain);
}

int CAT::intFromAscii(char ascii){
    if(ascii <= 57)
        return ascii - '0';
    else
        return ascii - 55;
}

/*
String CAT::qRadioInfo(){
    String info = "IF";
    
    String    

}*/
