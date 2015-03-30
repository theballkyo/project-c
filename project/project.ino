#include <Time.h>

//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key        = 0;
int adc_key_in     = 0;
int shit           = 0;
int mode           = 0;
int interval       = 1000;
// 0 Off 1 On
int alarm          = 0;
//Check button press time
int t_press        = 0;
int set_sel        = 2;
int current_select = 0;
int game_select    = 0;
//
int is_vision      = 0;
int is_click       = 0;
int is_pongpongpong = 0;
int is_generate    = 0;
int is_alarm       = 0;
int a              = 0;
int b              = 0;
int game_c[3]      = {0};
int game_ans       = 0;
int game_ans_c     = 0;
int game_select_c  = 0;
unsigned int previousMillis = 0; 
// time[3] > {hour, minute, second}
int temp_time[6]        = {0,0,0,0,0,0};
int alarm_time[3]  = {0,0,0};

char buffer1[18]    = "";
char buffer2[18]    = "";
char month_t[12][11]  = {"January","February","March","April","May","June","July","August","September","October","November","December"};
char month_short_t[12][10] = {"Jan","Feb","Mar","Apr","May","June","July","Aug","Sep","Oct","Nov","Dec"};
char day_t[7][10]     = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
char day_short_t[7][4] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

byte smile[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};
byte sad[8] = {
	0b00000,
	0b00000,
	0b10001,
	0b00000,
	0b00000,
	0b01110,
	0b10001,
	0b00000
};
//Mode
#define NUMMODE    4
#define SELECTMODE -1
#define TIME       0
#define SETTIME    1
#define SETALARM   2
#define SELECTGAME       3
#define ALARMING   97
#define PLAYGAME   98
#define PONGPONGPONG 99

//Game list
#define GAME1     0
#define GAME2     1

//Set time select
#define YEAR      5
#define MONTH     4
#define DAY       3
#define HOUR      0
#define MINUTE    1
#define SECOND    2

#define LONGCLICK 2000
//Button keys
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
   adc_key_in = analogRead(0);      // read the value from the sensor 
   // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
   // we add approx 50 to those values and check to see if we are close
   if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
   // For V1.1 us this threshold
   /*
   if (adc_key_in < 50)   return btnRIGHT;  
   if (adc_key_in < 250)  return btnUP; 
   if (adc_key_in < 450)  return btnDOWN; 
   if (adc_key_in < 650)  return btnLEFT; 
   if (adc_key_in < 850)  return btnSELECT;  
   */
   // For V1.0 comment the other threshold and use the one below:
  
   if (adc_key_in < 50)   return btnRIGHT;  
   if (adc_key_in < 195)  return btnUP; 
   if (adc_key_in < 380)  return btnDOWN; 
   if (adc_key_in < 555)  return btnLEFT; 
   if (adc_key_in < 790)  return btnSELECT;   
  
  
  
   return btnNONE;  // when all others fail, return this...
}

void setup()
{
   setTime(0,0,0,28,3,2015);
   lcd.begin(16, 2);              // start the library
   lcd.setCursor(0,0);
   lcd.print(hour()); // print a simple message
   lcd.createChar(0,smile);
   lcd.createChar(1,sad);
   Serial.begin(9600);
}
 
