#include <LiquidCrystal.h>

extern int is_generate;
extern int a;
extern int b;
extern int c;
extern int d;
extern int game_c[3];
extern int game_ans;
extern int game_ans_c;
extern int game_select_c;
extern int lcd_key;
extern int last_lcd_key;
extern int interval;
extern int mode;
extern int is_pongpongpong;
extern char buffer1[18];
extern char buffer2[18];

extern void stop_alarm(int);
extern LiquidCrystal lcd;
extern void reset_var();
void game1();
void game1_config(unsigned int&);
void game1_run(unsigned int, int&);
void game1_end(unsigned int&, int&);

