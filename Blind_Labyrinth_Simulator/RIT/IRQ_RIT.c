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
#include "../led/led.h"
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
	//static int down =0;
	static int down0=0;	
	static int down1=0;
	static int down2=0;
	
	//10, 11, 12
	//button0, button1, button2
	
if(button==0){
	down0++;

	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0 && !(LPC_PINCON->PINSEL4 &(1<<20))){
		//	static uint8_t position=0;
		reset_RIT();
		switch(down0){
			case 1:
				buttonStart();
			
				/*if( position == 7){
					LED_On(0);
					LED_Off(7);
					position = 0;
					*/
			
			}
		}else{ /* button released */
				
				down0 = 0;
				button = -1;
				disable_RIT();
				reset_RIT();
				NVIC_EnableIRQ(EINT0_IRQn);
				LPC_PINCON->PINSEL4 |= (1 << 20);
					//LED_Off(position);
					//LED_On(++position);
				}
}else if(button==1){
	down1++;
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0 && !(LPC_PINCON->PINSEL4 &(1<<22))){
		//	static uint8_t position=0;
		

		reset_RIT();
		
		}else{ /* button released */
				if(down1>3)	buttonRotate();
				down1 = 0;
				button = -1;
				disable_RIT();
				reset_RIT();
				NVIC_EnableIRQ(EINT1_IRQn);
				LPC_PINCON->PINSEL4 |= (1 << 22);
					//LED_Off(position);
					//LED_On(++position);
				}
		}		
else if(button==2){ //Bottone per muovere il robot = run()

	
if((LPC_GPIO2->FIOPIN & (1<<12)) == 0 && !(LPC_PINCON->PINSEL4 &(1<<24))){
		//	static uint8_t position=0;
		down2++;
		reset_RIT();
		
		if(down2%40==0){
				buttonRun();
			}
		if(distance==0 && win!=1)
			{
			if(down2%20==10) LED_On(led_running);
			if(down2%20==0) LED_Off(led_running);
			}
		else if(win!=1)
		{
		if(down2%40==20) LED_On(led_running);
		if(down2%40==0) LED_Off(led_running);
		}
		
		}else{ /* button released */
				if(win!=1) LED_Off(led_running);
				down2 = 0;
				button = -1;
				disable_RIT();
				reset_RIT();
				NVIC_EnableIRQ(EINT2_IRQn);
				LPC_PINCON->PINSEL4 |= (1 << 24);
				}
		}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
