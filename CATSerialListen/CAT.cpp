#include "CAT.h"


CAT::CAT(HardwareSerial& serial, RadioHead& head)
  : _serial(serial), _head(head)
{
  serIn = "";
}

void CAT::processSerial(char serData){
  serIn += serData;

  if(serData == '\n'){
    if(serIn[0] == 'D' && serIn[1] == 'N'){
      PTTDown();
      serIn = "";
      return;
    }
    if(serIn[0] == 'U' && serIn[1] == 'P'){
      PTTUp();
      serIn = "";
      return;
    }
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

void CAT::PTTDown(){
  unsigned int head_buttons = 0;
  head_buttons |= (1<<12);
  _head.sendButton(head_buttons);
}

void CAT::PTTUp(){
  unsigned int head_buttons = 0;
  _head.sendButton(head_buttons);
}

String CAT::qAFGain(){
  return "true";
}

void CAT::sAFGain(String gain){
  unsigned int head_buttons = 0;
  _head.sendButton(head_buttons);
}

int CAT::intFromAscii(char ascii){
    if(ascii <= 57)
        return ascii - '0';
    else
        return ascii - 55;
}

