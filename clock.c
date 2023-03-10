 /*
	ENEL 387 - Rover Project - clock.c
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
 #include "stm32f10x.h"
 #include "clock.h"
 #include "gpio.h"

 
//**************************** Utility ************************************************************
// i= 6000 is approximately 1 ms @ SYSCLK = 24 MHz

void delay(uint32_t delay)
{
	uint32_t i=0;
	uint32_t b=0;
	uint32_t c=0;
	for(i = 0x0; i< delay; i++)
	{
		 { b=c; }
		
	}
}


//**************************** Clock Configuration ************************************************************
void clockInit(void)
{
  
//* enable HSI and wait for it to be ready
    RCC->CR |= 0x00000001;
    while (((RCC->CR) & 0x00000003) == 0);
			
//* enable HSE with Bypass and wait for it to be ready
    RCC->CR |= 0x00050000;
    while (((RCC->CR) & 0x0007000) == 0);
				
//* enable flash prefetch buffer */
//FLASH->ACR = 0x00000012;
    
////SET HSI as SYSCLK and wait for it to be recognized
//    RCC->CFGR = 0x04000000;
//    while (((RCC->CFGR) & 0x0000000C) != 0);
			
//SET HSE as SYSCLK and wait for it to be recognized
    RCC->CFGR = 0x04000001;
    while (((RCC->CFGR) & 0x00000005) == 0);
			
// Disable PLL. 
		
		RCC->CR &= ~RCC_CR_PLLON;
			
//Change PLL source and set the PLL multiplier
			
//These are the SYSCLK values when using the PLL with HSI/2 as the input. The max value is 64 MHz
		//RCC->CFGR = 0x00000000;// 8MHz
		//RCC->CFGR = 0x00040000;// 12 MHz
		//RCC->CFGR = 0x00080000;// 16 MHz
		//RCC->CFGR = 0x000c0000;// 20 MHz			
		//RCC->CFGR = 0x00100000;// 24 MHz **** MAX SPEED for VLDISCOVERY *****
		//RCC->CFGR = 0x00140000;//	28 MHz
		//RCC->CFGR = 0x00180000;// 32 MHz
		//RCC->CFGR = 0x001C0000;// 36 MHz			
		//RCC->CFGR = 0x00200000;// 40 MHz
		//RCC->CFGR = 0x00240000;//	44 MHz
		//RCC->CFGR = 0x00280000;// 48 MHz
		//RCC->CFGR = 0x002C0000;// 52 MHz
		//RCC->CFGR = 0x003C0000;// 64 MHz			

//These are the SYSCLK values when using the PLL with HSE/Bypass as the input. The max value is 72 MHz
		//RCC->CFGR = 0x00010000;// 16 MHz
		RCC->CFGR = 0x00050000;// 24 MHz **** MAX SPEED for VLDISCOVERY *****
		//RCC->CFGR = 0x00090000;// 32 MHz
		//RCC->CFGR = 0x000d0000;// 40 MHz			
		//RCC->CFGR = 0x00110000;// 48 MHz
		//RCC->CFGR = 0x00150000;//	56 MHz
		//RCC->CFGR = 0x00190000;// 64 MHz
		//RCC->CFGR = 0x001d0000;// 72 MHz			

//ENABLE PLL and wait for it to be ready

    RCC->CR |= RCC_CR_PLLON;
    while (((RCC->CR) & 0x03000000) == 0);
   
// Set PLL as SYSCLK and wait for it to be ready
			
    RCC->CFGR |= 0x00000002;
    while (((RCC->CFGR) & 0x00000008) == 0);
			
// Enable clock visibity on PA8, select PLL/2 as output source
			
		RCC->CFGR |= RCC_CFGR_MCO_PLL;

	
//Enable peripheral clocks for various ports and subsystems
//Bit 4: Port C Bit3: Port B Bit 2: Port A
			
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;

}
//Open clocks
void openClocks(void ){
		RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;
		RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;
		RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;
}

void initSystick(void){
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0x00;
	SysTick->LOAD = 0xB71B00;
	SysTick->CTRL = 0x07;
}

//initializing the bumper interrupt handler
void initExt2(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->EXTICR[2] |= AFIO_EXTICR2_EXTI5_PA;
	AFIO->EXTICR[2] |= AFIO_EXTICR2_EXTI6_PA;
	EXTI->IMR |= EXTI_IMR_MR5;
	EXTI->FTSR |= EXTI_FTSR_TR5;
	EXTI->IMR |= EXTI_IMR_MR6;
	EXTI->FTSR |= EXTI_FTSR_TR6;
	NVIC->ISER[0] |= NVIC_ISER_SETENA_23;
}

