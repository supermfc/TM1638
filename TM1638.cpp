/*
 *  Tube display utilities for 74HC595 IC  
 *  Code by DingBing in jiaozuo city henan province China
	My QQ：396364218 and i love mcu & arduino.

 *  This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See Github project https://github.com/supermfc/TM1638 for latest
 */
#ifndef TM1638_cpp
#define TM1638_cpp

#include "TM1638.h"

unsigned char TM1638::LED_MODEL[17] = 
    { 
    // 0    1     2   3    4    5    6    7    8     9   A    B    C    D    E    F    - 
      0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0x8E,0xbf};

unsigned char TM1638::LedData[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

TM1638::TM1638(int dataPin,int sclkPin,int rclkPin)
{
	DIO = dataPin;
	SCLK = sclkPin;
	RCLK = rclkPin;
	
	pinMode(SCLK,OUTPUT);
	pinMode(RCLK,OUTPUT);
	pinMode(DIO,OUTPUT); 
}

void TM1638::setLetter(int place,char letter)
{
	if( (letter >= 'A') & ( letter <= 'F'))
		LedData[place] = LED_MODEL[letter-55];
	if( letter == '-')
		LedData[place] = LED_MODEL[16];
}

void TM1638::setDigital(int place,int digital)
{
	TM1638::LedData[place] = TM1638::LED_MODEL[digital];
}


// display a int number
void TM1638::displayInt(int num)
{
	char string[9]={0};
	sprintf(string,"%8d",num);
  
	for(int i = 7; i >=0; i--) 
	{
		switch(string[i])
		{
			case ' ':
				break;
			case '-':
				LedData[7-i] = LED_MODEL[16];
				break;
			default:
				LedData[7-i] = LED_MODEL[string[i]-'0'];
       // LedData[7-i] = LED_MODEL[1];
				break;       
		}
	}
	update();
}

//prec 小数位数
void TM1638::displayFloat(float number,int prec)
{
	char str[10]={0};
	int j = 9;
	int point = 0;
	dtostrf(number,8,prec,str);

	while(str[j]==0)
		j--;

	for(int i = 7; i >= 0; i--) 
	{
    
		switch(str[j])
		{
			case ' ':
				break;
			case '-':
				LedData[7-i] = LED_MODEL[16];
				break;
			case '.':
				LedData[7-i] &= 0x7F;
				i++;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				LedData[7-i]  &=  LED_MODEL[str[j]-'0']  ;
				//LedData[7-i] = LED_MODEL[3];
				break;     
     
		}
			// LedData[7-i] = LED_MODEL[i];
		j--;
   
	}

	update();

}

void TM1638::setNoPoint(int place)
{
	LedData[place] |= 0x80;
}

void TM1638::setPoint(int place)
{
	LedData[place] &= 0x7F;
}

void TM1638::closeDisplay()
{
	for(int i = 0; i < 8;i ++)
		LedData[i] = 0xFF;
	
	update();
}
void TM1638::update()
{
	for(int i = 0 ; i < 8; i++)
	{
	shiftOut(DIO,SCLK,MSBFIRST,LedData[i]);
	shiftOut(DIO,SCLK,MSBFIRST,1<<i);
    digitalWrite(RCLK,LOW);
    digitalWrite(RCLK,HIGH);
	}
}




#endif
