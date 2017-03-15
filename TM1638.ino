#include "TM1638.h"
//the param is DATA pin,SCLK pin and STB pin
TM1638 tm(2,3,4);
  
  void setup ()
    {
       delay(1000);
       //Serial.begin(9600);
        tm.setDigital(0,1);
      // tm.setDigital(1,2);
       //tm.update();
         
           
    }
    void loop()
    {
      //tm.setDigital(1,0);
    }

 
   