//bumper interrupt function 
void EXTI9_5_IRQHandler(void){
	uint32_t turnState = 0;
	while((GPIOA->IDR & GPIO_IDR_IDR6) != GPIO_IDR_IDR6 || (GPIOA->IDR & GPIO_IDR_IDR5) != GPIO_IDR_IDR5){	//if bumpers are triggered
		if((GPIOA->IDR & GPIO_IDR_IDR6) != GPIO_IDR_IDR6 && (GPIOA->IDR & GPIO_IDR_IDR5) != GPIO_IDR_IDR5){	//instruct to reverse wiht bias
			
			turnState = 0;
			//setLed(1);
		}
		else if((GPIOA->IDR & GPIO_IDR_IDR5) != GPIO_IDR_IDR5){ //reverse turn left
			turnState = 1;
			//setLed(2);
		}
		else if((GPIOA->IDR & GPIO_IDR_IDR6) != GPIO_IDR_IDR6){ //reverse turn right
			turnState = 2;
			//setLed(3);
		}
		
		setMotor('B', 45, 'B', 45);	//clear obstalce
		delay(200000);

		//turn away from obstacle based on determined turn state
		if(turnState == 0){
			setMotor('B', 40, 'B', 60); 
			delay(600000);
		}
		if(turnState == 2){
			setMotor('B', 35, 'B', 55);
			delay(550000);
		}
		if(turnState == 1){
			setMotor('B', 50, 'B', 30);
			delay(500000);
		}

		//check if on the black square. if so, stop.
		uint32_t adcInputMid = adcRead2();		
		uint32_t adcInputRight = adcRead3();
		uint32_t adcInputLeft = adcRead1();
		if(((GPIOA->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8) && ((GPIOA->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && adcInputMid > 2700 && adcInputLeft > 2700 && adcInputRight > 2700){
			state = 1;
			setMotor('F', 1, 'F', 1);
		}
		
	}
	
	

	EXTI->PR |= EXTI_PR_PR5;
	

	
	EXTI->PR |= EXTI_PR_PR6;
	if(state == 0){
			setMotor('F', 55, 'F', 55); //go forward once finished and not on the black square
	}

}


//initializing the target counter interrupt
void initExt1(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;				
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->EXTICR[3] |= AFIO_EXTICR3_EXTI10_PA;
	EXTI->IMR |= EXTI_IMR_MR10;
	EXTI->FTSR |= EXTI_FTSR_TR10;
	NVIC->ISER[1] |= NVIC_ISER_SETENA_8;
}

//target counter interrupt function
void EXTI15_10_IRQHandler(void){
		if((GPIOA->IDR & GPIO_IDR_IDR10) != GPIO_IDR_IDR10){ //proceed if IR has found white
			uint32_t count = 0;
			uint32_t stateTwo = 0;
			uint32_t score = 0;
			uint32_t count2 = 0;
			while(count < 250000){	//count until given time has expired. If given time is expired; assume no more bars. exit
				setLineDirection();		//use line follower
				
				if(stateTwo == 1){		//white line state. increment count 
					if(((GPIOA->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10)){	//if black is detected, toggle to black state. count++
						setLineDirection();
						stateTwo = 0;
						while(count2 < 1000){	//buffer to avoid repeating lines
							setLineDirection();
							count2 = count+1;
							count = count+1;
						}
						count2 = 0;
					}
				}
				else if(stateTwo == 0){	//black line state. 
					if(((GPIOA->IDR & GPIO_IDR_IDR10) != GPIO_IDR_IDR10)){ //if white is detected - increment count. Toggle to black state
					
						stateTwo = 1;
						score = score + 1;
						count = 0;
						while(count2 < 1000){	//buffer loop to avoid repeating bars
							setLineDirection();
							count2 = count+1;
							count = count+1;
						}
						count2 = 0;
					}
				}
				setLed(score);	//set score as it reads. Should count up from 1. 
				count = count+1;//count.
				
				
				
			}
			endState = endState + 1;
			unsetLeds();	//unset after the given time
			uint32_t adcInputMid = adcRead2();		
			uint32_t adcInputRight = adcRead3();
			uint32_t adcInputLeft = adcRead1();
			//If on the black square, stop moving. 
			if(((GPIOA->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8) && ((GPIOA->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && adcInputMid > 2700 && adcInputLeft > 2700 && adcInputRight > 2700){
				state = 1;
				setMotor('F', 1, 'F', 1);
			}
		}
		
	EXTI->PR |= EXTI_PR_PR10;
}

void SysTick_Handler(void)
{
	//GPIOA->ODR ^= GPIO_ODR_ODR5;
}

//used for stop logic given a black square
void initStop(void){
	GPIOC->CRL &= ~GPIO_CRL_CNF0_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF9_1;
}
