#include <stdio.h>
#include "NUC100Series.h"

#define HXT_STATUS 1<<0 //12 MHz
#define PLL_STATUS 1<<2

int main(void)
{
	SYS_UnlockReg(); //Unlock protected register
	
	//Enable clock sources and wait until stable
	CLK->PWRCON |= 1<<0;
	while(!(CLK->CLKSTATUS & HXT_STATUS)); //Issue 1
	
	//PLL configuration starts (from slide from 12 generate 36)
	CLK->PLLCON &= (~(1<<19)); //0: PLL input is HTX 12 MHz (default) 1: PLL input is HIRC 22MHz
	CLK->PLLCON &= (~(1<<16)); //0: PLL in normal mode. 1:PLL in power down mode (default)
	CLK->PLLCON &= (~(0xFFFF<<0)); // Reset all value [15:0]
	CLK->PLLCON |=0xC222;//PLLCON[15:0] get 36 MHz //Issue 2
	CLK->PLLCON &=(~(1<<18)); //0:enable PLL clock 1: Disable PLL clock
	while(!(CLK->CLKSTATUS & PLL_STATUS)); //wait until PLLOUT is fully generated //Issue 1
	//PLL configuration ends
	
	//CPU clock source selection configuration starts
	CLK->CLKSEL0 &= (~(0b111<<0)); //0x07<<0 clear PLLOUT first //Issue 3
	CLK->CLKSEL0 |=0b010;//Select PULLOUT for CPU
	CLK->CLKDIV &= (~(0x0F<<0)); 
	//CPU clock source selection configuration ends
	
	SYS_LockReg(); //Lock protected register
	
	
	while(1){
		
	}
}