void loop()
{
  unsigned int currentMillis = millis();
  
  //lcd.setCursor(8,1);            // move cursor to second line "1" and 9 spaces over
  //lcd.print(t_press);      // display seconds elapsed since power-up
  lcd_key = read_LCD_buttons();
  if (hour() == alarm_time[HOUR] && minute() == alarm_time[MINUTE] && second() == alarm_time[SECOND]) {
    //Okay, PONGPONGPONGPONGPONG
  }
  //
  
  if (t_press > LONGCLICK)
  {
    Serial.print(t_press);
    Serial.println();
    if (!is_pongpongpong){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please select mode.");
      t_press = 0; 
      mode = SELECTMODE;
      interval = 100;
    }
  }
  
  if(currentMillis - previousMillis > interval)
  {
    if (mode == TIME) {
      show_time(); 
    } else if (mode == SETTIME) {
      set_time(lcd_key);
    } else if (mode == SELECTMODE) {
      select_mode(lcd_key); 
    } else if (mode == SETALARM) {
      set_alarm(lcd_key);
    } else if (mode == SELECTGAME) {
      select_game(lcd_key); 
    } else if (mode == PLAYGAME) {
      //Play game     
      if (game_select == GAME1) {
        game1(lcd_key);
      } else if (game_select == GAME2) {
        
      }
    }
    //Set randomSeed
    randomSeed(millis());
    previousMillis = currentMillis;
  }
  is_click += 1;
  switch (lcd_key)
  {
    case btnSELECT:
      {
        t_press += 1;
        break;
      }
    case btnNONE:
      {

        is_click = 0;
        t_press = 0;
        break;
      }
  }
  delay(1);
}
void show_time()
{
  interval = 1000;
  /*
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCur
  */
  lcd.clear();
  lcd.setCursor(shit,0);
  sprintf(buffer1,"%02d:%02d:%02d",hour(),minute(),second());
  
  lcd.print(buffer1);

  sprintf(buffer1,"%s:%02d/%s/%04d", day_short_t[weekday()-1], day(), month_short_t[month()-1], year());
  lcd.setCursor(0,1);
  //lcd.print(String(day_short_t[weekday()-1]) + "/" + month_short_t[month()-1] + "/" + year());
  //int w = weekday();
  //lcd.print(String(day_t[6]) + "/" + month_t[1]);
  lcd.print(buffer1);
  lcd.write((byte)alarm);
  Serial.print(buffer1);
  Serial.println();
  shit += 1;
  if (shit >=9)
    shit = 0;
}

void set_time(int key)
{
  // Set delay
  interval = 100;
  lcd.setCursor(0, 0);
  //lcd.print("                ");
  //Serial.print(set_sel);
  // Set cooldown blink = 3
  sprintf(buffer1,"%02d:%02d:%02d",temp_time[HOUR],temp_time[MINUTE],temp_time[SECOND]);
  sprintf(buffer2,"%02d/%4s/%04d", temp_time[DAY], month_short_t[temp_time[MONTH]-1], temp_time[YEAR]);
  if (is_vision == 4) {
    if (set_sel == SECOND) {
      sprintf(buffer1,"%02d:%02d:  ",temp_time[HOUR],temp_time[MINUTE]);
    } else if (set_sel == MINUTE) {
      sprintf(buffer1,"%02d:  :%02d",temp_time[HOUR],temp_time[SECOND]);
    } else if (set_sel == HOUR) {
      sprintf(buffer1,"  :%02d:%02d",temp_time[MINUTE],temp_time[SECOND]);
    } else if (set_sel == DAY) {
      sprintf(buffer2,"  /%4s/%04d", month_short_t[temp_time[MONTH]-1], temp_time[YEAR]);
    } else if (set_sel == MONTH) {
      sprintf(buffer2,"%02d/    /%04d", temp_time[DAY], temp_time[YEAR]);
    } else if (set_sel == YEAR) {
      sprintf(buffer2,"%02d/%4s/    ", temp_time[DAY], month_short_t[temp_time[MONTH]-1]);
    } 
    is_vision = 0;
  } else {
    is_vision += 1;
  }
  
  switch (key)
  {
    
    case btnLEFT:
      {
        set_sel -= 1;
        if (set_sel < HOUR) set_sel = YEAR;
        break;
      }
      
    case btnRIGHT:
      {
        set_sel += 1;
        if (set_sel > YEAR) set_sel = HOUR;
        break;
      }
      
    case btnUP:
      {
        switch (set_sel)
        {
          case SECOND:
            {
              //adjustTime(-second());
              temp_time[SECOND] = 0;
              break;
            }
          case MINUTE:
            {
              //adjustTime(60);
              temp_time[MINUTE] += 1;
              break;
            } 
          case HOUR:
            {
              //adjustTime(3600);
              temp_time[HOUR] += 1;
              break;
            }
          case DAY:
            {
              //adjustTime(86400);
              temp_time[DAY] += 1;
              break;
            }
          case MONTH:
            {
              temp_time[MONTH] += 1;
              //setTime(hour(), minute(), second(), day(), month()+1, year());
              break;
            } 
          case YEAR:
            {
              temp_time[YEAR] += 1;
              //setTime(hour(), minute(), second(), day(), month(), year()+1);
              break;
            }  
        }
        break; 
      }
      
    case btnDOWN:
      {
        switch (set_sel)
        {
          case SECOND:
            {
              //adjustTime(-second());
              temp_time[SECOND] = 0;
              break;
            }
          case MINUTE:
            {
              //adjustTime(60);
              temp_time[MINUTE] -= 1;
              break;
            } 
          case HOUR:
            {
              //adjustTime(3600);
              temp_time[HOUR] -= 1;
              break;
            }
          case DAY:
            {
              //adjustTime(86400);
              temp_time[DAY] -= 1;
              break;
            }
          case MONTH:
            {
              temp_time[MONTH] -= 1;
              //setTime(hour(), minute(), second(), day(), month()+1, year());
              break;
            } 
          case YEAR:
            {
              temp_time[YEAR] -= 1;
              //setTime(hour(), minute(), second(), day(), month(), year()+1);
              break;
            }  
        }
        break; 
      }
      
    case btnSELECT:
      {
        if (is_click < LONGCLICK) mode = TIME, interval = 1000, previousMillis = 0, lcd.clear(), setTime(temp_time[HOUR], temp_time[MINUTE], temp_time[SECOND], temp_time[DAY], temp_time[MONTH], temp_time[YEAR]);
        break;
      }
  }
  
  if (key != btnNONE) delay(200);
  if (temp_time[MINUTE] > 59) temp_time[MINUTE] = 0;
  if (temp_time[HOUR] > 23) temp_time[HOUR] = 0;
  if (temp_time[DAY] > 31) temp_time[DAY] = 1;
  if (temp_time[MONTH] > 12) temp_time[MONTH] = 1;
  
  if (temp_time[MINUTE] < 0) temp_time[MINUTE] = 59;
  if (temp_time[HOUR] < 0) temp_time[HOUR] = 23;
  if (temp_time[DAY] < 1) temp_time[DAY] = 31;
  if (temp_time[MONTH] < 1) temp_time[MONTH] = 12;
  
  lcd.print(buffer1);
  lcd.setCursor(0, 1);
  lcd.print(buffer2);
  
}

