#include <Arduino.h>
#include "Radio.h"
#include "RadioHead.h"
#include "CAT.h"

char serReadData = 0;

const int strLength = 90;
const int lenSWDS = 18;
const int numKeys = 23;

char ser[strLength + 1];
int serLoc = 0;

int ser2[strLength + 1];
int ser2Loc = 0;

unsigned short lcsa[34];

Radio radio;
RadioHead head(Serial2);
CAT cat(Serial,head);

unsigned long prevMillis = 0;
int interval = 3000;

char recvSWDS[lenSWDS] = {'S','W','D','S','0','0','0','0','0','0','0','0','0','1','0','0','\r','\n'};
char SWDSToSend[lenSWDS] = {'S','W','D','S','0','0','0','0','0','0','0','0','0','1','0','0','\r','\n'};
char blankSWDS[lenSWDS] = {'S','W','D','S','0','0','0','0','0','0','0','0','0','1','0','0','\r','\n'};

//'MF','1','2','3','4','5','6','7','8','9','*','0','ENT','FUNC','RIT','KEY','MODE','V/M','M/KHz','RF','UP','DOWN','PTTDn','PTTUp'
int keys[] = {72,34,33,32,43,42,41,40,51,50,49,35,48,75,74,73,67,66,65,64,83,82,56};
bool keysPressed[numKeys];


void setup() {
  // put your setup code here, to run once:

  // start serial port at 9600 bps:
  Serial.begin(38400);
  Serial1.begin(38400);
  Serial2.begin(38400);

  for(int i = 0; i < numKeys; i++)
    keysPressed[i] = false;

  Serial.println("Setup complete. Listening.");
}

void loop() {

  unsigned long currentMillis = millis();

  if (Serial2.available() > 0) {
    serReadData = Serial2.read();
    Serial1.write(serReadData);

    ser2[ser2Loc] = serReadData;
    ser2Loc++;
    ser2[ser2Loc] = 0;
    //Serial.print(serReadData);
    
    if(ser2[ser2Loc - 1] == 'A' && ser2[ser2Loc - 2] == 'S' && 
      ser2[ser2Loc - 3] == 'C' &&ser2[ser2Loc - 4] == 'L'){
      
      String outstring = "";
      
      for(int i = 0; i < 34; i++)
        lcsa[i] = ser2[i];

      radio.processLCSA(lcsa);

      if(currentMillis - prevMillis > interval) {
        // save the last time you blinked the LED 
        prevMillis = currentMillis;
        
        Serial.println("");
        for(int i = 0; i < 34; i++){
          Serial.print(lcsa[i]);
          Serial.print(" ");
        }
        Serial.println("");
        Serial.println(radio.getFreq());
        Serial.println(radio.getRFGain());
        Serial.println(radio.getAGC());
        Serial.println((radio.getFunc() ? "true" : "false"));
        Serial.println((radio.getLock() ? "true" : "false"));
        Serial.println((radio.getMultiFunc() ? "true" : "false"));
        Serial.println((radio.getNB() ? "true" : "false"));
        Serial.println((radio.getTone() ? "true" : "false"));
        Serial.println((radio.getTune() ? "true" : "false"));
        Serial.println("Volume: ");
        Serial.print(head.getVol());
      }
        
      ser2Loc = 4;
      ser2[0] = 'L';
      ser2[1] = 'C';
      ser2[2] = 'S';
      ser2[3] = 'A';
      ser2[4] = 0;                              
    }
  }

  
  while(Serial1.available() > 0){
    serReadData = Serial1.read();
    head.processSerial(serReadData);
  }

  while(Serial.available() > 0){
     serReadData = Serial.read();
     cat.processSerial(serReadData);  
  }
}
