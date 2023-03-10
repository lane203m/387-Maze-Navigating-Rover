 /*
	ENEL 387 - Rover Project - gpio.c
	Several functions for GPIO and motor handling
	Also handles line follower logic
	
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
 #include "gpio.h"

//**************************** I/O ************************************************************


void led_IO_init (void)
{
    //Enable peripheral clocks for various ports and subsystems
    //Bit 4: Port C Bit3: Port A
    RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPAEN ;
		RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPCEN ;
		RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPBEN ;
	//****************NUCLEO-64*******************************************************************
    //Set the config and mode bits for Port A bit 5 so it will be a push-pull output (up to 50 MHz)
    //GPIOA->CRL |= GPIO_CRL_MODE5;
    //GPIOA->CRL &= ~GPIO_CRL_CNF5;
	//****************NUCLEO-64**\*****************************************************************
	// Write a 0xB ( 1011b ) into the configuration and mode bits for PA8 (AFIO)
	
	
	//push-pull output for Port A [7:10]
		GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
	
		GPIOC->CRL |= GPIO_CRL_MODE7;
    GPIOC->CRL &= ~GPIO_CRL_CNF7;
	
		GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH &= ~GPIO_CRH_CNF10;	
		
		GPIOB->CRL |= GPIO_CRL_MODE2;
    GPIOB->CRL &= ~GPIO_CRL_CNF2;
}	


//set LEDs in binary from 1-15
void setLed(uint32_t x){
	unsetLeds();
	switch(x){
		case 1:			GPIOA->BSRR = GPIO_BSRR_BR7;	
								return;
		case 2:			GPIOC->BSRR = GPIO_BSRR_BR7;
								return;
		case 3:			GPIOA->BSRR = GPIO_BSRR_BR7;	
								GPIOC->BSRR = GPIO_BSRR_BR7;
								return;
		case 4:			GPIOB->BSRR = GPIO_BSRR_BR10;
								return;
		case 5:			GPIOB->BSRR = GPIO_BSRR_BR10;
								GPIOA->BSRR = GPIO_BSRR_BR7;	
								return;
		case 6:			GPIOB->BSRR = GPIO_BSRR_BR10;
								GPIOC->BSRR = GPIO_BSRR_BR7;
								return;
		case 7:			GPIOB->BSRR = GPIO_BSRR_BR10;
								GPIOC->BSRR = GPIO_BSRR_BR7;
								GPIOA->BSRR = GPIO_BSRR_BR7;
								return;
		case 8:			GPIOB->BSRR = GPIO_BSRR_BR2;
								return;
		case 9:			GPIOB->BSRR = GPIO_BSRR_BR2;
								GPIOA->BSRR = GPIO_BSRR_BR7;	
								return;
		case 10:		GPIOB->BSRR = GPIO_BSRR_BR2;
								GPIOC->BSRR = GPIO_BSRR_BR7;
								return;
		case 11:		GPIOB->BSRR = GPIO_BSRR_BR2;
								GPIOC->BSRR = GPIO_BSRR_BR7;
								GPIOA->BSRR = GPIO_BSRR_BR7;
								return;
		case 12:		GPIOB->BSRR = GPIO_BSRR_BR2;
								GPIOB->BSRR = GPIO_BSRR_BR10;
								return;
		case 13:		GPIOB->BSRR = GPIO_BSRR_BR2;
								GPIOB->BSRR = GPIO_BSRR_BR10;
								GPIOA->BSRR = GPIO_BSRR_BR7;
								return;
		case 14:		GPIOB->BSRR = GPIO_BSRR_BR2;
								GPIOB->BSRR = GPIO_BSRR_BR10;
								GPIOC->BSRR = GPIO_BSRR_BR7;
								return;
		case 15:		GPIOB->BSRR = GPIO_BSRR_BR2;
								GPIOB->BSRR = GPIO_BSRR_BR10;
								GPIOC->BSRR = GPIO_BSRR_BR7;
								GPIOA->BSRR = GPIO_BSRR_BR7;
								return;
		default:		GPIOB->BSRR = GPIO_BSRR_BR2;
								GPIOB->BSRR = GPIO_BSRR_BR10;
								GPIOC->BSRR = GPIO_BSRR_BR7;
								GPIOA->BSRR = GPIO_BSRR_BR7;
								return;
	}
	
}

//turn off an LED based on position
void turnOffLed(uint32_t x){
	switch(x){
		case 1:			GPIOA->BSRR = GPIO_BSRR_BS7;	
								return;
		case 2:			GPIOC->BSRR = GPIO_BSRR_BS7;
								return;
		case 3:			GPIOB->BSRR = GPIO_BSRR_BS10;	
								return;
		case 4:			GPIOB->BSRR = GPIO_BSRR_BS2;
								return;
	}
}

//Turn off all LEDs
void unsetLeds(void){
	GPIOA->BSRR = GPIO_BSRR_BS7;
	GPIOC->BSRR = GPIO_BSRR_BS7; 
	GPIOB->BSRR = GPIO_BSRR_BS10; 
	GPIOB->BSRR = GPIO_BSRR_BS2;
}

//Acceptable inputs
void inputModes(void ){
	
		//GPIOA->CRL |= GPIO_CRL_MODE0;
		GPIOA->CRL &= ~GPIO_CRL_CNF0_1;
	  //GPIOA->CRL |= GPIO_CRL_MODE1;
		GPIOA->CRL &= ~GPIO_CRL_CNF1_1;
		//GPIOA->CRL |= GPIO_CRL_MODE4;
		GPIOA->CRL &= ~GPIO_CRL_CNF4_1;
		//GPIOB->CRL |= GPIO_CRL_MODE0;
		GPIOB->CRL &= ~GPIO_CRL_CNF0_1;
	
	
	
		/*GPIOA->CRH = GPIO_CRH_MODE8  | 	//Configure LEDs on A9
			GPIO_CRH_MODE9 | 	//Configure LEDs on A10
			GPIO_CRH_MODE10;
		//Configure LEDs on A11
		GPIOA->CRH = GPIO_CRL_MODE7 ;	//Configure LEDs on A12*/
}

