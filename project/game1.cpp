#include "Arduino.h"
#include "game1.h"
#include "config.h"
/*
  Game Math Kid Wai
*/

void game1()
{
  //char buffer[18] = "";
  interval = 100;
  if (!is_generate) {
    a = random(100,500);
    b = random(100,500);
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print(String(a) + "+" + String(b) + " = ???");
    game_ans = a + b;
    int shift = 2;
    //lcd.setCursor(0,1);
    game_ans_c = random(0, 3);
    for (int i = 0; i < 3; i++) {
      if (i==game_ans_c) {
        game_c[i] = game_ans;
        //lcd.print(game_ans);
      } else {
        int r;
        do {
          r = random(100, 1000);
        }while(r==game_ans);
        game_c[i] = r;
        //lcd.print(r);
      }
      //lcd.setCursor((i+1)*5 + 1,1); 
    }
    sprintf(buffer1, " %d  %d  %d", game_c[0], game_c[1], game_c[2]);
    is_generate = 1;
  }
  lcd.setCursor(0,1);
  lcd.print(buffer1);
  lcd.setCursor(game_select_c*5,1);
  lcd.print(">");
  switch(lcd_key)
  {
    case btnRIGHT:
      {
        game_select_c += 1;
        if (game_select_c > 2) game_select_c = 0;
        break;
      }
    case btnLEFT:
      {
        game_select_c -= 1;
        if (game_select_c < 0) game_select_c = 2;
        break;
      }
    case btnSELECT:
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(String(a) + "+" + String(b) + " = " + String(game_c[game_select_c]));
        lcd.setCursor(0,1);
        if (game_select_c == game_ans_c) {
          lcd.print("Yes, Correct.");
        } else {
          lcd.print("No, Wrong.");
        }
        delay(5000);
        mode = SELECTGAME;
        is_generate = 0;
        lcd.clear();
        break;
      } 
  }
  if (lcd_key != btnNONE) delay(200);

}