void set_alarm(int key)
{
  char alarm_t[2][5] = {"On", "Off"};
  interval = 100;
  sprintf(buffer1, "%02d:%02d:%02d",alarm_time[HOUR], alarm_time[MINUTE], alarm_time[SECOND]);
  sprintf(buffer2,"Alarm is %s",alarm_t[alarm]);
  if (is_vision == 4) {
    if (set_sel == SECOND) {
      sprintf(buffer1,"%02d:%02d:  ",alarm_time[HOUR],alarm_time[MINUTE]);
    } else if (set_sel == MINUTE) {
      sprintf(buffer1,"%02d:  :%02d",alarm_time[HOUR],alarm_time[SECOND]);
    } else if (set_sel == HOUR) {
      sprintf(buffer1,"  :%02d:%02d",alarm_time[MINUTE],alarm_time[SECOND]);
    } else if (set_sel == 3) {
      sprintf(buffer2,"Alarm is       ");
    }
    is_vision = 0;
  } else {
    is_vision += 1;
  }
  switch(key)
  {
    case btnLEFT:
      {
        set_sel -= 1;
        if (set_sel < 0) set_sel = 3;
        break;
      }
      
    case btnRIGHT:
      {
        set_sel += 1;
        if (set_sel > 3) set_sel = 0;
        break;
      }
      
    case btnUP:
      {
        switch (set_sel)
        {
          case SECOND:
            {
              alarm_time[SECOND] += 1;
              break;
            }
          case MINUTE:
            {
              alarm_time[MINUTE] += 1;
              break;
            } 
          case HOUR:
            {
              alarm_time[HOUR] += 1;
              break;
            }
          //Set on/off  
          case 3:
            {
              alarm = (alarm+1)%2;
              break; 
            }
        }
        break; 
      }
      
    case btnDOWN:
      {
        switch (set_sel)
        {
          case SECOND:
            {
              alarm_time[SECOND] -= 1;
              break;
            }
          case MINUTE:
            {
              alarm_time[MINUTE] -= 1;
              break;
            } 
          case HOUR:
            {
              alarm_time[HOUR] -= 1;
              break;
            }
          //Set on/off  
          case 3:
            {
              alarm = (alarm+1)%2;
              break; 
            }
        }
        break; 
      }
      
    case btnSELECT:
      {
        if (is_click < LONGCLICK) mode = TIME, interval = 1000, previousMillis = 0, lcd.clear();
        break;
      }
  }
  if (alarm_time[SECOND] > 59) alarm_time[SECOND] = 0;
  if (alarm_time[MINUTE] > 59) alarm_time[MINUTE] = 0;
  if (alarm_time[HOUR] > 23) alarm_time[HOUR] = 0;
  
  if (alarm_time[SECOND] < 0) alarm_time[SECOND] = 59;
  if (alarm_time[MINUTE] < 0) alarm_time[MINUTE] = 59;
  if (alarm_time[HOUR] < 0) alarm_time[HOUR] = 23;

  lcd.setCursor(0,0);
  lcd.print(buffer1);
  lcd.setCursor(0,1);
  lcd.print(buffer2);
  
  if (key != btnNONE) delay(200);
}

