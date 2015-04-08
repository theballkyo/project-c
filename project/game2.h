#include <LiquidCrystal.h>

extern int game_pos_me[2];
extern int game_dm[2][16];
extern int game_ans;
extern int cd;
extern int last_lcd_key;
extern int is_pongpongpong;
extern int is_alarm;
extern int lcd_key;
extern int interval;
extern int mode;
extern int last_mode;
extern int game_select;
extern int current_select;
extern void reset_var();

extern LiquidCrystal lcd;
extern int read_eeprom(int);
extern void write_eeprom(int,int);
extern void stop_alarm(int);

void game2();
void game2_run();
void game2_config();
void game2_level();

int game2_end();
