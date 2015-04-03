//#include <Time.h>
#include "Wire.h"
#include "SPI.h"  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
RTC_DS3231 RTC;
// define some values used by the panel and buttons
int lcd_key        = 0;
int last_lcd_key   = 0;
int adc_key_in     = 0;
int shit           = 0;
int mode           = 0;
int interval       = 1000;
int sound_interval[2] = {100, 5};
// 0 Off 1 On
int alarm          = 0;
//Check button press time
int t_press        = 0;
int set_sel        = 2;
int current_select = 0;
int game_select    = 0;
//
int cd             = 0;
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

int game_pos_me[2] = {0};
int game_dm[2][16] = {0};
unsigned int previousMillis[4] = {0};

// time[3] > {hour, minute, second}
int temp_time[6]        = {0,0,0,0,0,0};

// 3 Day
int alarm_time[4]  = {0,0,0,0};

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

//setup brigtness LCD
byte barUp[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte barDown[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
int brightlv = 255;

//setup Sound
#define speakerPin     13

//Mode
#define NUMMODE    5
#define SELECTMODE -1
#define TIME       0
#define SETTIME    1
#define SETALARM   2
#define SELECTGAME 3
#define SETBRIGHT  4
#define ALARMING   97
#define PLAYGAME   98
#define PONGPONGPONG 99

//Game list
#define GAME1     0
#define GAME2     1

//Game mode
#define GAME2END  90

//GAME KEY
#define X         0
#define Y         1

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
   Wire.begin();
   RTC.begin();
   DateTime now = RTC.now();
   //RTC.adjust(DateTime(__DATE__, __TIME__));
   lcd.begin(16, 2);              // start the library
   lcd.setCursor(0,0);
   lcd.print(now.hour()); // print a simple message
   lcd.createChar(0,smile);
   lcd.createChar(1,sad);
   lcd.createChar(2, barUp);
   lcd.createChar(3, barDown);
   analogWrite(10, brightlv);
   Serial.begin(9600);
   Serial.println("Setup");
   //analogWrite (speakerPin, 255);
}
 
void loop()
{
  
  //Serial.println("Hello");
  unsigned int currentMillis = millis();
  DateTime now = RTC.now();
  //lcd.setCursor(8,1);            // move cursor to second line "1" and 9 spaces over
  //lcd.print(t_press);      // display seconds elapsed since power-up
  lcd_key = read_LCD_buttons();
  //
  if (now.day() == alarm_time[DAY] && now.hour() == alarm_time[HOUR] && now.minute() == alarm_time[MINUTE] && alarm == 0 && mode != SETALARM) {
      //Serial.println("test");
      is_alarm = 1;
      Serial.println(String(lcd_key) + String(last_lcd_key));
      if(lcd_key != btnNONE && last_lcd_key != lcd_key)
      {
        is_alarm = 0;
        alarm_time[DAY] += 1;
        analogWrite (speakerPin, 0);
      } else {
        if(currentMillis - previousMillis[1] > sound_interval[1])
        {
          analogWrite (speakerPin, 255);
          previousMillis[1] =currentMillis;
        //delay(100);
          if(currentMillis - previousMillis[2] > sound_interval[0] + sound_interval[1])
            {
            previousMillis[1] =currentMillis;
            previousMillis[2] =currentMillis;
            analogWrite (speakerPin, 0);
            }
        //delay(5);
        }
      }
  }
  if (t_press > LONGCLICK)
  {
    Serial.print(t_press);
    Serial.println();
    if (!is_pongpongpong){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Plz select mode.");
      t_press = 0; 
      mode = SELECTMODE;
      interval = 100;
    }
  }
  
  if(currentMillis - previousMillis[0] > interval)
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
        game2(lcd_key);
      } else if (game_select == GAME2END) {
        game2_end(lcd_key);
      }
    } else if (mode == SETBRIGHT) {
      brigtness(lcd_key);
    }
    //Set randomSeed
    randomSeed(millis());
    last_lcd_key = lcd_key;
    previousMillis[0] = currentMillis;
  }
  if(currentMillis - previousMillis[3] > 1)
  {
    is_click += currentMillis - previousMillis[3];
    switch (lcd_key)
    {
      case btnSELECT:
        {
          t_press += currentMillis - previousMillis[3];
          break;
        }
      case btnNONE:
        {
          
          is_click = 0;
          t_press = 0;
          break;
        }
    } 
    previousMillis[3] = currentMillis;
  }
  //delay(1);
}
void show_time()
{
  DateTime now = RTC.now();
  interval = 1000;
  /*
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCur
  */
  lcd.clear();
  lcd.setCursor(shit,0);
  sprintf(buffer1,"%02d:%02d:%02d %dC",now.hour(),now.minute(),now.second(), (int)floor(RTC.getTemperature()));
  Serial.println(RTC.getTemperature());
  lcd.print(buffer1);

  sprintf(buffer1,"%s:%02d/%s/%04d", day_short_t[now.dayOfWeek()-1], now.day(), month_short_t[now.month()-1], now.year());
  lcd.setCursor(0,1);
  //lcd.print(String(day_short_t[weeknow.day()-1]) + "/" + month_short_t[now.month()-1] + "/" + now.year());
  //int w = weeknow.day();
  //lcd.print(String(day_t[6]) + "/" + month_t[1]);
  lcd.print(buffer1);
  lcd.write((byte)alarm);
  Serial.print(buffer1);
  Serial.println();
  shit += 1;
  if (shit >=5)
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
  if (cd == 4) {
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
    cd = 0;
  } else {
    cd += 1;
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
              //adjustTime(-now.second());
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
              //setTime(now.hour(), now.minute(), now.second(), now.day(), now.month()+1, now.year());
              break;
            } 
          case YEAR:
            {
              temp_time[YEAR] += 1;
              //setTime(now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year()+1);
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
              //adjustTime(-now.second());
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
              //setTime(now.hour(), now.minute(), now.second(), now.day(), now.month()+1, now.year());
              break;
            } 
          case YEAR:
            {
              temp_time[YEAR] -= 1;
              //setTime(now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year()+1);
              break;
            }  
        }
        break; 
      }
      
    case btnSELECT:
      {
        if (is_click < LONGCLICK) mode = TIME, interval = 1000, previousMillis[0] = 0, lcd.clear(), RTC.adjust(DateTime(temp_time[YEAR], temp_time[MONTH], temp_time[DAY], temp_time[HOUR], temp_time[MINUTE], temp_time[SECOND]));
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
  DateTime now = RTC.now();
  alarm_time[DAY] = now.day();
  char alarm_t[2][5] = {"On", "Off"};
  interval = 100;
  sprintf(buffer1, "%02d:%02d:%02d",alarm_time[HOUR], alarm_time[MINUTE], alarm_time[SECOND]);
  sprintf(buffer2,"Alarm is %s",alarm_t[alarm]);
  if (cd == 4) {
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
        if (is_click < LONGCLICK) mode = TIME, interval = 1000, previousMillis[0] = 0, lcd.clear();
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
  interval = 100;
  //lcd.setCursor(1,1);
  //lcd.print("              ");
  lcd.setCursor(1,1);
  if (current_select == TIME) {
    lcd.print("Time         ");
  } else if (current_select == SETTIME) {
    lcd.print("Set time.      ");
  } else if (current_select == SETALARM) {
    lcd.print("Set alarm.     ");
  } else if (current_select == SELECTGAME) {
    lcd.print("Play game Yeah!");
  } else if (current_select == SETBRIGHT) {
    lcd.print("Set brightness.");
  } else if (current_select == NUMMODE) {
    lcd.print("Exit.         ");
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
          previousMillis[0] = 0;
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
  
  if (key < 5) delay(200);
}

void set_temp_time()
{
  DateTime now = RTC.now();
  temp_time[SECOND] = now.second();
  temp_time[MINUTE] = now.minute();
  temp_time[HOUR] = now.hour();
  temp_time[DAY] = now.day();
  temp_time[MONTH] = now.month();
  temp_time[YEAR] = now.year();
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
          //Reset variable
          reset_var();
          mode = PLAYGAME;
          //interval = 100;
          previousMillis[0] = 0;
          lcd.clear();
        }
        //if (game_select == 2) mode = TIME;
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

void game2(int key)
{
  interval = 100;
  lcd.clear();
  lcd.setCursor(0,0);
  //game_dm[1] = 1;
  //lcd.print(">");
  for(int i = 0; i < 16; i++)
  {
    if (game_pos_me[Y] == 0) {
      if (game_pos_me[X] == i) {
        lcd.print(">");
      } else {
        lcd.print(" ");
      }

    }
  }
  lcd.setCursor(0,1);
  for(int i = 0; i < 16; i++)
  {
    if (game_pos_me[Y] == 1) {
      if (game_pos_me[X] == i) {
        lcd.print(">");
      } else {
        lcd.print(" ");
      }
    }
  }
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
    game_select = GAME2END;
  }
}

void game2_end(int key)
{
  interval = 100;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Win Yeahh");
  lcd.setCursor(0,1);
  lcd.print("Please any key.");
  if(key != btnNONE){
    game_select = GAME2;
    mode = SELECTGAME;
  }
}

void reset_var()
{
  a              = 0;
  b              = 0;
  //game_select    = 0;
  //game_c[3]      = {0};
  game_ans       = 0;
  game_ans_c     = 0;
  game_select_c  = 0;
  memset(game_pos_me, 0, sizeof(game_pos_me));
  memset(game_dm, 0, sizeof(game_dm[0][0]));
  //game_pos_me = {0};
  //game_dm[2][16] = {0};
}
void brigtness(int key)
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

