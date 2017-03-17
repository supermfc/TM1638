/*
 *  Tube display utilities for TM1638 IC  
 *  Code by DingBing in jiaozuo city henan province China
 *	My QQ：396364218 and i love mcu & arduino.
 *  welcome add my qq and study each other.
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

//display model,how get this array,pls view the excel file.
unsigned char TM1638::LED_MODEL[17] = 
    { 
    // 0    1     2   3    4    5    6    7    8     9   A    B    C    D    E    F    - 
      0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0xEE,0x3E,0x9C,0x7A,0x9E,0x8E,0x02
	};


// Display Buffer,Default don't display any char or digital	  
unsigned char TM1638::LedData[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


//construct function,set the pin and mode
TM1638::TM1638(int dataPin,int sclkPin,int stbPin)
{
	DIO = dataPin;
	SCLK = sclkPin;
	STB = stbPin;
	
	pinMode(DIO,OUTPUT);
	pinMode(SCLK,OUTPUT);
	pinMode(STB,OUTPUT); 
  digitalWrite(STB,HIGH);
}

//only display 0-9 A-F and the '-' char.
void TM1638::setLetter(int place,char letter)
{
  place = 7-place;
  
	if( (letter >= 'A') & ( letter <= 'F'))
		LedData[place] = LED_MODEL[letter-55];
	if( letter == '-')
		LedData[place] = LED_MODEL[16];

   update();
}

//set display the digital in the special place
void TM1638::setDigital(int place,int digital)
{
  place = 7 - place;
	TM1638::LedData[place] = TM1638::LED_MODEL[digital];
  update();
}

// display a int number,only 0-9 can be displayed
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
				LedData[i] = LED_MODEL[16];
				break;
			default:
				LedData[i] = LED_MODEL[string[i]-'0'];
       // LedData[7-i] = LED_MODEL[1];
				break;       
		}
	}
	update();
}

//prec , because display a float,so only the 0-9 or '.' can be displayed.
void TM1638::displayFloat(float number,int prec)
{
	char str[10]={0};
	
	int point = 0;
	dtostrf(number,8,prec,str);   //convert the float type to a string.
  
  int j = 9;
	while(str[j]==0)
		j--;

	for(int i = 7; i >= 0; i--) 
	{
    
		switch(str[j])
		{
			case ' ':
				break;
			case '.':
				LedData[i] |= 0x01;
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
				LedData[i]  |=  LED_MODEL[str[j]-'0']  ;
				break;     
     
		}
		j--;
   
	}

	update();

}

//turn off the dg at special place
void TM1638::setNoPoint(int place)
{
	LedData[7-place] &= 0xFE;
  update();
}

//set the dg turn on at special place
void TM1638::setPoint(int place)
{
	LedData[7-place] |= 0x01;
  update();
}

//turn off the display
void TM1638::turnOffDisplay()
{
    digitalWrite(STB,LOW);
    writeData(0x88);
    digitalWrite(STB,HIGH);
}

void TM1638::turnOnDisplay()
{
   digitalWrite(STB,LOW);
   writeData(0x8A);
   digitalWrite(STB,HIGH);
}

// the light scope is 0-7
void TM1638::setDisplayLight(int light)
{
	  if(light>=0 || light <=7)
   {
      digitalWrite(STB,LOW);
      writeData(0x80|light);
      digitalWrite(STB,HIGH);
   }
}

void TM1638::writeData(unsigned char data)
{
	 shiftOut(DIO,SCLK,LSBFIRST,data);
}

//clear the display data
void TM1638::clear()
{
	for(int i = 0; i<8; i++)
		LedData[i] = 0x00;
	
	update();
}

//write data to the TM1638
void TM1638::update()
{
 
	unsigned  char buff[8] = {0x00};
	
	for(int i = 0; i < 8 ; i++)
	{
		unsigned char temp = 0x00;
		
		for (int j = 0; j < 8; j++)
		{
      temp = LedData[j] & ( 1 << (7-i) );                     //first get the high bit
    	buff[i] = buff[i] | (temp << i) >> j;                  
		       
		}
		
	}
	
	digitalWrite(STB,LOW);
	writeData(0x8a);
  digitalWrite(STB,HIGH);
 
	//writeData(0x40); //write data command
	
	digitalWrite(STB,LOW);
	writeData(0xc0);   //set the reg display address to the 0x00
		
	//write the 16 byte display data

	for( int i = 0; i < 8 ; i++)
	{
    
		writeData(buff[i]);
   //Serial.println(buff[i]);
		writeData(0x00);
	}
	digitalWrite(STB,HIGH);
	
}

void TM1638::scanKey()
{
  //
  digitalWrite(STB,LOW);
  writeData(0x42);
  delayMicroseconds(1);
  pinMode(DIO,INPUT);
  key[0] = shiftIn(DIO,SCLK,LSBFIRST);
  key[1] = shiftIn(DIO,SCLK,LSBFIRST);
  key[2] = shiftIn(DIO,SCLK,LSBFIRST);
  key[3] = shiftIn(DIO,SCLK,LSBFIRST);
  digitalWrite(STB,HIGH);
  pinMode(DIO,OUTPUT);
}

 unsigned char TM1638::getKey(int index)
 {
   return key[index];
  }

#endif
