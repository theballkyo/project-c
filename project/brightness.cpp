#include "Arduino.h"
#include "brightness.h"
#include "EEPROM.h"
//#include <LiquidCrystal.h>

int brightlv = EEPROM.read(ADDRBN);

void brigtness()
{
  interval = 100;
  lcd.setCursor(0,0);
  lcd.print("Set Bright Level");
  if (brightlv > 255) brightlv = 255;
  else if (brightlv < 0) brightlv = 1;
  //int i=0;
  lcd.setCursor(0,1);
  lcd.print("                 ");
  lcd.setCursor(0,1);
  analogWrite(10, brightlv);
  for (int i = 0; i <= brightlv/16; ++i)
  {
    lcd.write(byte(2));
  }

    switch(lcd_key)
    {
      case btnUP:
        {
          brightlv+=16;
          break;
        }
      case btnRIGHT:
        {
          brightlv+=16;
          break;
        }
      case btnLEFT:
        {
          brightlv-=16;
          break;
        }
      case btnDOWN:
        {
          brightlv-=16;
          break;
        }
      case btnSELECT:
        {
          write_eeprom(ADDRBN, brightlv);
          mode = TIME;
          lcd.clear(); 
        }
    }

}

int get_bright_lv() { return brightlv; }
