/*
  Radio.cpp - Process Alinco Screen Messages
*/

#include "RadioScreen.h"


RadioScreen::RadioScreen(Radio& radio, HardwareSerial& comSerial)
  : _radio(radio), _comSerial(comSerial)
{

}

/*
RadioScreen::RadioScreen(Radio& radio)
  : _radio(radio)
{

}

*/

void RadioScreen::processLCSA(unsigned short recvLCSA[]){
    char digits[7];
    digits[0] = lcdToChar(getDigit_1(recvLCSA[9],recvLCSA[10]));
    digits[1] = lcdToChar(getDigit_1(recvLCSA[11],recvLCSA[12]));
    digits[2] = lcdToChar(getDigit_1(recvLCSA[13],recvLCSA[14]));
    digits[3] = lcdToChar(getDigit_1(recvLCSA[15],recvLCSA[16]));
    digits[4] = lcdToChar(getDigit_2(recvLCSA[26],recvLCSA[27]));
    digits[5] = lcdToChar(getDigit_3(recvLCSA[24],recvLCSA[25]));
    digits[6] = lcdToChar(getDigit_4(recvLCSA[22],recvLCSA[23]));

    bool isFreq = true;
    long tempFreq = 0;
    for(int i = 0; i < 7; i++){
        int value = digits[i]-'0';
        tempFreq = (tempFreq * 10) + digits[i]-'0';
        if(value < 0 || value > 9){
            isFreq = false;
        }
    }

    tempFreq = (tempFreq * 10);

    if(isFreq)
        _radio.setRadFreq(tempFreq);

    _radio.setRadRFGain(RFPower(recvLCSA));
    _radio.setRadAGC(AGC(recvLCSA));

    if(recvLCSA[9] & (1<<4))
        _radio.setRadFunc(true);
    else
        _radio.setRadFunc(false);

    if(recvLCSA[11] & (1<<4))
        _radio.setRadLock(true);
    else
        _radio.setRadLock(false);

    if(recvLCSA[29] & (1<<0))
        _radio.setRadMultiFunc(true);
    else
        _radio.setRadMultiFunc(false);

    if(recvLCSA[26] & (1<<4))
        _radio.setRadNB(true);
    else
        _radio.setRadNB(false);   
    
    if(recvLCSA[23] & (1<<0))
        _radio.setRadTone(true);
    else
        _radio.setRadTone(false);

    if(recvLCSA[6] & (1<<4))
        _radio.setRadTune(true);
    else
        _radio.setRadTune(false);

    if(recvLCSA[6] & (1<<6))
        _radio.setRadSplit(true);
    else
        _radio.setRadSplit(false);

    if(recvLCSA[20] & (1<<6))
        _radio.setRadXIT(true);
    else
        _radio.setRadXIT(false);

    if(recvLCSA[20] & (1<<7))
        _radio.setRadRIT(true);
    else
        _radio.setRadRIT(false);

    _radio.setRadIT(ITVal(recvLCSA[20],recvLCSA[21]));

} 

int RadioScreen::RFPower(unsigned short recvLCSA[]){
    if(recvLCSA[28] & (1<<4))
        return 10;
    if(recvLCSA[30] & (1<<4))
        return 0;
    if(recvLCSA[5] & (1<<0))
        return -10;

    return -20;
}

char RadioScreen::AGC(unsigned short recvLCSA[]){
    char tempAGC;
    if(recvLCSA[4] & (1<<4))
        tempAGC = 'S';
    else if(recvLCSA[31] & (1<<0))
        tempAGC = 'F';
    else
        tempAGC = 'E';

    return tempAGC;
}

unsigned int RadioScreen::getDigit_1(int first, int second){
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

unsigned int RadioScreen::getDigit_2(int first, int second){
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

unsigned int RadioScreen::getDigit_3(int first, int second){
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

unsigned int RadioScreen::getDigit_4(int first, int second){
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


char RadioScreen::lcdToChar(unsigned int lcd){
    //Need digits, don't care about LCD X and Y (decimal point and arrow)
    //65532 = 11111111 11111100
    //_comSerial.print("LCD out: ");
    //_comSerial.println(lcd);
    
    unsigned int mask = 65532;
    lcd &= mask;

    //_comSerial.print("After mask: ");
    //_comSerial.println(lcd);

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
        case 46148:
            return '5';
        case 48196:
            return '6';
        case 57344:
            return '7';
        case 64580:
            return '8';
        case 62532:
            return '9';
        case 0:
            return '0';
        default:
            return 'Z';

    }

}

short RadioScreen::ITVal(unsigned short first, unsigned short second){
    //A  B D G H I J K L M N
    //10 9 8 7 6 5 4 3 2 1 0
    //
    //
    //2  1  B  A  
    /* From Josh Myers work
    txit: TTT    1  AAAB   GGGG
                222 A  B   M  H
                 1  A  B   MLLH
    rxit: RRR       A  B   K  I
                    AAAB N JJJJ
    */

    unsigned int digit = 0;
    int value;

    //B
    if(first & (1<<2))
        digit |= (1<<9);
    //A    
    if(first & (1<<3))
        digit |= (1<<10);
    //G
    if(second & (1<<0))
        digit |= (1<<7);
    //H
    if(second & (1<<1))
        digit |= (1<<13);
    //I
    if(second & (1<<2))
        digit |= (1<<6);
    //J
    if(second & (1<<3))
        digit |= (1<<14);
    //M    
    if(second & (1<<4))
        digit |= (1<<1);
    //L
    if(second & (1<<5))
        digit |= (1<<11);
    //K
    if(second & (1<<6))
        digit |= (1<<2);
    //N - decimal - don't care
    //if(second & (1<<7))
        //digit |= (1<<10);    

    //Mask only uses the components that make up the 
    //second digit, no decimal point
    //A  B D G H I J K L M N
    //10 9 8 7 6 5 4 3 2 1 0
    //       1 1 1 1 1 1 1 0

    unsigned int mask = 254;

    switch(digit & mask){
        case 250:
            value = 0;
            break;
        case 96:
            value = 1;
            break;
        case 220:
            value = 2;
            break;
        case 244:
            value = 3;
            break;
        case 102:
            value = 4;
            break;
        case 182:
            value = 5;
            break;
        case 190:
            value = 6;
            break;
        case 224:
            value = 7;
            break;
        case 254:
            value = 8;
            break;
        case 246:
            value = 9;
            break;
        default:
            value = 0;
            break;
    }
    
    //A - Which would indicate a 0, if
    //not A, then it's a 1.
    if(!(first & (1<<3)))
        value = value + 10;

    value = value * 100;

    //1 - Indicates plus sign, if not 
    //lit, then value is negative
    if(!(first & (1<<1)))
        value = -value;
}

