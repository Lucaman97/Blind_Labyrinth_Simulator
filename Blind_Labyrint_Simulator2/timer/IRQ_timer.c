/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "stdbool.h"
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../variables.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	//static int clear = 0;
	//char time_in_char[5] = "";
	
  getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
	#ifdef SIMULATOR
	if(!start){
	#elif
	if(display.x <= 240 && display.x > 0 && display.y != 0){
	#endif
	if(display.x <= 240 && display.x > 0 && display.y < 317){
		
		startGame();
		game_init(); 
			
			
	}
	}
	else if (start){

		if((display.x <= 105 && display.x > 15) && (display.y >= 270 &&display.y < 300)){
		//restartGame
		restartGame();
		}
		else if((display.y >= 270 &&display.y < 300) && (display.x <= 225 && display.x > 135) && win==0){	
			//clear
			game_init();
		}
		}
		
	
  LPC_TIM0->IR = 1;			// clear interrupt flag 
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
