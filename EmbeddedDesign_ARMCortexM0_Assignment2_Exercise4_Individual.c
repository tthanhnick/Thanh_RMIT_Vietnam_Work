#include <stdio.h>
#include "NUC100Series.h"
#include <stdbool.h>

volatile int one_second = 1;
volatile int a_second = 1;
volatile bool two_elapsed = TRUE;
volatile bool three_elapsed = TRUE;
volatile bool sensor1 = FALSE;
volatile bool sensor2 = FALSE;  

// Normal West-East direction
enum WE_state 
{
	WE
};
enum WE_state WE_state;

// South direction
enum S_state 
{
	S
};
enum S_state S_state;

// West-South direction
enum WS_state 
{
	WS
};
enum WS_state WS_state;

int digit[] = 
{
	0b11101110, // Number 1 on 7-segment 
	0b00000111, // Number 2 on 7-segment 
	0b01000110 // Number 3 on 7-segment 
};

void sensor (void) // Sensor detect function
{
	if (!(PA->PIN & 1<<2)) // Sensor for West to South direction
	{
		sensor1 = TRUE;
		two_elapsed = TRUE;
	}
	if (!(PA->PIN & 1<<0)) // Sensor for South direction
	{
		sensor2 = TRUE;
		two_elapsed = TRUE;
	}	
}

void WS_7seg (void) // West-South direction 7-segment display
{
	PA->DOUT &= ~(1<<3);
	PA->DOUT |= (1<<4);
	PA->DOUT |= (1<<5);
			
	PC->DOUT &= ~(1<<4);
	PC->DOUT &= ~(1<<5);
	PC->DOUT |= (1<<6);
	PC->DOUT &= ~(1<<7);

	if (a_second == 1)
	{
		PE->DOUT = digit[1];
	}
	if (a_second == 2)
	{
		PE->DOUT = digit[0];
	}
}

void S_7seg (void) // South direction 7-segment display
{
	PA->DOUT |= (1<<3);
	PA->DOUT |= (1<<4);
	PA->DOUT &= ~(1<<5);
			
	PC->DOUT &= ~(1<<4);
	PC->DOUT |= (1<<5);
	PC->DOUT &= ~(1<<6);
	PC->DOUT &= ~(1<<7);

	if (a_second == 1)
	{
		PE->DOUT = digit[1];
	}
	if (a_second == 2)
	{
		PE->DOUT = digit[0];
	}
}

void WE_7seg (void) // West-East direction 7-segment display
{
	PC->DOUT &= ~(1<<4);
	PC->DOUT &= ~(1<<5);
	PC->DOUT &= ~(1<<6);
	PC->DOUT |= (1<<7);
					
	if (one_second == 1)
	{
		PE->DOUT = digit[2];
	}
	if (one_second == 2)
	{
		PE->DOUT = digit[1];
	}
	if (one_second == 3)
	{
		PE->DOUT = digit[0];
	}
}

void SysTick_Handler (void) // Interrupt Service Routine of SysTick
{
	// Two seconds elapsed for West-South direction and South direction
	if ((sensor1 || sensor2) && three_elapsed == FALSE) 
	{
		if (a_second == 2)
		{
			sensor1 = FALSE;
			sensor2 = FALSE;
			two_elapsed = FALSE;
			three_elapsed = TRUE;
			a_second = 1;
		}
		else
		{
			a_second++;
			two_elapsed = TRUE;
		}
	}

	// Three seconds elapsed for West-East direction
	else 
	{
		if (one_second == 3)
		{
			three_elapsed = FALSE;
			one_second = 1;
		}
		else
		{
			one_second++;
			three_elapsed = TRUE;
		}
	}
}

