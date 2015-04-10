#include "Arduino.h"
#include "game1.h"
#include "config.h"
/*
  Game Math Kid Wai
*/
static int current_n_game = 1;
extern int state;
void game1()
{
  interval = 100;
  static unsigned int num_game = 3;
  static int game_win = 0;
  if (state == 0) {
    game1_config(num_game);
  } else if (state == 1) {
    game1_run(num_game, game_win);
  } else if (state == 2) {
    game1_end(num_game, game_win);
  }
}

void game1_config(unsigned int& num_game)
{
  //interval = 150;
  if (last_lcd_key == lcd_key) return;
  if (num_game < 1) num_game = 1;
  if (is_pongpongpong == 1 && num_game < 3) num_game = 3;
  lcd.setCursor(0, 0);
  lcd.print("Enter num game.   ");
  lcd.setCursor(0, 1);
  sprintf(buffer1, "Numgame : %d     ", num_game);
  lcd.print(buffer1);
   switch(lcd_key)
    {
      case btnUP:
        {
          num_game+=1;
          break;
        }
      case btnRIGHT:
        {
          num_game+=1;
          break;
        }
      case btnLEFT:
        {
          num_game-=1;
          break;
        }
      case btnDOWN:
        {
          num_game-=1;
          break;
        }
      case btnSELECT:
        {
          if (last_lcd_key != lcd_key) state = 1;
        }
    }
}

void game1_run(unsigned int num_game, int& game_win)
{
  interval = 100;
  if (!is_generate) {
    a = random(10,100);
    b = random(10,100);
    c = random(10,100);
    d = random(10,100);
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    if (a > 70) {
      sprintf(buffer2, "%d+%d+%d-%d= ???", a, b, c, d);
      game_ans = a + b + c - d;
    } else if (a > 50) {
      sprintf(buffer2, "%d+%d-%d+%d= ???", a, b, c, d);
      game_ans = a + b - c + d;
    } else if (a > 30) {
      sprintf(buffer2, "%d-%d+%d+%d= ???", a, b, c, d);
      game_ans = a - b + c + d;
    } else {
      sprintf(buffer2, "%d-%d+%d-%d= ???", a, b, c, d);
      game_ans = a - b + c - d;
    }
    lcd.print(buffer2);
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
          r = random(-178, 288);
          //if (millis()%2) r = -r;
          //if (r < -178) r = -178;
        }while(r==game_ans);
        game_c[i] = r;
        //lcd.print(r);
      }
      //lcd.setCursor((i+1)*5 + 1,1); 
    }
    sprintf(buffer1, " %4d %4d %4d", game_c[0], game_c[1],game_c[2]);
    is_generate = 1;
  }
  lcd.setCursor(0,1);
  lcd.print(buffer1);
  lcd.setCursor(game_select_c*5,1);
  lcd.print(">");
  /*
  for(int i=0;i<3;i++)
  {
    if (game_select_c == i)
    {
      lcd.print(">");
    } else {
      lcd.print(" "); 
    }
    lcd.print(game_c[i]);
  }
  */
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
        if (last_lcd_key == lcd_key) break;
        lcd.clear();
        lcd.setCursor(0,0);
        if (a > 70) {
          sprintf(buffer2, "%d+%d+%d-%d=%d", a, b, c, d, game_c[game_select_c]);
          //lcd.print(String(a) + "+" + String(b) + "+" + String(c) + "-" + String(d) + " = " + String(game_c[game_select_c]));
        } else if (a > 50) {
          sprintf(buffer2, "%d+%d-%d+%d=%d", a, b, c, d, game_c[game_select_c]);
          //lcd.print(String(a) + "+" + String(b) + "-" + String(c) + "+" + String(d) + " = " + String(game_c[game_select_c]));
        } else if (a > 30) {
          sprintf(buffer2, "%d-%d+%d+%d=%d", a, b, c, d, game_c[game_select_c]);
          //lcd.print(String(a) + "-" + String(b) + "+" + String(c) + "+" + String(d) + " = " + String(game_c[game_select_c]));
        } else {
          sprintf(buffer2, "%d-%d+%d-%d=%d", a, b, c, d, game_c[game_select_c]);
          //lcd.print(String(a) + "-" + String(b) + "+" + String(c) + "-" + String(d) + " = " + String(game_c[game_select_c]));
        }
        lcd.print(buffer2);
        lcd.setCursor(0,1);
        if (game_select_c == game_ans_c) {
          lcd.print("Yes, Correct.");
          game_win += 1;
        } else {
          lcd.print("No, Wrong.");
        }
        game_select_c = 0;
        current_n_game += 1;
        delay(1500);
        is_generate = 0;
        if (current_n_game > num_game) {
          state = 2;
        }
        lcd.clear();
        break;
      } 
  }
  if (lcd_key != btnNONE) delay(200);
}

void game1_end(unsigned int& num_game, int& game_win)
{
  lcd.setCursor(0, 0);
  sprintf(buffer1, "Your win %d of %d ", game_win, num_game);
  lcd.print(buffer1);
  lcd.setCursor(0,1);
  lcd.print("Please any key.   ");
  if (lcd_key != btnNONE && last_lcd_key != lcd_key) {
    state = 0;
    mode = SELECTGAME;
    num_game = 3;
    game_win = 0;
    current_n_game = 1;
    reset_var();
    //if (is_pongpongpong && game_win == num_game) mode = TIME;
  }
  if (is_pongpongpong && game_win == num_game) {
     is_pongpongpong = 0;
     stop_alarm(1);
     mode = TIME;
  }
}
