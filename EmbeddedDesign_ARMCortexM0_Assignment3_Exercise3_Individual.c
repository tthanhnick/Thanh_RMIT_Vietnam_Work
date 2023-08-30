//------------------------------------------- main.c CODE STARTS -------------
#include <stdio.h>
#include "NUC100Series.h"
#include <stdbool.h>
#define TIMER0_COUNTS 250000-1
volatile bool two_elapsed = FALSE;
volatile bool three_elapsed = FALSE;
volatile int counts=0;
volatile int one_cycle = 1;
//TIMER0 to blink the LED at 0.5s
void TMR0_IRQHandler(void) {
	
	one_cycle++;
	if(one_cycle==2)  // 0.25s
	{
		two_elapsed=TRUE;
	}
	else if (one_cycle==3) //0.375s
	{
		three_elapsed=TRUE;
	}
	if((counts>=2)&&(counts<3)){ //count 2 times
		if(two_elapsed==TRUE){ // if 0.25 interval
		PC->DOUT ^= 1 << 12; //LED 5 blink
		PC->DOUT |= (1<<13); //turn off led 6
		PC->DOUT |= (1<<14); //turn off led 7
		two_elapsed=FALSE;
		one_cycle=1;
		}
	}
	if ((counts>=3)&&(counts<4)){ //count 3 times
		if(three_elapsed==TRUE){ // if 0.375 interval
		PC->DOUT |= (1<<12); //turn off led 5
		PC->DOUT ^= 1 << 13; //LED 6 blink
		PC->DOUT |= (1<<14); //turn off led 7
		three_elapsed=FALSE;
		one_cycle=1;
		}
	}
	else if (counts==4){ //count 4 times
		PC->DOUT |= (1<<12); //turn off led 5
		PC->DOUT |= (1<<13); //turn off led 6
		PC->DOUT ^= 1 << 14; //LED 7 blink
		PC->DOUT ^= 1 << 15; //LED  8 blink
	}

	TIMER0->TISR |= (1 << 0); //clear timer interrupt flag
}

// Interrupt Service Rountine of GPIO port B pin 15
void EINT1_IRQHandler(void) {
	counts++;
		//start counting
	TIMER0->TCSR |= (0x01 << 30);
	CLK_SysTickDelay(100000); //debounce the button press
	PB->ISRC |= (1 << 15); //clear the external interrupt flag
}

int main(void)
{
	//System initialization start-------------------
	SYS_UnlockReg();
	CLK->PWRCON |= (1<<0); // 12 MHz CLK
	while (!(CLK->CLKSTATUS & 1<<0)); // wait until clock to be stable
	CLK->PWRCON |= (1 << 1);//32.768KHz
	while (!(CLK->CLKSTATUS & (1 << 1)));// wait until clock to be stable
	
	//LED display via from GPIO-C12 to GPIO-C15  to indicate main program execution
	PC->PMD &= (~(0x03 << 24));
	PC->PMD |= (0x01 << 24);
	PC->PMD &= (~(0x03 << 26));
	PC->PMD |= (0x01 << 26);
	PC->PMD &= (~(0x03 << 28));
	PC->PMD |= (0x01 << 28);
	PC->PMD &= (~(0x03 << 30));
	PC->PMD |= (0x01 << 30);
		
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
	TIMER0->TCSR |= (0x01 << 27);// Periodic mode
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
		
	//Timer 0 initialization end----------------
	SYS_LockReg();  // Lock protected registers
	
	//GPIO Interrupt configuration. GPIO-B15 is the interrupt source
	PB->PMD &= (~(0x03 << 30));
	PB->IMD &= (~(1 << 15));
	PB->IEN |= (1 << 15);

	//NVIC interrupt configuration for GPIO-B15 interrupt source
	NVIC->ISER[0] |= 1 << 3;
	NVIC->IP[0] &= (~(3 << 30));
	while (1) {
		
	}
}



//------------------------------------------- main.c CODE ENDS