int main(void)
{
	SYS_UnlockReg(); // Unlock protected registers
	
	CLK->PWRCON |= (1<<1); // 32 kHz CLK
	while(!(CLK->CLKSTATUS & 1<<1));
	
	CLK->CLKSEL0 &= ~(0b111<<0); // Clear CLKSEL0
	CLK->CLKSEL0 |= (0b001<<0); // Selection bits for 32 kHz CLK
	CLK->PWRCON &= ~(1<<7); // Normal mode
	CLK->CLKDIV &= ~(0x0F<<0); // Clear CLKDIV to 0
	
	CLK->CLKSEL0 &= ~(0b111<<3); // Clear CLKSEL0
	CLK->CLKSEL0 |= (0b001<<3); // Selection bits for 32 kHz CLK
	SysTick->CTRL &= ~(1<<2); // Select CLK source
	
	SysTick->LOAD = 32767; // Load (reload) value
	SysTick->VAL = 0; // Inital value
	SysTick ->CTRL |= (1<<1); // Enable interrupt
	SysTick->CTRL |= (1<<0); // Enable SysTick
	
	PC->PMD &= ~(0b11<<24); // PC12
	PC->PMD |= (0b01<<24); // Output push-pull
	PC->PMD &= ~(0b11<<26); // PC13
	PC->PMD |= (0b01<<26); // Output push-pull
	PC->PMD &= ~(0b11<<28); // PC14
	PC->PMD |= (0b01<<28); // Output push-pull
	PC->PMD &= ~(0b11<<30); // PC15
	PC->PMD |= (0b01<<30); // Output push-pull
	
	// Set mode for PC4 to PC7 
	PC->PMD &= ~(0xFF<<8); // clear PMD[15:8] 
	PC->PMD |= (0b01010101<<8); // Set output push-pull for PC4 to PC7
	
	//Set mode for PE0 to PE7
	PE->PMD &= ~(0xFFFF<<0); // clear PMD[15:0] 
	PE->PMD |= (0b0101010101010101<<0); // Set output push-pull for PE0 to PE7
	
	//Set mode for PA
	PA->PMD &= ~(0xFFF<<0); // clear PMD[11:0] 
	PA->PMD |= (0b01010101010101<<0); // Set output push-pull for PA0 to PA5
	
	SYS_LockReg(); // Lock protected register
	
	while(1)
	{
		PA->DOUT &= ~(1<<3);
		PA->DOUT &= ~(1<<4);
		PA->DOUT &= ~(1<<5);
		
		sensor();

		if (sensor1 && three_elapsed == FALSE) // West-South direction condition
		{
			WS_7seg();
			
			if (two_elapsed) 
			{
				switch (WS_state)
				{
					case WS:
						PC->DOUT |= (1<<12);
						PC->DOUT |= (1<<13);
						PC->DOUT &= ~(1<<14);
						PC->DOUT |= (1<<15);
						WE_state = WE;
						break;
					
					default:
						PC->DOUT |= (1<<12);
						PC->DOUT |= (1<<13);
						PC->DOUT |= (1<<14);
						PC->DOUT |= (1<<15);
						WS_state = WS;
						break;
				}
			}
		}
		
		else if (sensor2 && three_elapsed == FALSE) // South direction condition
		{
			S_7seg();
			
			if (two_elapsed) 
			{
				switch (S_state)
				{
					case S:
						PC->DOUT |= (1<<12);
						PC->DOUT |= (1<<13);
						PC->DOUT |= (1<<14);
						PC->DOUT &= ~(1<<15);				
						WE_state = WE;
						break;
					
					default:
						PC->DOUT |= (1<<12);
						PC->DOUT |= (1<<13);
						PC->DOUT |= (1<<14);
						PC->DOUT |= (1<<15);
						S_state = S;
						break;
				}
			}	
		}
		
		else // West-East direction condition
		{
			WE_7seg();
				
			if (three_elapsed) 
			{
				switch (WE_state)
				{
					case WE:
						PC->DOUT &= ~(1<<12);
						PC->DOUT &= ~(1<<13);
						PC->DOUT |= (1<<14);
						PC->DOUT |= (1<<15);
						WE_state = WE;
						break;
					
					default:				
						PC->DOUT |= (1<<12);
						PC->DOUT |= (1<<13);
						PC->DOUT |= (1<<14);
						PC->DOUT |= (1<<15);
						WE_state = WE;
						break;
				}
			}
		}
	}
}











