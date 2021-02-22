/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../variables.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/




void RIT_IRQHandler (void)
{					
	static int press=1;
	static int right=1;
	static int left=1;
	static int up=1;
	static int down=1;
		
		//PRESS
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0 && start){
		/* Joytick Select pressed */
		press++;
		switch(press){
			case 2:
				changeMode();
				break;
			default:
				break;
		}

	}	else{
		press=1;	
		}//DOWN
	if(((LPC_GPIO1->FIOPIN & (1<<26)) == 0) && ((LPC_GPIO1->FIOPIN & (1<<27)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<28)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<29)) != 0) && start){
	down++;
	switch(down%20){
		case 0:
			if(win==0){
			if(mode==1){
			direzione=3;
			buttonRun();
			drawBot();
			}
			
			else{
			direzione=3;
			drawBot();
			calc_distance();
			}
		}
			break;
			default:
			break;
	}	
		}
		else{
			down=1;
	}//LEFT
	if(((LPC_GPIO1->FIOPIN & (1<<26)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<27)) == 0) && ((LPC_GPIO1->FIOPIN & (1<<28)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<29)) != 0) && start){
	left++;
	switch(left%20){
		case 0:
			if(win==0){
			if(mode==1){
			direzione=2;
			buttonRun();
			drawBot();
	}else{
			direzione=2;			
			drawBot();
			calc_distance();
			}
		}
			break;
			default:
			break;
	}	
		}
		else{
			left=1;
	}//RIGHT
	if(((LPC_GPIO1->FIOPIN & (1<<26)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<27)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<28)) == 0) && ((LPC_GPIO1->FIOPIN & (1<<29)) != 0) && start){
	right++;
	switch(right%20){
		case 0:
			if(win==0){
			if(mode==1)	{
			direzione=0;
			buttonRun();
			drawBot();	}
			
			else{
			direzione=0;			
			drawBot();
			calc_distance();}
		}
			break;
		default:
			break;
	}
}
		else{
			right=1;
	}//UP
	if(((LPC_GPIO1->FIOPIN & (1<<26)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<27)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<28)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<29)) == 0) && start){
	up++;
	switch(up%20){
		case 0:
			if(win==0){
			if(mode==1){
			direzione=1;
			buttonRun();
			drawBot();
			}else{
			direzione=1;			
			drawBot();
			calc_distance();			
			}
		}
			break;
		default:
			break;
	}	
		}
	else{
			up=1;
	}
	
	
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	disable_RIT();
	init_RIT(1250000);	
	enable_RIT();
  return;
}

/******************************************************************************
**                            End Of File 
******************************************************************************/