//PWM timer3 setup 
void tim3GpioSetup(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	GPIOB->CRL |= GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0 | GPIO_CRL_CNF0_1;
	GPIOB->CRL &=~GPIO_CRL_CNF0_0;
	GPIOB->CRL |= GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1 | GPIO_CRL_CNF1_1;
	GPIOB->CRL &=~GPIO_CRL_CNF1_0;
//// Write a 0xB ( 1011b ) into the configuration and mode bits for PA8 (AFIO)
//    GPIOA->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0 ;
//    GPIOA->CRH &= ~GPIO_CRH_CNF8_0 ;

	
}

//Handling ports for motor controller 
void motorController(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	
	GPIOB->CRL &=~GPIO_CRL_CNF6;
	GPIOB->CRL &=~GPIO_CRL_MODE6;
	GPIOB->CRL |= GPIO_CRL_MODE6;
	
	GPIOB->CRL &=~GPIO_CRL_CNF7;
	GPIOB->CRL &=~GPIO_CRL_MODE7;
	GPIOB->CRL |= GPIO_CRL_MODE7;
	
	GPIOB->CRL &=~GPIO_CRL_CNF5;
	GPIOB->CRL &=~GPIO_CRL_MODE5;
	GPIOB->CRL |= GPIO_CRL_MODE5;
	
	GPIOB->CRH &=~GPIO_CRH_CNF8;
	GPIOB->CRH &=~GPIO_CRH_MODE8;
	GPIOB->CRH |= GPIO_CRH_MODE8;	
	
	GPIOB->CRH &=~GPIO_CRH_CNF9;
	GPIOB->CRH &=~GPIO_CRH_MODE9;
	GPIOB->CRH |= GPIO_CRH_MODE9;
	
	GPIOB->BSRR = GPIO_BSRR_BS6;	
}

//Set motor speed and rotation direction
void setMotor(char a, uint16_t aP, char b, uint16_t bP){
	switch(a){	//right motor. forward, back, or stop
		case 'F': 
			GPIOB->BSRR = GPIO_BSRR_BR7;	
			GPIOB->BSRR = GPIO_BSRR_BS5;	
			break;
		case 'B': 
			GPIOB->BSRR = GPIO_BSRR_BS7;
			GPIOB->BSRR = GPIO_BSRR_BR5;	
			break;
		case 'S': 
			GPIOB->BSRR = GPIO_BSRR_BR7;
			GPIOB->BSRR = GPIO_BSRR_BR5;	
			break;
	}		
	
	switch(b){	//left motor
		case 'F': 
			GPIOB->BSRR = GPIO_BSRR_BR8;	
			GPIOB->BSRR = GPIO_BSRR_BS9;	
			break;
		case 'B': 
			GPIOB->BSRR = GPIO_BSRR_BS8;
			GPIOB->BSRR = GPIO_BSRR_BR9;	
			break;
		case 'S': 
			GPIOB->BSRR = GPIO_BSRR_BR8;
			GPIOB->BSRR = GPIO_BSRR_BR9;	
			break;
	}		
	
	//sets motor speeds
	TIM3->CCR3 = aP; // 50 counts = 5 ms = 50% duty cycle
	TIM3->CCR4 = bP; // 50 counts = 5 ms = 50% duty cycle
}

