#include <stdio.h>
#include "NUC100Series.h"

int main(void)
{
	SYS_UnlockReg(); // Unlock protected registers
	
	CLK->PWRCON |= (1<<0); // 12 MHz CLK
	while (!(CLK->CLKSTATUS & 1<<0));
	
	CLK->PWRCON |= (1<<2); // 22 MHz CLK
	while (!(CLK->CLKSTATUS & 1<<4));
	
	CLK->CLKSEL0 &= ~(0b111<<0); // Clear CLKSEL0
	CLK->CLKSEL0 |= (0b111<<0); // Selection bits for 22 MHz CLK
	CLK->PWRCON &= ~(1<<7); // Normal mode
	CLK->CLKDIV &= ~(0x0F<<0); // Clear CLKDIV to 0
	
	CLK->CLKSEL1 &= ~(0b111<<20); // Selection bits for 12 MHz CLK
	
	CLK->APBCLK |= (1<<5); // Activate TIMER3
	
	TIMER3->TCSR &= ~(0xFF<<0); // Clear prescaler
	TIMER3->TCSR |= (0xEF<<0); // 239
	
	TIMER3->TCSR |= (1<<26); // Reset TIMER3
	
	TIMER3->TCSR &= ~(0b11<<27); // Clear bits
	TIMER3->TCSR |= (0b10<<27); // Toggle mode
	TIMER3->TCSR &= ~(1<<24); // Timer mode
	
	TIMER3->TCSR |= (1<<16); 
	
	TIMER3->TCSR |= (1<<29); // Enable interrupt bit
	
	TIMER3->TCMPR = 10; // ((1 / (2400*2)) / (1 / 50000))
	
	TIMER3->TCSR |= (1<<30); // Start TIMER3 counting
	
	SYS->GPB_MFP |= (1<<11); // Change function for GPIOB11 for TIMER3
	SYS->ALT_MFP &= ~(1<<4);
	PB->PMD &= ~(0b11<<22); // PB11
	PB->PMD |= (0b01<<22); // Output push-pull
	
	SYS_LockReg(); // Lock protected register
	
	while(1)
	{
		if (TIMER3->TISR & 1<<0)
		{
			PB->DOUT ^= 1<<11;
			TIMER3->TISR |= (1<<0);
		}
	}
}

