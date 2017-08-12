/*
  Radio.cpp - Status of xcver side of the radio
*/

#include "Radio.h"

Radio::Radio(){

}

void Radio::processLCSA(unsigned short recvLCSA[]){
    char digits[7];
    digits[0] = lcdToChar(getDigit_1(recvLCSA[9],recvLCSA[10]));
    digits[1] = lcdToChar(getDigit_1(recvLCSA[11],recvLCSA[12]));
    digits[2] = lcdToChar(getDigit_1(recvLCSA[13],recvLCSA[14]));
    digits[3] = lcdToChar(getDigit_1(recvLCSA[15],recvLCSA[16]));
    digits[4] = lcdToChar(getDigit_2(recvLCSA[26],recvLCSA[27]));
    digits[5] = lcdToChar(getDigit_3(recvLCSA[24],recvLCSA[25]));
    digits[6] = lcdToChar(getDigit_4(recvLCSA[22],recvLCSA[23]));

    bool isFreq = true;
    String tempFreq = "";
    for(int i = 0; i < 7; i++){
        int value = digits[i]-'0';
        tempFreq += digits[i];
        if(value < 0 || value > 9)
            isFreq = false;
    }

    if(isFreq)
        freq = tempFreq;

    rfGain = RFPower(recvLCSA);
    agc = AGC(recvLCSA);

    if(recvLCSA[9] & (1<<4))
        func = true;
    else
        func = false;
    if(recvLCSA[11] & (1<<4))
        lock = true;
    else
        lock = false;
    if(recvLCSA[29] & (1<<0))
        multiFunc = true;
    else
        multiFunc = false;
    if(recvLCSA[26] & (1<<4))
        nb = true;
    else
        nb = false;   
    if(recvLCSA[23] & (1<<0))
        tone = true;
    else
        tone = false;
    if(recvLCSA[6] & (1<<4))
        tune = true;
    else
        tune = false;
    if(recvLCSA[6] & (1<<6))
        split = true;
    else
        split = false;

} 

int Radio::getFreq(){
    return freq;
}

int Radio::RFPower(unsigned short recvLCSA[]){
    if(recvLCSA[28] & (1<<4))
        return 10;
    if(recvLCSA[30] & (1<<4))
        return 0;
    if(recvLCSA[5] & (1<<0))
        return -10;

    return -20;
}

int Radio::getRFGain(){
    return rfGain;
}

char Radio::getAGC(){
   return agc; 
}

char Radio::getFunc(){
   return func; 
}

char Radio::getLock(){
   return lock; 
}

char Radio::getMultiFunc(){
   return multiFunc; 
}

char Radio::getNB(){
   return nb; 
}

char Radio::getTone(){
   return tone; 
}

char Radio::getTune(){
   return tune; 
}

char Radio::getSplit(){
   return split; 
}

char Radio::AGC(unsigned short recvLCSA[]){
    char tempAGC;
    if(recvLCSA[4] & (1<<4))
        tempAGC = 'S';
    else if(recvLCSA[31] & (1<<0))
        tempAGC = 'F';
    else
        tempAGC = 'E';

    return tempAGC;
}

unsigned int Radio::getDigit_1(int first, int second){
    // A C D E G HKMNPRSTUXY
    //1514131211109876543210
    //TSKAyGUHxDPCERMN
    unsigned int digit = 0;

    //T
    if(first & (1<<0))
        digit |= (1<<3);
    //S
    if(first & (1<<1))
        digit |= (1<<4);
    //K
    if(first & (1<<2))
        digit |= (1<<9);
    //A    
    if(first & (1<<3))
        digit |= (1<<15);    
    //Y
    if(first & (1<<4))
        digit |= (1<<0);
    //G
    if(first & (1<<5))
        digit |= (1<<11);
    //U
    if(first & (1<<6))
        digit |= (1<<2);
    //H
    if(first & (1<<7))
        digit |= (1<<10);    
    //X
    if(second & (1<<0))
        digit |= (1<<1);
    //D
    if(second & (1<<1))
        digit |= (1<<13);
    //P
    if(second & (1<<2))
        digit |= (1<<6);
    //C
    if(second & (1<<3))
        digit |= (1<<14);
    //E
    if(second & (1<<4))
        digit |= (1<<12);
    //R
    if(second & (1<<5))
        digit |= (1<<5);
    //M
    if(second & (1<<6))
        digit |= (1<<8);
    //N
    if(second & (1<<7))
        digit |= (1<<7);

    return digit;
}

