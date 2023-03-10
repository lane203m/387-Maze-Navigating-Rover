/*
	ENEL 387 - Rover Project - Main
	Handles start/stop/line-follower logic 
	Also handles initialization
	
	This device will navigate a course & 
	detect/read target information along 
	the way
*/

/*
	By: 	Mason Lane
				200376573
	Date: 11-04-2021
*/

#include "gpio.h"
#include "clock.h"
#include "stm32f10x.h"
//#include "stdio.h"
int main(void){
	clockInit();		//init clock
	
	adcInit();			//init adc
	openClocks();
	led_IO_init();	//init LEDs

	initStop();			//init stop logic ports
  state = 1;
	while(state == 1) {	//break once start button is pressed
		if((GPIOC->IDR & GPIO_IDR_IDR0) != GPIO_IDR_IDR0){
			state = 0;
		}
		
	}
	
	motorController();	//init motor controller
	tim3GpioSetup();		//init tim3 gpio
	initializePWM();		//init PWM using tim3 on ch3 & 4
	initExt1();					//init interrupts
	initExt2();
	delay(1000000);			//pause
	unsetLeds();				//unsetLEDs to indicate running

	//IR Line Sensor Inputs
	uint32_t adcInputLeft = adcRead2();	
	uint32_t adcInputMid = adcRead2();		
	uint32_t adcInputRight = adcRead2();	

	state = 1;
	
	//Proceed forward until robot has left the black square
	while(state == 1) {
		adcInputMid = adcRead2();		
		adcInputRight = adcRead3();
		adcInputLeft = adcRead1();
		setMotor('F', 50, 'F', 50);
		if(((GPIOA->IDR & GPIO_IDR_IDR8) != GPIO_IDR_IDR8) || ((GPIOA->IDR & GPIO_IDR_IDR9) != GPIO_IDR_IDR9)){
			state = 0;
		}
		
	}
	
	//normal running. All interrupts & line follower are ready.
	while(state == 0){
		
		//read line follower input
		adcInputMid = adcRead2();		
		adcInputRight = adcRead3();
		adcInputLeft = adcRead1();
		
		//if robot has re-eneted the black square, stop.
		if(((GPIOA->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8) && ((GPIOA->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && adcInputMid > 2700 && adcInputLeft > 2700 && adcInputRight > 2700){
			state = 1;
		}
		
		//set robots movement using line follower
		setLineDirection();

		//if red button is pressed, stop the loop
		if((GPIOC->IDR & GPIO_IDR_IDR0) != GPIO_IDR_IDR0){
			state = 1;
		}	
		
		//contingency to stop the loop
		if(state == 1){
			break;
		}
	}
	
	//stay in place until reset or interrupt 

	setMotor('S', 1, 'S', 1);

}
