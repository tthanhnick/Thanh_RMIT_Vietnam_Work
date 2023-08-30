//------------------------------------------- main.c CODE STARTS -------------
#include <stdio.h>
#include "NUC100Series.h"
#include <stdbool.h>
#define TIMER0_COUNTS 250000-1

//TIMER0 to blink the LED at 0.5s
void TMR0_IRQHandler(void) {

	PC->DOUT ^= 1 << 12; //LED 5 blink
	TIMER0->TISR |= (1 << 0); //clear timer interrupt flag
}

int main(void)
{
	//System initialization start-------------------
	SYS_UnlockReg();
	CLK->PWRCON |= (1<<0); // 12 MHz CLK
	while (!(CLK->CLKSTATUS & 1<<0)); // wait until clock to be stable
	CLK->PWRCON |= (1 << 1);//32.768KHz
	while (!(CLK->CLKSTATUS & (1 << 1)));// wait until clock to be stable
	
	//LED display via  GPIO-C12 to indicate main program execution
	PC->PMD &= (~(0x03 << 24));
	PC->PMD |= (0x01 << 24);
			
	//Timer 0 initialization start--------------
	//TM0 Clock selection and configuration
	CLK->CLKSEL1 &= ~(0b111 << 8); //reset  timer0 mode and choose 12 MHz
	CLK->APBCLK |= (1<< 2); //enable Timer 0
		
	//Set the prescaler to 2
	TIMER0->TCSR &= ~(0xFF << 0);// Clear prescaler for TIMER0
	TIMER0->TCSR |= 2 << 0;
	
	// Reset Timer 0
	TIMER0->TCSR |= (0x01 << 26);
		
	//define Timer 0 operation mode
	TIMER0->TCSR &= ~(0x03 << 27); // Clear bits
	TIMER0->TCSR |= (0b11 << 27);// Contious mode
	TIMER0->TCSR &= ~(0x01 << 24);// Timer mode
	
	//TimeOut = 0.5s --> Counter's TCMPR = 0.5s / (1/(32.7685 KHz) = 16384
	TIMER0->TCMPR = TIMER0_COUNTS;
	//Enable interrupt in the Control Resigter of Timer istsle
	TIMER0->TCSR |= (1 << 29);
	//Set Timer0 in NVIC Set-Enable Control Register (NVIC_ISER)
	NVIC->ISER[0] |= 1 << 8;
	
	
	//Priority
	//Set level 0 for Timer 0 
	NVIC->IP[2] &= (~(3 << 6));
	//start counting
	TIMER0->TCSR |= (0x01 << 30);	
	//Timer 0 initialization end----------------
	SYS_LockReg();  // Lock protected registers
	
		while (1) {
		
	}
}



//------------------------------------------- main.c CODE ENDS
