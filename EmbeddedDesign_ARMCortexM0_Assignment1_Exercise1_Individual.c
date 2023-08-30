#include <stdio.h>
#include "NUC100Series.h"

#define NUC140_CLOCK 1

#define PLL_STATUS 1 << 2 	 	// PLLCON
#define HIRC_STATUS 1 << 4 	// 22.11 MHz


int main(void) {
	SYS_UnlockReg(); // Unlock protected registers
	// Enable and wait until HIRC 22.11 MHz clock source is stable
	CLK->PWRCON |= 1 << 2;
	while (!(CLK->CLKSTATUS & HIRC_STATUS));
	//PLL configuration starts 
	CLK->PLLCON |= ((1<<19)); // 1: PLL input is HIRC 22MHz
	CLK->PLLCON &= (~(1<<16)); // 0: PLL in normal mode. 1: PLL in power-down mode (default)
	CLK->PLLCON &= (~(0xFFFF << 0));
	CLK->PLLCON |= (0x521F<<0);
	CLK->PLLCON &= (~(1<<18)); // 0: enable PLL clock out. 1: disable PLL clock (default)
	while(!(CLK->CLKSTATUS & PLL_STATUS)); // wait until PLLOUT is fully generated and enabled before selecting it for CPU/etc.
	//PLL configuration ends
	CLK->CLKSEL0 &= (~(0b111<<0));
	CLK->CLKSEL0 |= 0b010; // select PLLOUT for CPU
  CLK->PWRCON &= ~(1<<7);// Normal mode
	CLK->CLKDIV &= (~(0xF<<0)); // ensure that PLLOUT is not further divided
	//CPU clock source selection configuration ends
 
  SYS_LockReg();  // Lock protected registers
	//Display number 5 on U11 7segment LED
	//Set mode for PC4 to PC7 
  PC->PMD &= (~(0xFF<< 8));		//clear PMD[15:8] 
  PC->PMD |= (0b01010101 << 8);    	//Set output push-pull for PC4 to PC7
	
	//Set mode for PE0 to PE7
	PE->PMD &= (~(0xFFFF<< 0));		//clear PMD[15:0] 
	PE->PMD |= 0b0101010101010101<<0;   //Set output push-pull for PE0 to PE7
	
	//pin mode configuration input
	PB->PMD &= (~(0b11<<24)); //0x03<<24 Set GPB15 as input
	 while(1){
		 if(!(PB->PIN & (1<<15))) //Check if button pressed, GPB15 then LOW(pull-up)
		 {
		//Turn on U12, while turn off other 7segments
			PC->DOUT &= ~(1<<7);     //Logic 1 to turn on the digit U11
			PC->DOUT |= (1<<6);		   //U12
			PC->DOUT &= ~(1<<5);		//U13
			PC->DOUT &= ~(1<<4);		//U14
			 
			//Turn on segment for display number 5
			//Common anode, Logic 0 to turn on the segment
			PE->DOUT &= ~(1<<7);		//segment g
			//PE->DOUT &= ~(1<<6);	//segment e
			PE->DOUT &= ~(1<<5);		//segment d
			//PE->DOUT &= ~(1<<4);	//segment b
			PE->DOUT &= ~(1<<3);		//segment a
			PE->DOUT &= ~(1<<2);		//segment f
			//PE->DOUT &= ~(1<<1);	//DOT
			PE->DOUT &= ~(1<<0);		// segment c		
		 }
		 else
			{
		  //Turn on U12, while turn off other 7segments
			PC->DOUT &= ~(1<<7);     //Logic 1 to turn on the digit U11
			PC->DOUT |= (1<<6);		   //U12
			PC->DOUT &= ~(1<<5);		//U13
			PC->DOUT &= ~(1<<4);		//U14
			 
			//Turn on segment for display number 3
			//Common anode, Logic 0 to turn on the segment
			PE->DOUT &= ~(1<<7);		//segment g
			//PE->DOUT &= ~(1<<6);	//segment e
			PE->DOUT &= ~(1<<5);		//segment d
			PE->DOUT &= ~(1<<4);	//segment b
			PE->DOUT &= ~(1<<3);		//segment a
			//PE->DOUT &= ~(1<<2);		//segment f
			//PE->DOUT &= ~(1<<1);	//DOT
			PE->DOUT &= ~(1<<0);		// segment c		
        }
			}
}
