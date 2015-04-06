
//#include <Time.h>
#include "Wire.h"
#include "SPI.h"  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include "brightness.h"
#include "time.h"
#include "config.h"
#include "game1.h"
#include "game2.h"
#include <EEPROM.h>
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
extern int last_lcd_key;
int adc_key_in     = 0;
int shit           = 0;
int mode           = 0;
int last_mode      = 0;
int interval       = 1000;
int sound_interval[2] = {100, 5};
// 0 Off 1 On
int alarm          = 1;
//Check button press time
int t_press        = 0;
int set_sel        = 2;
int current_select = 0;
int game_select    = 0;
//
extern int cd;
int is_vision      = 0;
int is_click       = 0;
int is_pongpongpong = 0;
//int is_generate    = 0;
int is_alarm       = 0;

unsigned int currentMillis;
unsigned int previousMillis[4] = {0};

// time[3] > {hour, minute, second}
//int temp_time[6]        = {0,0,0,0,0,0};

// 3 Day
int alarm_time[4]  = {EEPROM.read(ADDRH), EEPROM.read(ADDRM), EEPROM.read(ADDRS), EEPROM.read(ADDRD)};

char buffer1[18]    = "";
char buffer2[18]    = "";
char month_short_t[12][10] = {"Jan","Feb","Mar","Apr","May","June","July","Aug","Sep","Oct","Nov","Dec"};
char day_short_t[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

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
   analogWrite(10, get_bright_lv());
   Serial.begin(9600);
   Serial.println("Setup");
   //analogWrite (speakerPin, 255);
   //mode =PLAYGAME;
   //game_select =GAME2;
}
 
void loop()
{
  
  //Serial.println("Hello");
  currentMillis = millis();
  //DateTime now = RTC.now();

  lcd_key = read_LCD_buttons();
  if (is_alarming()) play_sound_alarm();
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
    if(mode != last_mode)
    {
      lcd.clear(); 
    }
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
        game1(lcd, lcd_key, interval, mode);
      } else if (game_select == GAME2) {
        game2(lcd, lcd_key, interval, mode, game_select);
      } else if (game_select == GAME2END) {
        game2_end(lcd, lcd_key, interval, mode, game_select);
      }
    } else if (mode == SETBRIGHT) {
      brigtness();
    }
    //Set randomSeed
    randomSeed(millis());
    last_lcd_key = lcd_key;
    previousMillis[0] = currentMillis;
  }
  
  if (currentMillis - previousMillis[3] > 1) {
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


void select_mode(int key)
{
  interval = 100;
  lcd.setCursor(0,0);
  lcd.print("Plz select mode.");
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
  
  if (key != btnNONE) delay(200);
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
          //reset_var();
          mode = PLAYGAME;
          //interval = 100;
          previousMillis[0] = 0;
          lcd.clear();
        }
        if (game_select == 2) mode = TIME;
        break;
      }
  }
  
  if (key != btnNONE) delay(200);
}
//this performs as EEPROM.write(i, i)
void write_eeprom(int addr, int data)
{
  if (EEPROM.read(addr) != data) {
    EEPROM.write(addr, data);
  }
}
