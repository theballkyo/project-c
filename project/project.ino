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
//Check button press time
int t_press        = 0;
int set_sel        = 2;
int current_select = 0;
//
int is_vision      = 0;
int is_click       = 0;
int is_pongpongpong = 0;
unsigned int previousMillis = 0; 
// time[3] > {second, minute, hour}
int temp_time[6]        = {0,0,0,0,0,0};  

char time_bf[18]    = "";
char buffer_[18]    = "";
char month_t[12][11]  = {"January","February","March","April","May","June","July","August","September","October","November","December"};
char month_short_t[12][10] = {"Jan","Feb","Mar","Apr","May","June","July","Aug","Sep","Oct","Nov","Dec"};
char day_t[7][10]     = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
char day_short_t[7][3] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};

//Mode
#define NUMMODE    4
#define SELECTMODE -1
#define TIME       0
#define SETTIME    1
#define SETALARM   2
#define GAME       3
#define PONGPONGPONG 99

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
   setTime(1,2,3,28,3,2015);
   lcd.begin(16, 2);              // start the library
   lcd.setCursor(0,0);
   lcd.print(hour()); // print a simple message
   Serial.begin(9600);
}
 
void loop()
{
  unsigned int currentMillis = millis();
  
  //lcd.setCursor(8,1);            // move cursor to second line "1" and 9 spaces over
  //lcd.print(t_press);      // display seconds elapsed since power-up
  lcd_key = read_LCD_buttons();
  
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
    }
    
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
  sprintf(time_bf,"%02d:%02d:%02d",hour(),minute(),second());
  
  lcd.print(time_bf);

  sprintf(time_bf,"%s/%s/%04d", day_short_t[weekday()-1], month_short_t[month()-1], year());
  lcd.setCursor(0,1);
  //lcd.print(String(day_short_t[weekday()-1]) + "/" + month_short_t[month()-1] + "/" + year());
  //int w = weekday();
  //lcd.print(String(day_t[6]) + "/" + month_t[1]);
  lcd.print(time_bf);
  Serial.print(time_bf);
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
  sprintf(time_bf,"%02d:%02d:%02d",temp_time[HOUR],temp_time[MINUTE],temp_time[SECOND]);
  sprintf(buffer_,"%02d/%4s/%04d", temp_time[DAY], month_short_t[temp_time[MONTH]-1], temp_time[YEAR]);
  if (is_vision == 4) {
    if (set_sel == SECOND) {
      sprintf(time_bf,"%02d:%02d:  ",temp_time[HOUR],temp_time[MINUTE]);
    } else if (set_sel == MINUTE) {
      sprintf(time_bf,"%02d:  :%02d",temp_time[HOUR],temp_time[SECOND]);
    } else if (set_sel == HOUR) {
      sprintf(time_bf,"  :%02d:%02d",temp_time[MINUTE],temp_time[SECOND]);
    } else if (set_sel == DAY) {
      sprintf(buffer_,"  /%4s/%04d", month_short_t[temp_time[MONTH]-1], temp_time[YEAR]);
    } else if (set_sel == MONTH) {
      sprintf(buffer_,"%02d/    /%04d", temp_time[DAY], temp_time[YEAR]);
    } else if (set_sel == YEAR) {
      sprintf(buffer_,"%02d/%4s/    ", temp_time[DAY], month_short_t[temp_time[MONTH]-1]);
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
  
  if (key < 5) delay(200);
  if (temp_time[MINUTE] > 59) temp_time[MINUTE] = 0;
  if (temp_time[HOUR] > 23) temp_time[HOUR] = 0;
  if (temp_time[DAY] > 31) temp_time[DAY] = 1;
  if (temp_time[MONTH] > 12) temp_time[MONTH] = 1;
  
  if (temp_time[MINUTE] < 0) temp_time[MINUTE] = 59;
  if (temp_time[HOUR] < 0) temp_time[HOUR] = 23;
  if (temp_time[DAY] < 1) temp_time[DAY] = 31;
  if (temp_time[MONTH] < 1) temp_time[MONTH] = 12;
  
  lcd.print(time_bf);
  lcd.setCursor(0, 1);
  lcd.print(buffer_);
  
}

void select_mode(int key)
{
  lcd.setCursor(1,1);
  lcd.print("             ");
  lcd.setCursor(1,1);
  if (current_select == TIME) {
    lcd.print("Time");
  } else if (current_select == SETTIME) {
    lcd.print("Set time.");
  } else if (current_select == SETALARM) {
    lcd.print("Set alarm.");
  } else if (current_select == GAME) {
    lcd.print("Play game Yeah!");
  }
  
  switch (key)
  {
    case btnDOWN:
      {
        current_select += 1;
        if(current_select >= NUMMODE) current_select = 0;
        break; 
      }
    case btnUP:
      {
        current_select -= 1;
        if(current_select < 0) current_select = NUMMODE-1;
        break; 
      }
    case btnSELECT:
      {
        if (is_click < LONGCLICK){
          mode = current_select;
          interval = 1000;
          previousMillis = 0;
          lcd.clear();
          if (mode == SETTIME) {
            set_temp_time();
          }
        }
        break;
      }
  }
  
  if (key < 5) delay(200);
}

void set_temp_time()
{
  temp_time[0] = second();
  temp_time[1] = minute();
  temp_time[2] = hour();
  temp_time[3] = day();
  temp_time[4] = month();
  temp_time[5] = year();
}
void generate()
{
    int a = random(1,100);
    int b = random(1,100);
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print(String(a) + "+" + String(b) + " = " + String(a+b));
}


