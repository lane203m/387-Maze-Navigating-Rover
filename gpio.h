 /*
	ENEL 387 - Rover Project - gpio.h
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

#include <stdint.h>

void led_IO_init (void);
void sw2led(void);
void ledOff(void);
void setLed(uint32_t x);
void unsetLeds(void);
uint32_t buttonPress(void);
void inputModes(void);
void adcInit(void);
uint32_t adcRead1(void);
uint32_t adcRead2(void);
uint32_t adcRead3(void);
void turnOffLed(uint32_t);
void initializePWM(void);
void tim3GpioSetup(void);
void motorController(void);
void setLineDirection(void);
void setLineDirectionBack(void);
void setMotor(char a, uint16_t aP, char b, uint16_t bP);
#define NUC_GREEN_ON	0X00000020
#define NUC_GREEN_OFF	0x00200000 

