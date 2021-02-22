/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons and manage the bouncing effect
 *        	- key1 switches on the led at the left of the current led on, 
 *					- it implements a circular led effect. 	
  * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "stdbool.h"
#include "variables.h"
/* Led external variables from funct_led */
extern unsigned char led_value;					/* defined in funct_led								*/
#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


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

void startGame(){
start = 1;
}
				

				
bool led_est_acceso = false;
bool led_ovest_acceso = false;
bool led_nord_acceso = false;				
bool led_sud_acceso = false;			
			
	
const int led_running = 5;  
bool led_running_acceso = false;	
/*
 0 -> est
 1 -> nord
 2 -> ovest
 3 -> sud
*/				
//int direzione_arr[] = {0,1,2,3};			
int direzione = 0; 
int pos_X = 7;
int pos_Y = 7;
int distance = 5;
int button = -1;
int win=0;
int win_flag=0;
int conteggio;
bool start=false;

void runTimer(){
		reset_timer(0);
		LED_Out(0);
	if(win==1) 
		{
			LED_Out(0);
			reset_timer(0);
			init_timer(0, 25000000/(2*1));
			enable_timer(0);
		}
	else if(distance > 5)	{
		switch(direzione){
			case 0:
				LED_Off(0);
				break;
			case 1:
				LED_Off(1);
				break;
			case 2:
				LED_Off(2);
				break;
			case 3:
				LED_Off(0);
				break;
		}
		}
		else if((distance >= 3 && distance <= 5) && win_flag==0) 
		{
			init_timer(0, 25000000/(2 *2));
			enable_timer(0);
		}
		else if((distance > 0 && distance <3)&& win_flag==0) 
		{
			init_timer(0, 25000000/(2 *4));
			enable_timer(0);
		}
		else if (win_flag==0)
			{
			init_timer(0, 25000000/(2 *5));
			enable_timer(0);	
			}

	
}

void run(void){
calc_distance();
if(distance<=0){}
else move();
runTimer();

}

void rotate(){
	if(direzione == 0) direzione = 3;
	else direzione = direzione - 1;
	LED_Out(0);
	calc_distance();
	runTimer();
}


void buttonRun(void){
	if(start)	run();
	//runLed();
}

void buttonStart(void){
	startGame();
	calc_distance();
	runTimer();
}

void buttonRotate(void){
	if(start){
	rotate();	
	calc_distance();
	}
}

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
	
}
	

void move(void){
if((pos_X>=0 && pos_X<=15) && (pos_Y>=0 && pos_Y<=12)){
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
}
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/	


int main (void) {
		
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	//init_timer(0,312500);									//0x017D7840	
	init_RIT(1250000);										/* RIT Initialization 50 msec       */
	LED_Out(0);														//spegniamo tutti i led

	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);	
	
  while (1) {                    				 //       Loop forever                      
		__ASM("wfi");
		
		
  }
	
}
