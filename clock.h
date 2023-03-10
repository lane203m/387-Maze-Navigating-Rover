 /*
	ENEL 387 - Rover Project - clock.h
	Several functions for timers, PWM & interrupt handling
	
	This device will navigate a course & 
	detect/read target information along 
	the way
*/

/*
	By: 	Mason Lane
				200376573
	Date: 11-04-2021
*/
#include <stdint.h>

void delay(uint32_t delay); //delay for a given time
void clockInit(void);
void openClocks(void);
void initSystick(void);
void SysTick_Handler(void);
void initExt1(void);				//target interrupt 
void EXTI15_10_IRQHandler(void);	//target interrupt handler
void EXTI9_5_IRQHandler(void);	//bumper interrupt handler
static int32_t state = (int32_t)0;
static int32_t endState = (int32_t)0;
void initExt2(void);	//bumper interrupt
void initStop(void);	//init IR used for stopping
