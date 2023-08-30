//------------------------------------------- main.c CODE STARTS ---------------------------------------------------------------------------
#include <stdio.h>
#include "NUC100Series.h"
#include <string.h>
#include "MCU_init.h"
#include "SYS_init.h" 
#include "LCD.h"
#include <stdbool.h>

void System_Config(void);
void UART0_Config(void);
void UART0_SendChar(int ch);
char UART0_GetChar();
void SPI3_Config(void);

void LCD_start(void);
void LCD_command(unsigned char temp);
void LCD_data(unsigned char temp);
void LCD_clear(void);
void LCD_SetAddress(uint8_t PageAddr, uint8_t ColumnAddr);

volatile char data ;
volatile bool new_data = FALSE;

// Char function to receive data from 1 packet
char ReceiveByte(void)
{
	return (char) UART0->DATA;
}

//UART interrupt to receive data
void UART02_IRQHandler(void){
	data = ReceiveByte();
	new_data=TRUE;
}

int main(void)
{
	int i;
	char simulated_flight_data[129]=""; //129 char total character /packet ==> 130 p/s 129 still ok ?
	char temp[40]=""; //empty variable used to del data after use
	System_Config();
	UART0_Config();
	
	//--------------------------------
	//SPI3 initialization
	//--------------------------------
	SPI3_Config();

	//--------------------------------
	//LCD initialization
	//--------------------------------
	LCD_start();
	LCD_clear();

	//--------------------------------
	//LCD static content
	//--------------------------------


	//--------------------------------
	//LCD dynamic content
	//--------------------------------
	while(1){
		if(new_data==TRUE) //if there are new data
				{
					simulated_flight_data[i]=data;
					i++;
					if(data=='\r'){	//to the carriage return cursor to the current line					
					LCD_clear();
					printS_5x7(0,0,"Latitude"); 
					printS_5x7(0,10,"Longtitude");
					for(int j=21;j<29;j++){ //longtitude print
							printC_5x7((j-21)*5+45,0,simulated_flight_data[j]);  // Print longtitude value from the packet
					}
					for(int j=30;j<39;j++){ //latitude print
						printC_5x7((j-30)*5+55,10,simulated_flight_data[j]);  // Print latitude value from the packet
					}
					strcpy(simulated_flight_data,temp); // use string copy to assign emty string to data string => delete string
					i=0;		
					}
				new_data=FALSE;		//reset new data
				}			
		}
}

void System_Config (void){
	SYS_UnlockReg(); // Unlock protected registers

	// enable clock sources
	CLK->PWRCON |= (0x01 << 0);
	while(!(CLK->CLKSTATUS & (1 << 0)));

	//PLL configuration starts
	CLK->PLLCON &= ~(1<<19); //0: PLL input is HXT
	CLK->PLLCON &= ~(1<<16); //PLL in normal mode
	CLK->PLLCON &= (~(0x01FF << 0));
	CLK->PLLCON |= 48;
	CLK->PLLCON &= ~(1<<18); //0: enable PLLOUT
	while(!(CLK->CLKSTATUS & (0x01 << 2)));
	//PLL configuration ends

	// CPU clock source selection
	CLK->CLKSEL0 &= (~(0x07 << 0));
	CLK->CLKSEL0 |= (0x02 << 0);
	//clock frequency division
	CLK->CLKDIV &= (~(0x0F << 0));

	//UART0 Clock selection and configuration
	CLK->CLKSEL1 |= (0x03 << 24); // UART0 clock source is 22.1184 MHz
	CLK->CLKDIV &= ~(0x0F << 8); // clock divider is 1
	CLK->APBCLK |= (0x01 << 16); // enable UART0 clock
	//enable clock of SPI3
	CLK->APBCLK |= 1 << 15;
	SYS_LockReg();  // Lock protected registers
}

