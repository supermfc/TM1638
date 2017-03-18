#include "TM1638.h"
//the param is DATA pin,SCLK pin and STB pin
TM1638 tm(2,3,4);
  int disint = 100;
  void setup ()
    {
       delay(1000);
       Serial.begin(9600);
       tm.turnOnDisplay();

      tm.displayFloat(124565.35,2);
      delay(1000);
      tm.clear();
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
    
    void loop()
    {
      tm.scanKey();

      unsigned char c = tm.getKey(0);

      if(c == 0x04)
      {
         disint++;
         while(c==0x04)
         {
         tm.scanKey();
         c = tm.getKey(0);
         }
         tm.clear();
      }
      if(c==0x40)
      {
         disint--;
         while(c==0x40)
         {
         tm.scanKey();
         c = tm.getKey(0);
         }
         tm.clear();
      }
      
      tm.displayInt(disint);
    }

 
   