void select_mode(int key)
{
  lcd.setCursor(1,1);
  lcd.print("              ");
  lcd.setCursor(1,1);
  if (current_select == TIME) {
    lcd.print("Time");
  } else if (current_select == SETTIME) {
    lcd.print("Set time.");
  } else if (current_select == SETALARM) {
    lcd.print("Set alarm.");
  } else if (current_select == SELECTGAME) {
    lcd.print("Play game Yeah!");
  } else if (current_select == NUMMODE) {
    lcd.print("Exit.");
  }
  
  switch (key)
  {
    case btnDOWN:
      {
        current_select += 1;
        if(current_select > NUMMODE) current_select = 0;
        break; 
      }
    case btnUP:
      {
        current_select -= 1;
        if(current_select < 0) current_select = NUMMODE;
        break; 
      }
    case btnSELECT:
      {
        if (is_click < LONGCLICK){
          set_sel = 2;
          mode = current_select;
          interval = 1000;
          previousMillis = 0;
          lcd.clear();
          if (mode == SETTIME) {
            set_temp_time();
          } else if (mode == NUMMODE) {
            mode = TIME;
          }
        }
        break;
      }
  }
  
  if (key != btnNONE) delay(200);
}

void set_temp_time()
{
  temp_time[SECOND] = second();
  temp_time[MINUTE] = minute();
  temp_time[HOUR] = hour();
  temp_time[DAY] = day();
  temp_time[MONTH] = month();
  temp_time[YEAR] = year();
}

void select_game(int key)
{
  interval = 100;
  lcd.setCursor(0,0);
  lcd.print("Select game.");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(1,1);
  if (game_select == GAME1) {
    lcd.print("> MATH");
  } else if (game_select == GAME2) {
    lcd.print("> PONG PONG");
  } else if (game_select == 2) {
    lcd.print("> Exit");
  }
  
  switch (key)
  {
    case btnDOWN:
      {
        game_select += 1;
        if(game_select >= 3) game_select = 0;
        break; 
      }
    case btnUP:
      {
        game_select -= 1;
        if(game_select < 0) game_select = 2;
        break; 
      }
    case btnSELECT:
      {
        if (is_click < LONGCLICK){
          mode = PLAYGAME;
          //interval = 100;
          previousMillis = 0;
          lcd.clear();
        }
        if (game_select == 2) mode = TIME;
        break;
      }
  }
  
  if (key != btnNONE) delay(200);
}
/*
  Game Math Kid Wai
*/
void game1(int key)
{
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
    sprintf(buffer2, " %d  %d  %d", game_c[0], game_c[1], game_c[2]);
    is_generate = 1;
  }
  lcd.setCursor(0,1);
  lcd.print(buffer2);
  lcd.setCursor(game_select_c*5,1);
  lcd.print(">");
  switch(key)
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
  if (key != btnNONE) delay(200);
}









