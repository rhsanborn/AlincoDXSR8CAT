#include "CAT.h"


CAT::CAT(HardwareSerial& serial, Radio& radio)
  : _serial(serial), _radio(radio)
{
  serIn = "";
}

void CAT::processSerial(char serData){
  //We don't care about special chars and escape chars
  if(serData > 32 && serData < 126)
    serIn += serData;

  if(serData == ';'){

    //Test command to poke at radio.
    if(serIn[0] == 'T' && serIn[1] == 'S'){
      long freq = 14073000;
      int freqDiff = freq - _radio.getFreq();
      _serial.println(freqDiff);
      _serial.println(abs(freqDiff));
      _radio.setFreq(freq);
      serIn = "";
      return;
    }

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
        if(serIn[3] == ';')
          _serial.print(qAFGain());
        else if (serIn.length() == 7)
          sAFGain(serIn);
      serIn = "";
      return;
      }
    }

    //Auto Information Command
    if(serIn[0] == 'A' && serIn[1] == 'I'){
      _serial.print("AI0;");
      serIn = "";
      return;
    }

    //VFO-A information Command
    if(serIn[0] == 'F' && serIn[1] == 'A'){
        if(serIn[2] == ';'){
            //This is a query
            String resp = "FA";
            resp += padZeros(String(_radio.getFreq()), 11) 
                + ';';
            _serial.print(resp);
        } else if(serIn[2] == '0' && serIn[3] == '0'){
              bool isFreq = true;
              long freq = 0;
              for(int i = 2; i < 13; i++){
                  int value = serIn[i]-'0';
                  freq = (freq * 10) + serIn[i]-'0';
                  if(value < 0 || value > 9){
                      isFreq = false;
                  }
              }
              if(isFreq && freq != _radio.getFreq())
                  _radio.setFreq(freq);
        }
      serIn = "";
      return;
    }

    //VFO-B information Command
    if(serIn[0] == 'F' && serIn[1] == 'B'){
        //This is always a query
        String resp = "FB";

        resp += padZeros(String(_radio.getFreq()), 11) 
            + ';';

        _serial.print(resp);
      serIn = "";
      return;
    }


    //Noise Blanker Command
    if(serIn[0] == 'N' && serIn[1] == 'B'){
        //This is a query
        if(serIn[2] == ';'){
            String resp = "NB";
                resp += _radio.getNB() ? '1' : '0';
                resp += ';';
            _serial.print(resp);
        }
        else if(serIn[2] == '0')
            _radio.setNBOn(false);
        else if(serIn[2] == '1')
            _radio.setNBOn(true);
      serIn = "";
      return;
    }

    //Rit on/off Command
    if(serIn[0] == 'R' && serIn[1] == 'T'){
        //This is a query
        if(serIn[2] == ';'){
            String resp = "RT";
                resp += _radio.getRitOn() ? '1' : '0';
                resp += ';';
            _serial.print(resp);
        }
        else if(serIn[2] == '0'){
            _radio.setRitOn(false);
        }
        else if(serIn[2] == '1'){
            _radio.setRitOn(true);
        }
      serIn = "";
      return;
    }

    //Temp IT Val Command
    if(serIn[0] == 'I' && serIn[1] == 'T'){
        //This is a query
        if(serIn[2] == ';')                
            _serial.println("IT Val: ");
            _serial.print(_radio.getITVal());
      serIn = "";
      return;
    }

    //Xit on/off Command
    if(serIn[0] == 'X' && serIn[1] == 'T'){
        //This is a query
        if(serIn[2] == ';'){
            String resp = "XT";
            resp += _radio.getXitOn() ? '1' : '0';
            resp += ';';
            _serial.print(resp);
        }
        else if(serIn[2] == '0')
            _radio.setXitOn(false);
        else if(serIn[2] == '1')
            _radio.setXitOn(true);
      serIn = "";
      return;
    }

    //Radio ID Command 019 = TS-2000
    if(serIn[0] == 'I' && serIn[1] == 'D'){
      if(serIn[2] = ';')
        _serial.print("ID019;");
      serIn = "";
      return;
    }

    //IF information Command
    if(serIn[0] == 'I' && serIn[1] == 'F'){
        //This is always a query
        String resp = "IF";
        String stpSz = "0000";
        String ritFrq = "000000";

        resp += padZeros(String(_radio.getFreq()), 11) 
            + stpSz 
            + ritFrq 
            + (_radio.getRitOn() ? '1' : '0')
            + (_radio.getXitOn() ? '1' : '0')
            + '0' + '0' + '0' //channel bank
            + '0' // RX, TX = 1
            + _radio.getMode()
            + '0' //VFO A
            + '0' //Scan off
            + '0' //Split off
            + '0'//Tone off
            + '0' + '0' //Tone val = 0
            + '0' //Shift neutral
            + ';';

        _serial.print(resp);
      serIn = "";
      return;
    }

    //Mode Command 019 = TS-2000
    //1: LSB, 2: USB, 3: CW, 4: FM, 5: AM, 
    //6: FSK, 7: CR-R, 8: Reserved, 9: FSK-R
    if(serIn[0] == 'M' && serIn[1] == 'D'){
      if(serIn[2] = ';')
        _serial.print("MD2;");
      serIn = "";
      return;
    }

    serIn = "";
  }
}

String CAT::qAFGain(){
    String gainResp = "";
    int gain = _radio.getVol();
    gainResp += (gain/100) % 10;
    gainResp += (gain/10) % 10;
    gainResp += gain % 10;

    gainResp = "AG0" + gainResp + ";";
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

String CAT::padZeros(String string, int zeros){
    if(string.length() > zeros)
        return string;
    for(int i = 0; i < string.length() - zeros; i++)
        string = "0" + string;    

    return string;
}

