#include "stdbool.h"


extern bool start;

extern int direzione;
extern int button;


extern int pos_X;
extern int pos_Y;
extern int distance;
extern int win;
extern int win_flag;
extern int conteggio;	
extern int mode;
extern void game_init();

extern void rotate(void);
extern void run(void);
extern void calc_distance(void);
extern int labyrinth[13][15];
extern void buttonRotate(void);
extern void buttonRun();
extern void buttonStart();
extern void move();
extern void drawButtons(); 
extern void drawBot();		
extern void drawGrid();
extern void clearBot();
extern void	restartGame();
extern void startGame();
