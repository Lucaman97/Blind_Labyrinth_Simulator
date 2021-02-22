#include "stdbool.h"


#define led_est 2
#define led_ovest 0
#define led_nord  3				
#define led_sud  1				
				
extern bool led_est_acceso;
extern bool led_ovest_acceso;
extern bool led_nord_acceso;				
extern bool led_sud_acceso;
extern bool led_running_acceso;	
extern bool start;

extern int direzione;
extern int button;


extern int const led_running;
extern bool led_zero;

extern int pos_X;
extern int pos_Y;
extern int distance;
extern int win;
extern int win_flag;
extern int conteggio;	

extern void rotate(void);
extern void run(void);
extern void calc_distance(void);
extern int labyrinth[13][15];
extern void buttonRotate(void);
extern void buttonRun();
extern void buttonStart();
extern void move();
		
