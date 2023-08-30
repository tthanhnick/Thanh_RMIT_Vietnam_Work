#include <stdio.h>
#include "NUC100Series.h"

int main(void)
{
	SYS_UnlockReg(); // Unlock protected registers
	
	CLK->PWRCON |= (1<<1); // 32 kHz CLK
	while(!(CLK->CLKSTATUS & 1<<1));
	
	CLK->CLKSEL0 &= ~(0b111<<0); // Clear CLKSEL0
	CLK->CLKSEL0 |= (0b001<<0); // Selection bits for 32 kHz CLK
	CLK->PWRCON &= ~(1<<7); // Normal mode
	CLK->CLKDIV &= ~(0xF<<0); // Clear CLKDIV to 0
	
	CLK->CLKSEL0 &= ~(0b111<<3); // Clear CLKSEL0
	CLK->CLKSEL0 |= (0b001<<3); // Selection bits for 32 kHz CLK
	SysTick->CTRL &= ~(1<<2); // Select CLK source
	
	SysTick->LOAD = 32767; // Load (reload) value
	SysTick->VAL = 0; // Inital value
	SysTick ->CTRL |= (1<<1); // Enable interrupt
	
	SysTick->CTRL |= (1<<0); // Enable SysTick
	
	PC->PMD &= ~(0b11<<24); // PC12
	PC->PMD |= (0b01<<24); // Output push-pull
	
	SYS_LockReg(); // Lock protected register
	
	while(1)
	{
	}
}

void SysTick_Handler (void) // Interrupt Service Routine of SysTick
{
	PC->DOUT ^= 1<<12;
}
