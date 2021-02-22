/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "stdbool.h"
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
extern unsigned char led_value;					/* defined in funct_led								*/
static bool led_zero = false;
void TIMER0_IRQHandler (void)
{

	if(win==1)
				{
			if(led_zero == 0)
						{	
						LED_Out(255);
						led_zero=1;
						}
					else
						{
						LED_Out(0);
						led_zero = 0;
						}
					}
	else {	
	switch(direzione){
				
				case 0:
					if(!led_zero)
						{	
						LED_On(2);
						led_zero=true;
						}
					else
						{
						LED_Off(2);
						led_zero = false;
						}
						break;
				case 1:
					if(!led_zero )
						{	
						LED_On(3);
						led_zero=true;
						}
					else
						{
						LED_Off(3);
						led_zero = false;
						}				
						break;					
				case 2:
					if(!led_zero)
						{	
						LED_On(0);
						led_zero=true;
						}
					else
						{
						LED_Off(0);
						led_zero = false;
						}
						break;
				case 3:
					if(!led_zero)
						{	
						LED_On(1);
						led_zero=true;
						}
					else
						{
						LED_Off(1);
						led_zero = false;
						}
						break;
	}
}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
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
	/*
			conteggio++;
			if(conteggio % 10 && distance != 0 && button == 2)
				{
				if(led_running_acceso == 0)
				{	
				LED_On(led_running);
				led_running_acceso=1;
				}
				else
				{
				LED_Off(led_running);
				led_running_acceso = 0;
				}
				}
			else  LED_Off(led_running);
	*/
	/*
	LED_Off(led_running);
	disable_timer(1);
  LPC_TIM1->IR = 1;			// clear interrupt flag */
  
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
