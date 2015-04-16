#include "SPI.h"
#include "RTClib.h"

#include <LiquidCrystal.h>

extern int alarm_time[4];
extern RTC_DS3231 RTC;
extern int interval;
extern LiquidCrystal lcd;
extern int shit;
extern char buffer1[18];
extern char buffer2[18];
extern char month_short_t[12][10];
extern char day_short_t[7][4];
extern int alarm;
extern long int cd;
extern int is_click;
extern int mode;
extern int set_sel;
extern int is_vision;
extern unsigned int previousMillis[4];
extern int is_alarm;
extern int last_lcd_key;
extern unsigned int currentMillis;
extern int sound_interval[2];
extern int lcd_key;
extern int is_pongpongpong;

extern void write_eeprom(int, int);
extern int switch_cd(int, int);
void show_time();
void set_time();
void set_alarm();
int is_alarming();
void play_sound_alarm();
void set_temp_time();
