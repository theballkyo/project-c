#include "Arduino.h"
#include "game2.h"
#include "config.h"

int level = read_eeprom(ADDRLV);

void game2()
{
  static int times = 0;
  //Prepare if enter game first time
  if (mode != last_mode) {
    current_select = 0;
  }
  if (state == 0) {
    if (is_pongpongpong && level < 9) level = 9; 
    game2_config();
  } else if (state == 1) {
    game2_end();
  } else if (state == 2) {
    game2_run();
  } else if (state == 3) {
    game2_level();
  }
  if (last_state != state) {
    lcd.clear();
    last_state = state;
  }
}

void game2_config()
{
  byte hero[8] = {
	0b10001,
	0b11111,
	0b10101,
	0b01110,
	0b00100,
	0b11111,
	0b00100,
	0b01010
  };
  byte boom[8] = {
	0b00011,
	0b00100,
	0b01000,
	0b10000,
	0b10000,
	0b01000,
	0b00100,
	0b00011
  };
  
  lcd.createChar(0, hero);
  lcd.createChar(1, boom);
  lcd.setCursor(0, 0);
  lcd.print("NIGHTMARE HERO! ");
  lcd.setCursor(0, 1);
  if (current_select == 0) {
    lcd.print("Play game.      ");
  } else if (current_select == 1) {
    lcd.print("Set level.      ");
  } else {
    lcd.print("Exit.          ");
  }
  
  switch (lcd_key)
  {
    case btnDOWN:
      {
        current_select += 1;
        if(current_select > 2) current_select = 0;
        break; 
      }
    case btnUP:
      {
        current_select -= 1;
        if(current_select < 0) current_select = 2;
        break; 
      }
    case btnSELECT:
      {
        if (lcd_key != last_lcd_key){
          if (current_select == 0) {
            state = 2;
          } else if (current_select == 1) {
            state = 3;
          } else {
            mode = SELECTGAME; 
          }
          /* else {
            if (is_pongpongpong != 1)
              mode = SELECTGAME;
            else
              state = 0;
          }
          */
        }
        break;
      }
  }
  
  if (lcd_key != btnNONE) delay(200);
}

void game2_level()
{
  interval = 200;
  if (level < 1) level = 1;
  if (level > 16) level = 16;
  if (is_pongpongpong && level < 8) level = 8; 
  lcd.setCursor(0, 0);
  lcd.print("Set level    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  for (int i = 1; i <= level; ++i)
  {
    lcd.write(byte(2));
  }
  switch(lcd_key)
    {
      case btnUP:
        {
          level+=1;
          break;
        }
      case btnRIGHT:
        {
          level+=1;
          break;
        }
      case btnLEFT:
        {
          level-=1;
          break;
        }
      case btnDOWN:
        {
          level-=1;
          break;
        }
      case btnSELECT:
        {
          write_eeprom(ADDRLV, level);
          state = 0;
        }
    }
}

void game2_run()
{
  interval = 50;
  //lcd.clear();
  
  lcd.setCursor(0,0);
    for(int i = 0; i < 14; i++)
    {
      if (game_dm[0][i] == 1) {
        lcd.write(byte(1));
        //lcd.print("-"); 
      } else {
        lcd.print(" ");
      }
      
    }
    lcd.setCursor(0,1);
    for(int i = 0; i < 14; i++)
    {
      if (game_dm[1][i] == 1) {
        //lcd.print("-");
        lcd.write(byte(1)); 
      } else {
        lcd.print(" ");
      }
      
      //Generate 
      if (millis() > cd) {
        game_dm[0][i] = game_dm[0][i+1];
        game_dm[1][i] = game_dm[1][i+1];
        if (game_dm[0][i-1] == 0 && game_dm[1][i-1] == 0 && game_dm[0][i-2] == 0 && game_dm[1][i-2] == 0 && i == 13) {
         //randomSeed(micros());
         int c = millis()%2;
         //Serial.println(c);
         if (c == 0) {
           game_dm[0][i] = 1;
         } else {
           game_dm[1][i] = 1;
         }
         if (game_dm[0][i] == 1 && game_dm[0][i-3] == 1 && game_dm[0][i-6] == 1 && game_dm[0][i-9] == 1) {
           game_dm[0][i] = 0;
           game_dm[1][i] = 1;
         } else if (game_dm[1][i] == 1 && game_dm[1][i-3] == 1 && game_dm[1][i-6] == 1 && game_dm[1][i-9] == 1) {
           game_dm[0][i] = 1;
           game_dm[1][i] = 0;
         }
        }
      }
    }
  if (millis() > cd)
    cd = millis() + (50 * (16 - level));
  //cd += interval;
  
  lcd.setCursor(game_pos_me[X], game_pos_me[Y]);
  //lcd.print(">");
  lcd.write(byte(0));
  lcd.setCursor(14, 0);
  //lcd.print("^^");
  lcd.write(byte(3));
  lcd.write(byte(4));
  lcd.setCursor(14, 1);
  lcd.write(byte(5));
  lcd.write(byte(6));
  //lcd.print("oo");
  
  if(last_lcd_key != lcd_key)
  {
    switch(lcd_key){
        
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

  if (game_pos_me[X] >= 14) {
    game_ans = 1;
    //game_select = GAME2END;
    state = 1;
    delay(500);
  } else if (game_dm[game_pos_me[Y]][game_pos_me[X]] == 1) {
    lcd.clear();
    game_ans = 0;
    state = 1;
    delay(500);
  }
}

int game2_end()
{
  
  lcd.setCursor(0,0);
  interval = 100;
  if (game_ans == 1)
    lcd.print("Win Yeahh    ");
   else
    lcd.print("Lose Yeahh   ");
  lcd.setCursor(0,1);
  lcd.print("Please any key.");
  Serial.println(game_ans);
  Serial.println("is_pong" + String(is_pongpongpong));
  if (is_pongpongpong == 1 && game_ans  == 1) {
    Serial.println(game_ans);
    Serial.println("is_pong" + String(is_pongpongpong));
    is_pongpongpong = 0;
    stop_alarm(1);
    reset_var();
    mode = TIME;
  }
  if(lcd_key != btnNONE && last_lcd_key != lcd_key){
    state = 0;
    
    reset_var();
    //delay(100);
  }
  return 0;
}


