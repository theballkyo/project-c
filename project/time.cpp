#include "Arduino.h"
#include "time.h"
#include "config.h"
#include <EEPROM.h>
//#include "Wire.h"

int temp_time[6]        = {0,0,0,0,0,0};

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
  sprintf(buffer1,"%02d:%02d:%02d %d",now.hour(),now.minute(),now.second(), (int)floor(RTC.getTemperature()));
  Serial.println(RTC.getTemperature());
  lcd.print(buffer1);
  lcd.write(byte(0));
  sprintf(buffer1,"%s:%02d/%s/%04d", day_short_t[now.dayOfWeek()], now.day(), month_short_t[now.month()-1], now.year());
  lcd.setCursor(0,1);
  //lcd.print(String(day_short_t[weeknow.day()-1]) + "/" + month_short_t[now.month()-1] + "/" + now.year());
  //int w = weeknow.day();
  //lcd.print(String(day_t[6]) + "/" + month_t[1]);
  lcd.print(buffer1);
  if(alarm) lcd.write((byte)1);
  Serial.print(buffer1);
  Serial.println();
  shit += 1;
  if (shit >=5)
    shit = 0;
}

void set_time(int key)
{
  DateTime now = RTC.now();
  // Set delay
  interval = 100;
  lcd.setCursor(0, 0);
  //lcd.print("                ");
  //Serial.print(set_sel);
  // Set cooldown blink = 3
  sprintf(buffer1,"%02d:%02d:%02d",temp_time[HOUR],temp_time[MINUTE],now.second());
  sprintf(buffer2,"%02d/%4s/%04d", temp_time[DAY], month_short_t[temp_time[MONTH]-1], temp_time[YEAR]);
  if (switch_cd(2,4)) {
    if (set_sel == SECOND) {
      sprintf(buffer1,"%02d:%02d:  ",temp_time[HOUR],temp_time[MINUTE]);
    } else if (set_sel == MINUTE) {
      sprintf(buffer1,"%02d:  :%02d",temp_time[HOUR],now.second());
    } else if (set_sel == HOUR) {
      sprintf(buffer1,"  :%02d:%02d",temp_time[MINUTE],now.second());
    } else if (set_sel == DAY) {
      sprintf(buffer2,"  /%4s/%04d", month_short_t[temp_time[MONTH]-1], temp_time[YEAR]);
    } else if (set_sel == MONTH) {
      sprintf(buffer2,"%02d/    /%04d", temp_time[DAY], temp_time[YEAR]);
    } else if (set_sel == YEAR) {
      sprintf(buffer2,"%02d/%4s/    ", temp_time[DAY], month_short_t[temp_time[MONTH]-1]);
    } 
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
              RTC.adjust(DateTime(temp_time[YEAR], temp_time[MONTH], temp_time[DAY], temp_time[HOUR], temp_time[MINUTE], 0));
              //temp_time[SECOND] = 0;
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
              RTC.adjust(DateTime(temp_time[YEAR], temp_time[MONTH], temp_time[DAY], temp_time[HOUR], temp_time[MINUTE], 0));
              //temp_time[SECOND] = 0;
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
        if (is_click < LONGCLICK) mode = TIME, interval = 1000, previousMillis[0] = 0, lcd.clear(), RTC.adjust(DateTime(temp_time[YEAR], temp_time[MONTH], temp_time[DAY], temp_time[HOUR], temp_time[MINUTE], now.second()));
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
  char alarm_t[2][5] = {"OFF", "ON "};
  interval = 100;
  sprintf(buffer1, "%02d:%02d:%02d",alarm_time[HOUR], alarm_time[MINUTE], alarm_time[SECOND]);
  sprintf(buffer2,"Alarm is %s",alarm_t[alarm]);
  if (switch_cd(2,4)) {
    if (set_sel == SECOND) {
      sprintf(buffer1,"%02d:%02d:  ",alarm_time[HOUR],alarm_time[MINUTE]);
    } else if (set_sel == MINUTE) {
      sprintf(buffer1,"%02d:  :%02d",alarm_time[HOUR],alarm_time[SECOND]);
    } else if (set_sel == HOUR) {
      sprintf(buffer1,"  :%02d:%02d",alarm_time[MINUTE],alarm_time[SECOND]);
    } else if (set_sel == 3) {
      sprintf(buffer2,"Alarm is       ");
    }
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
        if (is_click < LONGCLICK) {
          mode = TIME, interval = 1000, previousMillis[0] = 0, lcd.clear();
          //this performs as EEPROM.write(i, i)
          write_eeprom(ADDRS, alarm_time[SECOND]);
          write_eeprom(ADDRM, alarm_time[MINUTE]);
          write_eeprom(ADDRH, alarm_time[HOUR]);
          write_eeprom(ADDRA, alarm);
        }
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

int is_alarming()
{
  if (is_alarm == 1) return is_alarm;
  DateTime now = RTC.now();
  //Serial.println("alarm");
  if (now.hour() == alarm_time[HOUR] && now.minute() == alarm_time[MINUTE] && now.second() == alarm_time[SECOND] && alarm == 1 && mode != SETALARM) {
      Serial.println("alarm");
      is_alarm = 1;
  }
  
  // Alarm over 1 minute
  if (is_alarm == 1 && (now.minute() > alarm_time[MINUTE] || (now.minute() == 0 && alarm_time[MINUTE] == 59)) && is_pongpongpong == 0) {
      //Serial.println("test");
      is_alarm = 0;
  }
  return is_alarm;
}

int stop_alarm()
{
  if (is_pongpongpong == 1) return 0;
  is_alarm = 0;
  analogWrite (speakerPin, 0);
}

void stop_alarm(int i)
{
  is_alarm = 0;
  analogWrite (speakerPin, 0);
}

void play_sound_alarm()
{
    if (currentMillis - previousMillis[1] > sound_interval[1]) {
      analogWrite (speakerPin, 255);
      previousMillis[1] = currentMillis;
        //delay(100);
      if (currentMillis - previousMillis[2] > sound_interval[0] + sound_interval[1]) {
        previousMillis[1] = currentMillis;
        previousMillis[2] = currentMillis;
        analogWrite (speakerPin, 0);
      }
        //delay(5);
    }
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
