# TM1638
a arduino class for 8 digitals 7-seg tube and 2*8 keyboard scan hardware module use TM1638 IC.

![module pic](https://github.com/supermfc/TM1638/blob/master/Module_PIC/module.jpg)

it is very simple and easy to use:

```C
#include "TM1638.h"
//the param is DATA pin,SCLK pin and STB pin
TM1638 tm(2,3,4);
  
  void setup ()
    {
       delay(1000);
       Serial.begin(9600);

      tm.displayFloat(124565.35,2);

     /*
       tm.setDigital(0,0);
       tm.setDigital(1,1);
       tm.setDigital(2,2);
       tm.setDigital(3,3);
       tm.setDigital(4,4);
       tm.setDigital(5,5);
       tm.setDigital(6,6);
       tm.setDigital(7,7);

       delay(1000);
       tm.clear();
       tm.displayInt(123);
       
       tm.setPoint(3);
       */  
           
    }
 ```