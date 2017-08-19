#include <Arduino.h>
#include "Radio.h"
#include "RadioHead.h"
#include "RadioScreen.h"
#include "RadioSerialHandler.h"
#include "CAT.h"

char serReadData = 0;

const int strLength = 90;
//const int lenSWDS = 18;
//const int numKeys = 23;

char ser[strLength + 1];
int serLoc = 0;
int ser2[strLength + 1];
int ser2Loc = 0;

unsigned short lcsa[34];

RadioSerialHandler serialHandler(Serial2, Serial);
Radio radio(Serial2, serialHandler);
//Radio radio(Serial2);
RadioHead head(radio);
//RadioScreen screen(radio);
RadioScreen screen(radio, Serial);
CAT cat(Serial,radio);

unsigned long prevMillis = 0;
int interval = 3000;

void setup() {
  // put your setup code here, to run once:

  //Start serial port at 38400 bps:
  //Serial is the computer
  Serial.begin(38400);
  //Serial1 is the head of the radio
  Serial1.begin(38400);
  //Serial2 is the main body of the radio
  Serial2.begin(38400);
}

void loop() {

    unsigned long currentMillis = millis();
  
    if (Serial2.available() > 0) {
      serReadData = Serial2.read();
      Serial1.write(serReadData);
  
      ser2[ser2Loc] = serReadData;
      ser2Loc++;
      ser2[ser2Loc] = 0;
      
      if(ser2[ser2Loc - 1] == 'A' && ser2[ser2Loc - 2] == 'S' && 
        ser2[ser2Loc - 3] == 'C' &&ser2[ser2Loc - 4] == 'L'){
        
        String outstring = "";
        
        for(int i = 0; i < 34; i++)
          lcsa[i] = ser2[i];
  
        screen.processLCSA(lcsa);
  
        if(currentMillis - prevMillis > interval) {
          // save the last time you blinked the LED 
          prevMillis = currentMillis;

          /*
          Serial.println("");
          for(int i = 0; i < 34; i++){
            Serial.print(lcsa[i]);
            Serial.print(" ");
          }
          Serial.println("");
          String freq = String(radio.getFreq() * 10);
          freq = padZeros(freq, 11);
          Serial.println(freq);
          Serial.println(radio.getRFGain());
          Serial.println(radio.getAGC());
          Serial.println((radio.getNB() ? "true" : "false"));
          //Serial.println((radio.getTone() ? "true" : "false"));
          //Serial.println((radio.getTune() ? "true" : "false"));
          Serial.println("Volume: ");
          Serial.println(head.getVol());
          Serial.println("Volume _rad: ");
          Serial.println(radio.getVol());
          */
        }
          
        ser2Loc = 4;
        ser2[0] = 'L';
        ser2[1] = 'C';
        ser2[2] = 'S';
        ser2[3] = 'A';
        ser2[4] = 0;                              
      }
    }

    if(serialHandler.readyToSend())
        serialHandler.sendNext();
  
    while(Serial1.available() > 0){
        serReadData = Serial1.read();
        head.processSerial(serReadData);
    }

    while(Serial.available() > 0){
        serReadData = Serial.read();
        cat.processSerial(serReadData);  
    }
}

String padZeros(String string, int zeros){
    int len = string.length();
    if(len > zeros)
        return string;
    //return string;
    for(int i = 0; i <= (zeros - len); i++)
        string = "0" + string;    
    return string;
}
