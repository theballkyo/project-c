#include "Arduino.h"
#include "game2.h"
#include "config.h"
/*
int game_pos_me[2] = {0};
int game_dm[2][16] = {0};
int game_ans       = 0;
int cd = 0;
int last_lcd_key = btnNONE;
*/
void game2(LiquidCrystal lcd, int key, int& interval, int& mode, int& game_select)
{
  //int f_spawn = 0;
  interval = 100;
  lcd.clear();
  
  lcd.setCursor(0,0);
    for(int i = 0; i < 14; i++)
    {
      //game_dm[0][i+1] = (game_dm[0][i+1]) ? 0 : 1;
      /*
      if (game_pos_me[Y] == 0) {
        if (game_pos_me[X] == i) {
          lcd.print(">");
        }
      }
      */
      if (game_dm[0][i] == 1) {
        lcd.print("-"); 
      } else {
        lcd.print(" ");
      }
      
    }
    lcd.setCursor(0,1);
    for(int i = 0; i < 14; i++)
    {
      /*
      if (game_pos_me[Y] == 1) {
        if (game_pos_me[X] == i) {
          lcd.print(">");
        }
      }
      */
      if (game_dm[1][i] == 1) {
        lcd.print("-"); 
      } else {
        lcd.print(" ");
      }
      
      //Generate 
      if (cd >= interval * 10) {
        game_dm[0][i] = game_dm[0][i+1];
        game_dm[1][i] = game_dm[1][i+1];
        if (game_dm[0][i-1] == 0 && game_dm[1][i-1] == 0 && game_dm[0][i-2] == 0 && game_dm[1][i-2] == 0 && i == 13) {
         if (random(1,2) == 1) {
           if (random(0,100) > 50) {
             game_dm[0][i] = 1;
           } else {
             game_dm[1][i] = 1;
           }
         }
        }
      }
    }
  if (cd >= interval * 10)
    cd = 0;
  cd += interval;
  lcd.setCursor(game_pos_me[X], game_pos_me[Y]);
  lcd.print(">");
  lcd.setCursor(14, 0);
  lcd.print("^^");
  lcd.setCursor(14, 1);
  lcd.print("oo");
  if(last_lcd_key != key)
  {
    switch(key){
        
      case btnUP:
      {
      	game_pos_me[Y] = 0;
        break;
      }
      case btnDOWN:
      {
        game_pos_me[Y] = 1;
        break;
      }
      case btnRIGHT:
      {
        game_pos_me[X] += 1;
        break;
      }
    }
  }

  if (game_pos_me[X] >= 14)
  {
    game_ans = 1;
    game_select = GAME2END;
    delay(500);
  }
  Serial.println(game_dm[game_pos_me[Y]][game_pos_me[X]]);
  if (game_dm[game_pos_me[Y]][game_pos_me[X]] == 1)
  {
    game_ans = 0;
    game_select = GAME2END;
    delay(500);
  }
}

void game2_end(LiquidCrystal lcd, int key, int& interval, int& mode, int& game_select)
{
  interval = 100;
  //lcd.clear();
  lcd.setCursor(0,0);
  if (game_ans == 1)
    lcd.print("Win Yeahh");
   else
    lcd.print("Lose Yeahh");
  lcd.setCursor(0,1);
  lcd.print("Please any key.");
  //delay(2000);
  if(key != btnNONE && last_lcd_key != key){
    game_select = GAME2;
    mode = SELECTGAME;
  }
}
void reset_var()
{
  //a              = 0;
  //b              = 0;
  game_ans       = 0;
  //game_ans_c     = 0;
  //game_select_c  = 0;
  memset(game_pos_me, 0, sizeof(game_pos_me));
  memset(game_dm, 0, sizeof(game_dm[0][0])*2*16);

}

