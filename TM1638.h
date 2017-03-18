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
#ifndef TM1638_h
#define TM1638_h

#include <avr/io.h>
#include <Arduino.h>
#include <avr/interrupt.h>

class TM1638
{
	public:
  
    // properties
   	
	
    // methods
	//
	TM1638(int dataPin,int sclkPin,int stbPin);
	
	void setLetter(int place,char letter);
	void setDigital(int place,int digital);
 
	void setPoint(int place);
	void setNoPoint(int place);

	void displayInt(int num);
	void displayFloat(float number,int prec);

  //turn on  the display
	void turnOnDisplay();
	void turnOffDisplay();

	void scanKey();
	unsigned char getKey(int index);
    
	void update();
	void clear();
	
	void setDisplayLight(int light);
	void writeData(unsigned char data);
	
  private:
	
	static unsigned char LED_MODEL[17];
	// Default not display
	static unsigned char LedData[8]; 
	
	int DIO;        		// Arduino pin for Data	   数据引脚
	int SCLK;						// Arduino pin for CLK     时钟引脚，高电平将数据串行移入
	int STB;						// Arduino pin for STB    片选引脚，高电平将锁存数据送到595引脚上
    unsigned char key[4];
};

#endif
