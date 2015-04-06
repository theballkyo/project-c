#include "Arduino.h"
#include "brightness.h"
//#include <LiquidCrystal.h>

int brightlv = 255;

void brigtness(LiquidCrystal lcd, int key, int& interval, int& mode)
{
  interval = 100;
  lcd.setCursor(0,0);
  lcd.print("Set Bright Level");
  if (brightlv > 255) brightlv = 255;
  else if (brightlv < 0) brightlv = 0;
  int brightcursor=0;
  lcd.setCursor(0,1);
  lcd.print("                 ");
  lcd.setCursor(0,1);
  analogWrite(10, brightlv);
  for (brightcursor = 0; brightcursor <= brightlv/16; ++brightcursor)
  {
    lcd.write(byte(2));
  }
  brightcursor--;
    switch(key)
    {
      case btnUP:
        {
          brightlv+=16;
          brightcursor++;
          //lcd.setCursor(1,brightcursor);
          //lcd.write(byte(2));
          break;
        }
      case btnDOWN:
        {
          brightlv-=16;
          brightcursor--;
          //lcd.setCursor(1,brightcursor);
          //lcd.write(byte(3));
          break;
        }
      case btnSELECT:
        {
          mode = TIME;
         lcd.clear(); 
        }
    }

}

int get_bright_lv() { return brightlv; }