//initialize PWM using timer 3 channel 3 & 4
void initializePWM(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 |= TIM_CR1_CEN; // Enable Timer3
	TIM3->EGR |= TIM_EGR_UG; // Reinitialize the counter
	TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE | TIM_CCMR2_OC4FE; 
	TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE | TIM_CCMR2_OC3FE; 
	TIM3->CCER |= TIM_CCER_CC3E; // Enable CH3
	TIM3->CCER |= TIM_CCER_CC4E; // Enable CH4
	TIM3->PSC = 0x095F; // Divide 24 MHz by 2400 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
	TIM3->ARR = 100; // 100 counts = 10 ms or 100 Hz
	TIM3->CCR3 = 1; // 50 counts = 5 ms = 50% duty cycle
	TIM3->CCR4 = 1; // 50 counts = 5 ms = 50% duty cycle
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void sw2led(void)
{
	
  // Read the USER button and control the GREEN LED pattern according to its state
			
	//Uncomment for Nucleo 64

	if ((GPIOC->IDR & GPIO_IDR_IDR13) == 0)
		
    {
       GPIOA->BSRR = GPIO_BSRR_BS5;
    }
   else
    {
        GPIOA->BSRR = GPIO_BSRR_BR5;
    }
}

void ledOff(void){

	GPIOA->BSRR = GPIO_BSRR_BR5;

}

uint32_t buttonPress(void)
{
	uint32_t test = GPIOA->IDR & GPIO_IDR_IDR4;
	
	if((GPIOA->IDR & GPIO_IDR_IDR0) == 0){
		setLed(1);
	}
	if((GPIOA->IDR & GPIO_IDR_IDR1) == 0){
		setLed(2);
	}
	if((GPIOA->IDR & GPIO_IDR_IDR4) == 0){
		setLed(3);
	}
	if((GPIOB->IDR & GPIO_IDR_IDR0) == 0){
		setLed(4);
	}
	test = test+1-1;
	test = test-1+1;
	return test;
}

//Init ports for ADC 
void adcInit(void){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL  &= ~GPIO_CRL_CNF0;
	GPIOA->CRL  &= ~GPIO_CRL_CNF1;
	GPIOA->CRL  &= ~GPIO_CRL_CNF4;
	ADC1->CR2 |= ADC_CR2_ADON;
}

//Read adc value - left line sensor
uint32_t adcRead1(void){
	ADC1->SQR3 = ADC_SQR3_SQ1_2;
	ADC1->CR2 = ADC_CR2_ADON;
	
	while((ADC1->SR & ADC_SR_EOC) != ADC_SR_EOC)    
    {

    }
    return (ADC1->DR);
	
}

//Read adc value - middle line sensor
uint32_t adcRead2(void){
	ADC1->SQR3 = ADC_SQR3_SQ1_1;
	ADC1->CR2 = ADC_CR2_ADON;
	
	while((ADC1->SR & ADC_SR_EOC) != ADC_SR_EOC)    
    {

    }
    return (ADC1->DR);
	
}

//Read adc value - right line sensor
uint32_t adcRead3(void){
	ADC1->SQR3 = ADC_SQR3_SQ1_0;
	ADC1->CR2 = ADC_CR2_ADON;
	
	while((ADC1->SR & ADC_SR_EOC) != ADC_SR_EOC)    
    {

    }
    return (ADC1->DR);
	
}

//line follower - but in reverse
void setLineDirectionBack(void){
		uint32_t adcInputMid = adcRead2();		
		uint32_t adcInputRight = adcRead3();
		uint32_t adcInputLeft = adcRead1();
		if(adcInputMid > 2925){
			setMotor('B', 35, 'B', 35);
			//setLed(8);
		}else if(adcInputLeft > 2925 && adcInputRight > 2925){
			setMotor('F',35, 'B', 55);
		}else if(adcInputLeft > 2925){
			//setLed(1);
			setMotor('B', 30, 'F', 30);		
		}else if(adcInputRight > 2925){
			//setLed(4);
			setMotor('F', 35, 'B', 35);
		}else{
			setMotor('B', 35, 'B', 35);
			//setLed(8);
		}	
}

//line follower - forward
void setLineDirection(void){
		//IR line sensor values
		uint32_t adcInputMid = adcRead2();		
		uint32_t adcInputRight = adcRead3();
		uint32_t adcInputLeft = adcRead1();
	
		//forward if mid is black
		if(adcInputMid > 2920 /*&& adcInputLeft > 2930 && adcInputRight > 2930*/){
			setMotor('F', 45, 'F', 45);
			//setLed(8);
		}
		else if(adcInputLeft > 2925 && adcInputRight > 2925){
			setMotor('B',45, 'F', 25);
		}
		//forward if mid is black
		else if(adcInputMid > 2925){
			setMotor('F', 45, 'F', 45);
		}
		//left if left is black & mid is not
		else if(adcInputLeft > 2925){
			//setLed(1);
			setMotor('F', 35, 'B', 35);
		}
		//right if right is black & mid is not
		else if(adcInputRight > 2925){
			//setLed(4);
			setMotor('B', 40, 'F', 40);
		}
		//otherwise, forward
		else{
			setMotor('F', 45, 'F', 45);
			//setLed(8);
		}
		
		//stop if on the black square
		if(((GPIOA->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8) && ((GPIOA->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && adcInputMid > 2700 && adcInputLeft > 2700 && adcInputRight > 2700){
			setMotor('F', 1, 'F', 1);
		}


}

