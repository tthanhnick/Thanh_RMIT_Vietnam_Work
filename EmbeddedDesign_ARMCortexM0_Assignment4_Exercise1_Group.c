#include <stdio.h>
#include "NUC100Series.h"

#define HXT_STATUS 1<<0
#define PLL_STATUS 1<<2

void System_Config(void);
void SPI2_Config(void);
void SPI2_TX(unsigned char temp);
void ADC7_Config(void);
void ADC_IRQHandler(void);

int main(void)
{
	System_Config();
	SPI2_Config();
	ADC7_Config();
	
	ADC->ADCR |= (1 << 11); // Start ADC channel 7 conversion

	while(1)
	{
	}
}

void System_Config(void) 
{
	SYS_UnlockReg(); // Unlock protected registers
	
	CLK->PWRCON |= (1<<0); // 12 MHz clk
	while (!(CLK->CLKSTATUS & HXT_STATUS)); // Wait the CLK to be stable
	
	// PLL configuration
	CLK->PLLCON &= ~(1<<19); // 0: PLL input is HXT
	CLK->PLLCON &= ~(1<<16); // PLL in normal mode
	CLK->PLLCON &= ~(0xFFFF<<0); // Clear all the bits
	CLK->PLLCON |= (0xC230<<0); // Set 50 MHz - NR = 3 (1+2); NF = 50 (48+2); NO = 4
	CLK->PLLCON &= ~(1<<18); // Enable PLL output
	while (!(CLK->CLKSTATUS & PLL_STATUS)); // Wait PLL to be stable
	
	CLK->CLKSEL0 &= ~(0b111<<0); // Clear all selection bits
	CLK->CLKSEL0 |= (0b010<<0); // Selection bit for PLL
	CLK->CLKDIV &= (~0x0F<<0); // CLKDIV = 1
	
	CLK->APBCLK |= (1<<14); // Enable clock for SPI2	
	
	CLK->CLKSEL1 &= ~(0x03<<2); // ADC clock source is 12 MHz
	CLK->CLKDIV &= ~(0xFF<<16); // Clear all bits
	CLK->CLKDIV |= (0x0B<<16); // ADC clock divider is (11+1) --> ADC clock is 12/12 = 1 MHz
	CLK->APBCLK |= (0x01<<28); // enable ADC clock

	SYS_LockReg();  // Lock protected registers		
}

void SPI2_Config(void)
{
	//SPI2 initialization
	GPIO_SetMode(PD,BIT0,GPIO_MODE_OUTPUT);
	GPIO_SetMode(PD,BIT1,GPIO_MODE_OUTPUT);
	GPIO_SetMode(PD,BIT3,GPIO_MODE_OUTPUT);
	SYS->GPD_MFP |= 1<<0; // 1: PD0 is configured for alternative function - SS
	SYS->GPD_MFP |= 1<<1; // 1: PD1 is configured for alternative function - CLK
	SYS->GPD_MFP |= 1<<3; // 1: PD3 is configured for alternative function - MOSI

	SPI2->CNTRL &= ~(1<<23); // 0: disable variable clock feature
	SPI2->CNTRL &= ~(1<<22); // 0: disable two bits transfer mode
	SPI2->CNTRL &= ~(1<<18); // 0: select Master mode
	SPI2->CNTRL &= ~(1<<17); // 0: disable SPI interrupt

	SPI2->CNTRL |= 1<<11; // 1: SPI clock idle high
	SPI2->CNTRL |= 1<<10; // 1: LSB is sent first
	SPI2->CNTRL &= ~(0b11<<8); // 00: one transmit/receive word will be executed in one data transfer
	SPI2->CNTRL &= ~(0b11111<<3); // Clear all bits
	SPI2->CNTRL |= (8<<3); // 8 bits/word
	SPI2->CNTRL &= ~(1<<2);  // 0: Transmit at positive edge of SPI CLK

	SPI2->DIVIDER = 24; // SPI clock divider. SPI clock = HCLK / ((DIVIDER+1)*2)
}

void SPI2_TX(unsigned char temp)
{
	SPI2->SSR |= 1<<0; // Set active state for SS
	SPI2->TX[0] = temp; // Hold data to be transmitted
	SPI2->CNTRL |= 1<<0; // 1: Start SPI transmission
	while (SPI2->CNTRL & 1<<0);
	SPI2->SSR &= ~(1<<0); // Set inactive state for SS
}

void ADC7_Config(void) 
{
	PA->PMD &= ~(0b11<<14); // Clear all bits
	PA->PMD |= (0b01<<14); // PA.7 is input pin
	PA->OFFD |= (0x01<<7); // PA.7 digital input path is disabled
	SYS->GPA_MFP |= (0x01<<7); // GPA_MFP[7] = 1 for ADC7
	SYS->ALT_MFP &= ~(0x01<<11); // ALT_MFP[11] = 0 for ADC7
	
	ADC->ADCR |= (0x03<<2); // Continuous scan mode
	ADC->ADCR |= (1<<1); // ADC interrupt is enable
	ADC->ADCR |= (0x01<<0); // ADC is enabled
	ADC->ADCHER &= ~(0x03<<8); // ADC7 input source is external pin
	ADC->ADCHER |= (0x01<<7); // ADC channel 7 is enabled.
	
	ADC->ADCMPR[0] |= (1<<0); // Enable compare function
	ADC->ADCMPR[0] |= (1<<1); // Enable compare function interrupt
	ADC->ADCMPR[0] |= (1<<2); // Select compare condition to be greater than 2V
	ADC->ADCMPR[0] &= ~(0b111<<3); // Clear all bits
	ADC->ADCMPR[0] |= (0b111<<3); // Select Channel 7
	ADC->ADCMPR[0] &= ~(0xF<<8); // Compare Match Count = 1 (0+1)
	ADC->ADCMPR[0] |= (2482<<16); // Comparison data for 2V with Vref is 3.3V and the resolution of 12-bit
	
	NVIC->ISER[0] |= (1<<29); // Enable control register
	NVIC->IP[7] &= ~(3<<14); // Set priority
}

void ADC_IRQHandler(void)
{
	if (ADC->ADSR & (1 << 1)) // greater than 2V condition
	{
		SPI2_TX('2'); 
		SPI2_TX('0'); 
		SPI2_TX('2'); 
		SPI2_TX('2'); 
		CLK_SysTickDelay(100); // Time delay between 2 packets
	}
	ADC->ADSR |= (1 << 0); // write 1 to clear ADF
}
