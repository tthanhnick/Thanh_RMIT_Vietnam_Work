//------------------------------------------- main.c CODE STARTS -------------
#include <stdio.h>
#include "NUC100Series.h"

volatile int counts =0;

void SysTick_Handler (void) // Interrupt Service Routine of SysTick
{
	PC->DOUT ^= (1 << 12);
	counts++;
	if (counts == 16*2) {
		PC->DOUT |= (1 << 12);
		counts=0;
	} 
}
void EINT1_IRQHandler(void){
	
		//Start SysTick
		SysTick->CTRL |= (1 << 0);
    PB->ISRC |= (1 << 15);
}	
int main(void)
{
		//System initialization start------------
	SYS_UnlockReg(); //Unlock protected regs
	CLK->PWRCON |= (1 << 0);
	while (!(CLK->CLKSTATUS & (1 << 0)));

	//Select CPU clock
	//12 MHz HXT
	CLK->CLKSEL0 &= ~(0b111 << 0);
  CLK->PWRCON &= ~(1<<7);// Normal mode

	//Clock frequency divider
	CLK->CLKDIV &= ~(0xF<<0);
	//System initialization end----------
	
	//GPIO initialization start --------
	PC->PMD &= ~(0b11 << 24);
	PC->PMD |= 0b01 << 24;
	// GPB.15 Input
	PB->PMD &= ~(0b11 << 30);
	PB->IMD &= (~(1 << 15));
	PB->IEN |= (1 << 15);
	//GPIO initialization end ------
	
//System Tick initialization start--------------
	//STCLK = HXT/2 = 6 MHz
	CLK->CLKSEL0 &= ~(0b111 << 3);
	CLK->CLKSEL0 |= (0b010 << 3);
	
		//STCLK as SysTick clock source
	SysTick->CTRL &= ~(1 << 2);

	//SysTick Reload Value
	SysTick->LOAD = 3000000-1; 
	//Initial SysTick count value
	SysTick->VAL = 0;
	//Start SysTick
	SysTick->CTRL |= (1 << 0);
	//System Tick initialization end-------
	
	//NVIC interrupt configuration for GPIO-B15 interrupt source
	NVIC->ISER[0] |= 1<<3;
	NVIC->IP[0] &= (~(0b11<<30));
	
	SYS_LockReg();  // Lock protected registers
	
		while (1) {

	}
}
//------------------------------------------- main.c CODE ENDS 
