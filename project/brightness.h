#include "config.h"
#include <LiquidCrystal.h>

extern LiquidCrystal lcd;
extern int lcd_key;
extern int interval;
extern int mode;
extern void write_eeprom(int, int);
void brigtness();

int get_bright_lv();
