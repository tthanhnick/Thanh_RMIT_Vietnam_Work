//------------------------------------------- main.c CODE STARTS -------------
#include <stdio.h>
#include "NUC100Series.h"

#define HXT_STATUS 1<<0
#define PLL_STATUS 1<<2
#define TIMER1_COUNTS 6000000-1

int main(void)
{
	//System initialization start-------------------
	SYS_UnlockReg();     
	CLK->PWRCON |= (1 << 0);     
	while(!(CLK->CLKSTATUS & HXT_STATUS));          
	
	// Configure PLL = 40 MHz
	CLK->PLLCON &= ~(1 << 19); 	// 0: PLL source clock from external 4-24MHz
	CLK->PLLCON &= ~(1 << 18);	// 0: enable PLL clock out
	CLK->PLLCON &= ~(1 << 16);	// 0: PLL in normal mode 
	CLK->PLLCON &= ~(0x01FF << 0);	// Set FB_DV_Mask to 0 
	// Frequency = 1Mhz * (FB_DV_Value + 2)
	CLK->PLLCON |= 10;
	while (!(CLK->CLKSTATUS & PLL_STATUS));
	
	// SELECT CPU clock = PLL
	CLK -> CLKSEL0 &= ~(0b111 << 0);
	CLK -> CLKSEL0 |= 0b010 << 0;
	  
	CLK->CLKDIV &= ~(0x0F);
	CLK->CLKDIV |= 1;

	//System initialization end---------------------
	//GPIO initialization start --------------------
	//GPIOB.9: output push-pull
    PB->PMD &= (~(0b11<< 18));		
    PB->PMD |= (0b01 << 18);    	
	//GPIO initialization end ----------------------
	
	//Configure TM1 in Toggle mode
	//Enable Alternative function
	SYS->GPB_MFP |= (1<<9);
	SYS->ALT_MFP &= ~(1<<1);
	
	//Timer 1 initialization start--------------
	//TM1 Clock selection and configuration
	CLK->CLKSEL1 &= ~(0b111 << 12);
	CLK->CLKSEL1 |= (0b010 << 12);
	CLK->APBCLK |= (1 << 3);

	//Pre-scale =1
	TIMER1->TCSR &= ~(0xFF << 0);
	TIMER1->TCSR |= 1 << 0;

	//reset Timer 1
	TIMER1->TCSR |= (1 << 26);
	
	//define Timer 1 operation mode
	TIMER1->TCSR &= ~(0b11 << 27);
	TIMER1->TCSR |= (0b10 << 27); //Toogle mode
	TIMER1->TCSR &= ~(1 << 24);
	
	//TDR to be updated continuously while timer counter is counting
	TIMER1->TCSR |= (1 << 16);
	
	//Enable TE bit (bit 29) of TCSR
	//The bit will enable the timer interrupt flag TIF
	TIMER1->TCSR |= (1 << 29);
	
	//TimeOut = 0.8s --> Counter's TCMPR = 800000-1
	TIMER1->TCMPR = TIMER1_COUNTS;
	
	//start counting
	TIMER1->TCSR |= (1 << 30);
	//Timer 0 initialization end----------------
	
	SYS_LockReg();  // Lock protected registers

	while (1) {
		
	}
}
//------------------------------------------- main.c CODE ENDS 