void UART0_Config(void) {
	// UART0 pin configuration. PB.0 pin is for UART0 RX
	SYS->GPB_MFP |= (1 << 0); // GPB_MFP[0] = 1 -> PB.0 is UART0 RX pin	
	PB->PMD &= ~(0b11 << 0);	// Set Pin Mode for GPB.0(RX - Input)
	
	// UART0 operation configuration
	UART0->FCR |= (0x03 << 1); // clear both TX & RX FIFO
	UART0->FCR &= ~(0x0F << 16); // FIFO Trigger Level is 1 byte
	UART0->LCR &= ~(0x01 << 3); // no parity bit
	UART0->LCR &= ~(0x01 << 2); // one stop bit
	UART0->LCR |= (0x03 << 0); // 8 data bit

	//Baud rate config: BRD/A = 70, DIV_X_EN=0 //Note 19200 Baudrate
	//--> Mode 0, Baud rate = UART_CLK/[16*(A+2)] = 22.1184 MHz/[16*(1+2)]= 192200 bps
	UART0->BAUD &= ~(0x0FFFF << 0);
	UART0->BAUD |= 70;//19200 BAUDRATE
	UART0->BAUD &= ~(0x03 << 28); // mode 0
	
	//UART interrupt 
	UART0->IER |= (1 << 0);// Enable interrupt for receive data
	NVIC->ISER[0] |= (1<<12); // Enable control register // bit 12 for UART0
	NVIC->IP[3] &= ~(0b11<<6); // Set priority // Highest priority '0'
}


void SPI3_Config(void) {
	SYS->GPD_MFP |= 1 << 11; //1: PD11 is configured for alternative function
	SYS->GPD_MFP |= 1 << 9; //1: PD9 is configured for alternative function
	SYS->GPD_MFP |= 1 << 8; //1: PD8 is configured for alternative function

	SPI3->CNTRL &= ~(1 << 23); //0: disable variable clock feature
	SPI3->CNTRL &= ~(1 << 22); //0: disable two bits transfer mode
	SPI3->CNTRL &= ~(1 << 18); //0: select Master mode
	SPI3->CNTRL &= ~(1 << 17); //0: disable SPI interrupt
	SPI3->CNTRL |= 1 << 11; //1: SPI clock idle high
	SPI3->CNTRL &= ~(1 << 10); //0: MSB is sent first
	SPI3->CNTRL &= ~(3 << 8); //00: one transmit/receive word will be executed in one data transfer

	SPI3->CNTRL &= ~(31 << 3); //Transmit/Receive bit length
	SPI3->CNTRL |= 9 << 3;     //9: 9 bits transmitted/received per data transfer

	SPI3->CNTRL |= (1 << 2);  //1: Transmit at negative edge of SPI CLK
	SPI3->DIVIDER = 0; // SPI clock divider. SPI clock = HCLK / ((DIVIDER+1)*2). HCLK = 50 MHz
}

void LCD_start(void)
{
	LCD_command(0xE2); // Set system reset
	LCD_command(0xA1); // Set Frame rate 100 fps
	LCD_command(0xEB); // Set LCD bias ratio E8~EB for 6~9 (min~max)
	LCD_command(0x81); // Set V BIAS potentiometer
	LCD_command(0xA0); // Set V BIAS potentiometer: A0 ()
	LCD_command(0xC0);
	LCD_command(0xAF); // Set Display Enable
}

void LCD_command(unsigned char temp)
{
	SPI3->SSR |= 1 << 0;
	SPI3->TX[0] = temp;
	SPI3->CNTRL |= 1 << 0;
	while (SPI3->CNTRL & (1 << 0));
	SPI3->SSR &= ~(1 << 0);
}

void LCD_data(unsigned char temp)
{
	SPI3->SSR |= 1 << 0;
	SPI3->TX[0] = 0x0100 + temp;
	SPI3->CNTRL |= 1 << 0;
	while (SPI3->CNTRL & (1 << 0));
	SPI3->SSR &= ~(1 << 0);
}

void LCD_clear(void)
{
	int16_t i;
	LCD_SetAddress(0x0, 0x0);
	for (i = 0; i < 132 * 8; i++)
	{
		LCD_data(0x00);
	}
}

void LCD_SetAddress(uint8_t PageAddr, uint8_t ColumnAddr)
{
	LCD_command(0xB0 | PageAddr);
	LCD_command(0x10 | (ColumnAddr >> 4) & 0xF);
	LCD_command(0x00 | (ColumnAddr & 0xF));
}

//------------------------------------------- main.c CODE ENDS ---------------------------------------------------------------------------

