//------------------------------------------- main.c CODE STARTS -------------
#include <stdio.h>
#include "NUC100Series.h"
#include <stdbool.h>

volatile int counts=0;

void TMR0_IRQHandler(void) {
	if(counts %2 ==0){ //count odd
		TIMER0->TISR &= ~ (1 << 0); //clear timer interrupt flag
		TIMER0->TCMPR = 250000-1; // 0.125s
		PC->DOUT ^= 1 << 12; //LED 5 blink
		TIMER0->TISR |= (1 << 0); //set timer interrupt flag
	}
	if (counts %2 == 1){ //count even
		TIMER0->TISR &= ~ (1 << 0); //clear timer interrupt flag
		TIMER0->TCMPR = 750000-1;//0.375s
		PC->DOUT |= (1<<12); //turn off led 5
		TIMER0->TISR |= (1 << 0); //set timer interrupt flag
	}
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
	TIMER0->TCSR |= (0b11 << 27);	// Contious mode
	TIMER0->TCSR &= ~(0x01 << 24);// Timer mode
	
	
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
