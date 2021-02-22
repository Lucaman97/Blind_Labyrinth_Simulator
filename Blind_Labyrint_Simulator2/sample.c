/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "stdbool.h"
#include "variables.h"
#include "joystick/joystick.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


int direzione = 0; 
int pos_X = 7;
int pos_Y = 7;
int distance = 5;
int button = -1;
int win=0;
int win_flag=0;
int conteggio;
bool start=false;
//mode=0: explore
//mode=1: move
int mode=1;

int labyrinth[13][15] = {
				{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
				{0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
				{1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
				{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
				{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
				{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},					
				};

		
				
				
			


				
				
void drawObstacles(int distance){
	//Questa funzione disegna un quadrato di un colore arbitrario
	int i;
	switch(direzione){
		case 0:
			if( (pos_X + distance > 0) && (pos_X + distance < 14)){
			for(i=1; i<=13; i++){
			LCD_DrawLine(15 +14*pos_X +14*(1+ distance) +i,  40+1 +14*pos_Y,  15 +14*pos_X +14*(1+ distance) + i  ,40+13 +14*pos_Y, Red);
			}
			}
			break;
		case 1:
			if( (pos_Y - distance > 0) && (pos_Y - distance < 12)){
			for(i=1; i<=13; i++){
			LCD_DrawLine(15 +14*pos_X +i,  40+1 +14*pos_Y  -14*(1+ distance),  15 +14*pos_X  +i ,40+13 +14*pos_Y  -14*(1+ distance), Red);
			}
			}
			break;
		case 2:
			if( (pos_X - distance > 0) && (pos_X - distance < 14)){
			for(i=1; i<=13; i++){
			LCD_DrawLine(15 +14*pos_X -14*(1+ distance) +i,  40+1 +14*pos_Y,  15 +14*pos_X -14*(1+ distance) + i  ,40+13 +14*pos_Y, Red);
			}
			}
			break;
		case 3:
			if( (pos_Y + distance > 0) && (pos_Y + distance < 12)){
				for(i=1; i<=13; i++){
				LCD_DrawLine(15 +14*pos_X +i,  40+1 +14*pos_Y  +14*(1+ distance),  15 +14*pos_X  +i ,40+13 +14*pos_Y  +14*(1+ distance), Red);
				}
			}
			break;
		
	}
	//LCD_DrawLine(15+2 +14*pos_X,  40+2 +14*pos_Y,  15+2 +14*pos_X  ,50+2 +14*pos_Y, color);
}



int color;
				
void drawBot(){
	clearBot();
	if(mode==0) color=Black;
	else if(mode==1) color=Red;
	switch(direzione){
		case 0:
			//linea verticale 
			LCD_DrawLine(15+2 +14*pos_X,  40+2 +14*pos_Y,  15+2 +14*pos_X  ,50+2 +14*pos_Y, color);
			//linea decrescente 
			LCD_DrawLine(15+2+14*pos_X,40+2+14*pos_Y,15+14-2+14*pos_X,45+2+14*pos_Y, color);
			//linea crescente 
			LCD_DrawLine(15+2+14*pos_X,50+2+14*pos_Y,15+14-2+14*pos_X,45+2+14*pos_Y, color);

			break;
		case 1:
			//linea orizzontale 
			LCD_DrawLine(15+2+14*pos_X, 50+2 +14*pos_Y, 15+2 +14-4+14*pos_X, 50+2 +14*pos_Y, color);
			//linea crescente 
			LCD_DrawLine(15+ 2+14*pos_X, 50+2 +14*pos_Y, 15+2 +5+14*pos_X, 50+2 -10+14*pos_Y, color);
			//linea decrescente 
			LCD_DrawLine(15+2 +14-4+14*pos_X, 50+2 +14*pos_Y, 15+2 +5+14*pos_X, 50+2 -10+14*pos_Y, color);
			break;
		case 2:
			//linea verticale 
			LCD_DrawLine(15+14-2+14*pos_X,  40+2 +14*pos_Y,  15+14-2+14*pos_X  ,40+2 +10+14*pos_Y, color);
			//linea decrescente 
			LCD_DrawLine(15+14-2+14*pos_X,  40+2 +14*pos_Y, 15+2  +14*pos_X, 45+2 +14*pos_Y, color);
			//linea crescente 
			LCD_DrawLine(15+14-2+14*pos_X, 	50+2 +14*pos_Y, 15+2  +14*pos_X, 45+2 +14*pos_Y, color);
			break;
		case 3:
			//linea orizzontale 
			LCD_DrawLine(15+2 +14*pos_X,  40+2 +14*pos_Y,  15+2 +10+14*pos_X  ,40+2 +14*pos_Y, color);
			//linea decrescente 
			LCD_DrawLine(15+2 +14*pos_X, 40+2 +14*pos_Y, 15+2 +5+14*pos_X, 40+2 +10+14*pos_Y, color);
			//linea crescente 
			LCD_DrawLine(15+2 +10+14*pos_X, 40+2 +14*pos_Y, 15+2 +5+14*pos_X, 40+2 +10+14*pos_Y, color);
			break;
	}
}				


void restartGame(){
			win=0;
			clearBot();
			pos_X=7;
			pos_Y=7;
			mode=1;
			direzione=0;
			game_init();
	
}


 void drawButtons()
{
	int i;
	//larghezza bottone: 6, padding laterale: 1, padding tra bottoni: 1
	//(unità di misura: caselle di 14 o 15 pixel)
	for(i=270; i<300; i++){
	LCD_DrawLine(15,i,105,i,White);	
	}
		for(i=270; i<300; i++){
	LCD_DrawLine(135,i,225,i,White);	
	}

	LCD_DrawLine(15,270,105,270,Black);
	LCD_DrawLine(15,300,105,300,Black);
	
	LCD_DrawLine(135,270,225,270,Black);
	LCD_DrawLine(135,300,225,300,Black);
	
	LCD_DrawLine(15,270,15,300,Black);
	LCD_DrawLine(105,270,105,300,Black);
	
	LCD_DrawLine(135,270,135,300,Black);
	LCD_DrawLine(225,270,225,300,Black);
	
	
	GUI_Text(30, 277, (uint8_t *) "Restart", Black, White);	
	GUI_Text(150, 277, (uint8_t *) " Clear", Black, White);	
	//LCD_DrawLine(15,285,127,290,Black);
	//LCD_DrawLine(x,40,x,222,Black);
}


void drawGrid()
{
	int x;
	int y;
	//disegno lo sfondo bianco
	
	for(y=41; y<=221; y++){
			LCD_DrawLine(16,y,224,y,White);
	}
	
	//disegno le linee verticali
	for(x=15; x<=225; x=x+14){
			LCD_DrawLine(x,40,x,222,Black);
	}
	
	//disegno le linee orizzontali
	for(y=40; y<=225; y=y+14){
			LCD_DrawLine(15, y, 225, y, Black);
	}
}	

void startGame(){
start = true;
}
				

/*				
 0 -> est
 1 -> nord
 2 -> ovest
 3 -> sud
*/				
//int direzione_arr[] = {0,1,2,3};			

void clearBot(){
			int local_color;
			local_color=White;
			//linea verticale 
			LCD_DrawLine(15+2 +14*pos_X,  40+2 +14*pos_Y,  15+2 +14*pos_X  ,50+2 +14*pos_Y, local_color);
			//linea decrescente 
			LCD_DrawLine(15+2+14*pos_X,40+2+14*pos_Y,15+14-2+14*pos_X,45+2+14*pos_Y, local_color);
			//linea crescente 
			LCD_DrawLine(15+2+14*pos_X,50+2+14*pos_Y,15+14-2+14*pos_X,45+2+14*pos_Y, local_color);

			//linea orizzontale 
			LCD_DrawLine(15+2+14*pos_X, 50+2 +14*pos_Y, 15+2 +14-4+14*pos_X, 50+2 +14*pos_Y, local_color);
			//linea crescente 
			LCD_DrawLine(15+ 2+14*pos_X, 50+2 +14*pos_Y, 15+2 +5+14*pos_X, 50+2 -10+14*pos_Y, local_color);
			//linea decrescente 
			LCD_DrawLine(15+2 +14-4+14*pos_X, 50+2 +14*pos_Y, 15+2 +5+14*pos_X, 50+2 -10+14*pos_Y, local_color);

			//linea verticale 
			LCD_DrawLine(15+14-2+14*pos_X,  40+2 +14*pos_Y,  15+14-2+14*pos_X  ,40+2 +10+14*pos_Y, local_color);
			//linea decrescente 
			LCD_DrawLine(15+14-2+14*pos_X,  40+2 +14*pos_Y, 15+2  +14*pos_X, 45+2 +14*pos_Y, local_color);
			//linea crescente 
			LCD_DrawLine(15+14-2+14*pos_X, 	50+2 +14*pos_Y, 15+2  +14*pos_X, 45+2 +14*pos_Y, local_color);

			//linea orizzontale 
			LCD_DrawLine(15+2 +14*pos_X,  40+2 +14*pos_Y,  15+2 +10+14*pos_X  ,40+2 +14*pos_Y, local_color);
			//linea decrescente 
			LCD_DrawLine(15+2 +14*pos_X, 40+2 +14*pos_Y, 15+2 +5+14*pos_X, 40+2 +10+14*pos_Y, local_color);
			//linea crescente 
			LCD_DrawLine(15+2 +10+14*pos_X, 40+2 +14*pos_Y, 15+2 +5+14*pos_X, 40+2 +10+14*pos_Y, local_color);


}




void changeMode(){
		if(mode==1) mode=0;
		else if(mode==0) mode=1;
		drawBot();
}

void run(void){
calc_distance();
clearBot();
if(distance<=0){}
else move();
//runTimer();

}
/*
void rotate(){
	if(win==0){
	if(direzione == 0) direzione = 3;
	else direzione = direzione - 1;
	//LED_Out(0);
	calc_distance();
	//runTimer();
	}
}
*/

void buttonRun(void){
	if(start)	run();
	if(win==1) start=true;
}

/*
void buttonStart(void){
	LCD_Clear(Blue2);
	drawGrid();
	startGame();
	calc_distance();
	//runTimer();
}

void buttonRotate(void){
	if(start){
	rotate();	
	calc_distance();
	}
}
*/
void calc_distance(void){
	int i=0;
	if(direzione == 0)
		{
		//Calcolo distanza verso est
		
		win_flag=0;	
		for(i = pos_X+1; i<15; i++)
			{
			if(labyrinth[pos_Y][i] == 1) break;
			if(labyrinth[pos_Y][i] == 2) 
			{
			win_flag=1;
			}
			}
		distance = i - pos_X - 1 ;
		}
	//Calcolo distanza verso nord
	else if(direzione == 1)
		{
		
		win_flag=0;
		for(i = pos_Y-1; i>=0; i--){
			if(labyrinth[i][pos_X] == 1) break;
			if(labyrinth[i][pos_X] == 2) 
			{
			win_flag=1;
			}
		}
		distance = pos_Y - i - 1;
		}
	//Calcolo distanza verso ovest
	else if(direzione == 2)
		{
	
	win_flag=0;	
	for(i = pos_X-1; i>=0; i--){
			if(labyrinth[pos_Y][i] == 1) break;
			if(labyrinth[pos_Y][i] == 2) 
			{
			win_flag=1;
			}
		}
		distance = pos_X - i - 1;
		}
	
	//Calcolo distanza verso sud
	else if(direzione == 3){
		win_flag=0;
		for(i = pos_Y+1; i<13; i++){
			if(labyrinth[i][pos_X] == 1) break;
			if(labyrinth[i][pos_X] == 2) 
			{
			win_flag=1;
			}
		}
	distance = i - pos_Y - 1;
	}
	if(distance <= 5) drawObstacles(distance);
}
	



void move(void){
if((pos_X>=0 && pos_X<=15) && (pos_Y>=0 && pos_Y<=12) && win==0){
switch(direzione){
	case 0:
		if(labyrinth[pos_Y][pos_X+1] != 1)
			{ 
			pos_X++;
			break;
			}
	case 1:
		if(labyrinth[pos_Y-1][pos_X] != 1)
		pos_Y--;
		break;
	case 2:
		if(labyrinth[pos_Y][pos_X-1] != 1)
		pos_X--;
		break;
	case 3:
		if(labyrinth[pos_Y+1][pos_X] != 1)
		pos_Y++;
		break;
}
if(labyrinth[pos_Y][pos_X]==2) win=1;
calc_distance();

if(win==1){ 
	GUI_Text(14*7, 230, (uint8_t *) "YOU WON!", Red, Blue2);
	//GUI_Text(14*5, 245, (uint8_t *) "Press restart.", Red, Blue2);
	}
}
}



void game_init(){
	
	LCD_Clear(Blue2);
	drawGrid();
	drawButtons(); 
	drawBot();
}

	  
//Grid: 240 * 316
int main(void)
 {
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	init_RIT(1250000);	
	joystick_init();	
  LCD_Initialization();

  
	TP_Init();
	 
	TouchPanel_Calibrate();
	LCD_Clear(Blue2);
	drawGrid();
	GUI_Text(15, 105, (uint8_t *) " TOUCH THE SCREEN TO START ", Blue, White);
	//game_init();
	
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	init_timer(0, 0x6108 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
 
	enable_timer(0);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		enable_RIT();
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