unsigned int Radio::getDigit_2(int first, int second){
    // A C D E G HKMNPRSTUXY
    //1514131211109876543210
    //ERMNyDPCaGUHTSKA
    unsigned int digit = 0;

    //E
    if(first & (1<<0))
        digit |= (1<<12);
    //R
    if(first & (1<<1))
        digit |= (1<<5);
    //M
    if(first & (1<<2))
        digit |= (1<<8);
    //N    
    if(first & (1<<3))
        digit |= (1<<7);
    //Y
    if(first & (1<<4))
        digit |= (1<<0);
    //D
    if(first & (1<<5))
        digit |= (1<<13);
    //P
    if(first & (1<<6))
        digit |= (1<<6);
    //C
    if(first & (1<<7))
        digit |= (1<<14);
    //A    
    if(second & (1<<0))
        digit |= (1<<15);    
    //G
    if(second & (1<<1))
        digit |= (1<<11);
    //U
    if(second & (1<<2))
        digit |= (1<<2);
    //H
    if(second & (1<<3))
        digit |= (1<<10);    
    //T
    if(second & (1<<4))
        digit |= (1<<3);
    //S
    if(second & (1<<5))
        digit |= (1<<4);
    //K
    if(second & (1<<6))
        digit |= (1<<9);
    //A    
    if(second & (1<<7))
        digit |= (1<<15);    

    return digit;
}

unsigned int Radio::getDigit_3(int first, int second){
    // A C D E G HKMNPRSTUXY
    //1514131211109876543210
    //FRMNEDPCxGUHTSKA
    unsigned int digit = 0;

    //E
    if(first & (1<<0))
        digit |= (1<<12);
    //R
    if(first & (1<<1))
        digit |= (1<<5);
    //M
    if(first & (1<<2))
        digit |= (1<<8);
    //N    
    if(first & (1<<3))
        digit |= (1<<7);
    //E
    if(first & (1<<4))
        digit |= (1<<12);
    //D
    if(first & (1<<5))
        digit |= (1<<13);
    //P
    if(first & (1<<6))
        digit |= (1<<6);
    //C
    if(first & (1<<7))
        digit |= (1<<14);
    //X
    if(second & (1<<0))
        digit |= (1<<1);
    //G
    if(second & (1<<1))
        digit |= (1<<11);
    //U
    if(second & (1<<2))
        digit |= (1<<2);
    //H
    if(second & (1<<3))
        digit |= (1<<10);    
    //T
    if(second & (1<<4))
        digit |= (1<<3);
    //S
    if(second & (1<<5))
        digit |= (1<<4);
    //K
    if(second & (1<<6))
        digit |= (1<<9);
    //A    
    if(second & (1<<7))
        digit |= (1<<15);    

    return digit;
}

unsigned int Radio::getDigit_4(int first, int second){
    // A C D E G HKMNPRSTUXY
    //1514131211109876543210
    //ERMNyDPCxGUHTSKA
    unsigned int digit = 0;

    //E
    if(first & (1<<0))
        digit |= (1<<12);
    //R
    if(first & (1<<1))
        digit |= (1<<5);
    //M
    if(first & (1<<2))
        digit |= (1<<8);
    //N    
    if(first & (1<<3))
        digit |= (1<<7);
    //Y
    if(first & (1<<4))
        digit |= (1<<0);
    //D
    if(first & (1<<5))
        digit |= (1<<13);
    //P
    if(first & (1<<6))
        digit |= (1<<6);
    //C
    if(first & (1<<7))
        digit |= (1<<14);
    //X    
    if(second & (1<<0))
        digit |= (1<<1);
    //G
    if(second & (1<<1))
        digit |= (1<<11);
    //U
    if(second & (1<<2))
        digit |= (1<<2);
    //H
    if(second & (1<<3))
        digit |= (1<<10);    
    //T
    if(second & (1<<4))
        digit |= (1<<3);
    //S
    if(second & (1<<5))
        digit |= (1<<4);
    //K
    if(second & (1<<6))
        digit |= (1<<9);
    //A    
    if(second & (1<<7))
        digit |= (1<<15);    

    return digit;
}


char Radio::lcdToChar(unsigned int lcd){
    //Need digits, don't care about LCD X and Y (decimal point and arrow)
    //65532 = 11111111 11111100
    unsigned int mask = 65532;
    lcd &= mask;

    switch(lcd){
        case 64648:
            return '0';
        case 24576:
            return '1';
        case 55364:
            return '2';
        case 61508:
            return '3';
        case 25668:
            return '4';
        case 46144:
            return '5';
        case 48196:
            return '6';
        case 57344:
            return '7';
        case 64580:
            return '8';
        case 62532:
            return '9';
        default:
            return 'Z';

    }

}

