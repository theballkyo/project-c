#include <LiquidCrystal.h>

extern int game_pos_me[2];
extern int game_dm[2][16];
extern int game_ans;
extern int cd;
extern int last_lcd_key;
extern int is_pongpongpong;
extern int is_alarm;
extern void reset_var();

void game2(LiquidCrystal, int, int&, int&, int&);

int game2_end(LiquidCrystal, int, int&, int&, int&);
