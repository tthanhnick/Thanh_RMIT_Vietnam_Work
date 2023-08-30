//------------------------------------------- main.c CODE STARTS -------------
#include <stdio.h>
#include "NUC100Series.h"

#define TIMER0_COUNTS 16384-1
#define TIMER3_COUNTS 10
volatile int counts=0;

//TIMER0 to blink the LED at 0.5s
void TMR0_IRQHandler(void) {
	if((counts>=3)&&(counts<8)){ //count 3 times
		PC->DOUT ^= 1 << 12; //LED blink
	}
	else if (counts==8){ //count 8 times
		PC->DOUT |= (1<<12); //turn off led and buzzer
	}
	TIMER0->TISR |= (1 << 0); //clear timer interrupt flag
}

//TIMER3 to blink the buzzer at 2.5KHz
void TMR3_IRQHandler(void) {
	if ((counts>=5)&&(counts<8)){ //count 5 times 
		PB->DOUT ^= (1 << 11); //buzzer beep at 2.5KHz
	}
	else if (counts==8){ //count 8 times
		PB->DOUT |= (1 << 11); //turn off led and buzzer
	}
	TIMER3->TISR |= (1 << 0); //clear timer interrupt flag
}
 
int main(void)
{
	//System initialization start-------------------
	SYS_UnlockReg();
	CLK->PWRCON |= (1<<0); // 12 MHz CLK
	while (!(CLK->CLKSTATUS & 1<<0)); // wait until clock to be stable
	CLK->PWRCON |= (1 << 1);//32.768KHz
	while (!(CLK->CLKSTATUS & (1 << 1)));// wait until clock to be stable
	
	//LED display via GPIO-C12 to indicate main program execution
	PC->PMD &= (~(0x03 << 24));
	PC->PMD |= (0x01 << 24);
	//BUZZER to indicate interrupt handling routine
	PB->PMD &= (~(0x03 << 22));
	PB->PMD |= (0x01 << 22);
	
	//Timer 0 and 3 initialization start--------------
	//TM0 Clock selection and configuration
	CLK->CLKSEL1 &= ~(0b111 << 8); //reset  timer0 mode
	CLK->CLKSEL1 |= (0x01 << 8); //choose timer 0 source from 32.768KHz
	CLK->APBCLK |= (1<< 2); //enable Timer 0
	
	//TM3 Clock selection and configuration
	CLK->CLKSEL1 &= ~(0b111 << 20); //reset and choose timer3 source from 12MHz
	CLK->APBCLK |= (1<<5); // enable TIMER3
	
	//Set the prescaler
	TIMER0->TCSR &= ~(0xFF << 0);// Clear prescaler for TIMER0
	TIMER3->TCSR &= ~(0xFF<<0); // Clear prescaler for TIMER3
	TIMER3->TCSR |= (239<<0); //Set the prescaler to 240
	
	// Reset Timer 0
	TIMER0->TCSR |= (0x01 << 26);
	// Reset TIMER 3
	TIMER3->TCSR |= (1<<26); 
	
	//define Timer 0 operation mode
	TIMER0->TCSR &= ~(0x03 << 27); // Clear bits
	TIMER0->TCSR |= (0x01 << 27);// Periodic mode
	TIMER0->TCSR &= ~(0x01 << 24);// Timer mode
	
	//define Timer 3 operation mode
	TIMER3->TCSR &= ~(0b11<<27); // Clear bits
	TIMER3->TCSR |= (0b10<<27); // Periodic mode
	TIMER3->TCSR &= ~(1<<24); // Timer mode
		
	//TimeOut = 0.5s --> Counter's TCMPR = 0.5s / (1/(32.7685 KHz) = 16384
	TIMER0->TCMPR = TIMER0_COUNTS;
	//Enable interrupt in the Control Resigter of Timer istsle
	TIMER0->TCSR |= (1 << 29);
	//Set Timer0 in NVIC Set-Enable Control Register (NVIC_ISER)
	NVIC->ISER[0] |= 1 << 8;
	
	//TimeOut = 2.5KHz --> Counter's TCMPR = 1/(2*2500) / (1*240/(12 MHz) = 10
	TIMER3->TCMPR = TIMER3_COUNTS;
	//Enable interrupt in the Control Resigter of Timer istsle
	TIMER3->TCSR |= (1 << 29);
	//Set Timer3 in NVIC Set-Enable Control Register (NVIC_ISER)
	NVIC->ISER[0] |= 1 << 11;
		
	//Priority
	//Set level 0 for Timer 0 
	NVIC->IP[2] &= (~(3 << 6));
	//Set level 3 for TIMER 3
	NVIC->IP[2] |= (0b11 << 30);
	
	//start counting
	TIMER0->TCSR |= (0x01 << 30);
	TIMER3->TCSR |= (0x01 << 30);
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

// Interrupt Service Rountine of GPIO port B pin 15
void EINT1_IRQHandler(void) {
	counts++; //count the button press's time
	CLK_SysTickDelay(100000); //debounce the button press
	PB->ISRC |= (1 << 15); //clear the external interrupt flag
}

//------------------------------------------- main.c CODE ENDS
