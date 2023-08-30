#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Draw2D.h"
#include "picture.h"

void System_Config(void);
void SPI3_Config(void);
void UART0_Config(void);
void GPIO_Config(void);

void EINT1_IRQHandler(void);
void TMR0_IRQHandler(void);
void UART02_IRQHandler(void);

void LCD_start(void);
void LCD_command(unsigned char temp);
void LCD_data(unsigned char temp);
void LCD_clear(void);
void LCD_SetAddress(uint8_t PageAddr, uint8_t ColumnAddr);

void KeyPadEnable(void);
uint8_t KeyPadScanningX(void);
uint8_t KeyPadScanningY(void);

void Welcome_screen(void);
void Start_screen(void);
void Coordinates(void);
void Shots(void);
void User_win(void);	
void Gameover(void);

void Num_Shots_Display(int i);
void X_Display(void);
void Y_Display(void);
char ReceiveByte(void);

enum state
{
	Welcome,
	Start,
	Prepare,
	Play,
	GameW,
	GameL
};
enum state state;

static int pattern[] = {
 // gedbaf_dot_c
	0b10000010,  // Number 0          // ---a----
  0b11101110,  // Number 1          // |      |
  0b00000111,  // Number 2          // f      b
  0b01000110,  // Number 3          // |      |
  0b01101010,  // Number 4          // ---g----
  0b01010010,  // Number 5          // |      |
  0b00010010,  // Number 6          // e      c
  0b11100110,  // Number 7          // |      |
	0b00000010,  // Number 8          // ---d----
  0b01000010,  // Number 9
  0b11111111   // Blank LED 
};  

char map[8][8]; // Map to store the position of the ships (for compare purpose)	
char score_map[8][8]; // Map to update the position of the ships that have beeen hitted (for LCD display)

volatile int hits = 0; // Num of ship sunk (get hitted)
volatile int shots = 16; // Num of shots (ammo)
volatile int NewData = 0; // New data transfer valid
volatile int map_loaded = 0; // Map loaded successfully 
volatile int coordinate_set = 0; // X and Y coordinates selection (for button 9)
								
volatile int position_x = 0; // X-coordinate
volatile int position_y = 0; // Y-coordinate

volatile char data; // Data (1 byte) received from UART
volatile int SW1 = 0; // SW1 pressed
volatile int Segment_Display = 1; // 7 Segment selection

int main (void)
{
	System_Config();
	SPI3_Config();
	GPIO_Config();
	UART0_Config();
	
	LCD_start();
	LCD_clear();
	
	state = Welcome; // Default state
	
	while(1)
	{
		switch (state)
		{
			case Welcome:
				Welcome_screen();
				break;
			
			case Start:
				Start_screen();
				break;
			
			case Prepare:
				Coordinates();
				break;
			
			case Play:
				Shots();
				break;
			
			case GameW:
				User_win();
				break;
			
			case GameL:
				Gameover();
				break;
			
			default:
				break;
		}
	}
}

void EINT1_IRQHandler(void) // ISR for GPIOB15
{
	SW1++; // Increase the number when SW1 is pressed 
	CLK_SysTickDelay(50000); // Button debounce 
	PB->ISRC |= (1<<15); // Clear flag
}

void TMR0_IRQHandler(void) // ISR for TIMER0
{
	int num1 = shots / 10; // Take the number x from x6
	int num2 = shots % 10; // Take the number x from 1x
	
	if (Segment_Display == 0) // Condition to display coordinate
	{
		PC->DOUT |= (1<<7); // Turn on 7segement U11
		PC->DOUT &= ~(1<<6);		
		PC->DOUT &= ~(1<<5);			
		PC->DOUT &= ~(1<<4);
		
		if (coordinate_set == 0)  // Condition to display x-coordinate
		{
			X_Display(); // Display X-coordinate
		}
		else if (coordinate_set == 1)  // Condition to display y-coordinate
		{
			Y_Display(); // Display Y-coordinate
		}
		Segment_Display = 1; // Transition to the next 7segment display
	}
	
	else if (Segment_Display == 1)  // Condition to display number of remaining ammo
	{
		PC->DOUT &= ~(1<<7); 
		PC->DOUT &= ~(1<<6);		
		PC->DOUT |= (1<<5); // Turn on 7segement U13
		PC->DOUT &= ~(1<<4);	
		
		Num_Shots_Display(num1); // Display the remaining ammpo
		Segment_Display = 2; // Transition to the next 7segment display
	}
	
	else if (Segment_Display == 2) // Condition to display number of remaining ammo
	{
		PC->DOUT &= ~(1<<7); 
		PC->DOUT &= ~(1<<6);		
		PC->DOUT &= ~(1<<5);		
		PC->DOUT |= (1<<4);	// Turn on 7segement U14
		
		Num_Shots_Display(num2); // Display the remaining ammpo
		Segment_Display = 0; // Transition to the next 7segment display
	}
	TIMER0->TISR |= (1<<0);	// Clear flag
}

void UART02_IRQHandler(void) // ISR for UART
{
	data = ReceiveByte(); // Receive new data
	if (data == '1' || data == '0') // Condition to store new data into the array
	{
		NewData = 1; // New data is valid
	}
}

char ReceiveByte(void)
{
	return (char) UART0->DATA; // Receive new data in char type
}

void Welcome_screen(void) // Welcome screen state
{
	int i = 0; // Column in array map[] 
	int j = 0; // Row in array map[]
	int size = 0; // Maximum size of the map is 64 (8x8)
	
	draw_LCD(ship); // Display the Welcome screen
					
	while (1)
	{
		if (NewData && size != 64) // Condition to store new data
		{
			map[i][j] = data; // Store data into the array map[]
			score_map[i][j] = 0; // Store data into the array score_map[]
			i = i + 1; // Go to new column
			if (i == 8) // Condition to move to the next row
			{
				i = 0; // Reset back to the first collumn if we reached the last one
				j = j + 1; // Move to next row
			}
			NewData = 0;
			size++;
		}

		else if (size == 64) // Condition to stop receiving new data 
		{
			map_loaded = 1; // Confirm map is loaded
		}
		
		if (map_loaded == 1) // Map loaded successfully
		{
			state = Start; // Transfer to the next state
			LCD_clear();
			break;
		}
		
		if (SW1 > 0) // Avoid SW1 is accidentally pressed and skip 'Start' state
		{
			SW1 = 0;
		}
	}
}

void Start_screen(void) // Map loaded confirm screen 
{
	printS_5x7(0, 25, "Map loaded successfully");
	printS_5x7(0, 35, "Press SW1 button to start");
	
	while (1)
	{
		if (SW1 > 0) // Press SW1 to start game
		{
			state = Prepare; // Transfer to the Set Coordinate state
			SW1 = 0;
			LCD_clear();
			break;
		}
	}
}

void Coordinates(void) // Set X-Y coordinate 
{
	int16_t x = 0; // Move to the next column in array score_map[]
	int16_t y = 0; // Move to the next row in array score_map[]
	
	int i = 0; // Column in array score_map[] 
	int j = 0; // Row in array score_map[]
	int size = 0; // Maximum size of the map is 64 (8x8)
	int count = 0; // Variable to switch between x and y selection
	
	TIMER0->TCSR |= (1<<30); // Start TIMER0 counting
	
	while (1)
	{	
		while (size < 64) // Print the field on LCD
		{
			if (score_map[i][j] == 0) // Print "-" condition
			{
				printC_5x7(32+x, y, '-'); // Print "-" for the field
			}
			if (score_map[i][j] == 1) // Print "X" condition
			{
				printC_5x7(32+x, y, 'X'); // Print "X" for the field
			}
			i++; // Go to new column in the array
			x = x + 8; // Move to the next column of the field (on LCD)
			if (i == 8)
			{
				y = y + 8; // Move to the next row of the field (on LCD)
				j++; // Move to next row in the array
				x = 0; // Reset back to the first collumn of the field
				i = 0; // Reset back to the first collumn in the array
			}
			size++;
		}
		
		PA0 = 0; PA1 = 1; PA2 = 1; PA3 = 1; PA4 = 1; PA5 = 1;
		if (PA5 == 0) // Check if button 9 is pressed
		{
			CLK_SysTickDelay(50000); // Button debounce
			count++;
		}
		
		if (count % 2 == 0) // Condition to switch to X-coordinate from Y-coordinate
		{
			KeyPadScanningX(); // Scan for X-coordinate button press
			coordinate_set = 0; // X-coordinate
			PC->DOUT |= 1<<15; 
			PC->DOUT &= ~(1<<14); // Turn on LED7
		}
		
		if (count %2 != 0)// Condition to switch to Y-coordinate from X-coordinate
		{
			KeyPadScanningY(); // Scan for Y-coordinate button press
			coordinate_set = 1; // Y-coordinate
			PC->DOUT |= 1<<14; 
			PC->DOUT &= ~(1<<15); // Turn on LED8
		}
		
		if (SW1 > 0) // Press SW1 to shoot
		{
			state = Play; // Transfer to the Shot phase
			SW1 = 0;
			shots--; // Reduce number of ammo
			LCD_clear();
			break;
		}
	}
}
	
void Shots(void) // Shots phase
{
	PC->DOUT |= (1<<15);
	PC->DOUT |= (1<<14);
	
	while (1)
	{
		if (map[position_x][position_y] == '1' && shots > 0) // Hit condition
		{
			score_map[position_x][position_y] = 1; // Replace 0 with 1 in score_map array
			
			for (int i = 0; i < 6; i++) // Flash LED5 3 times
			{
				PC->DOUT ^= 1<<12;
				CLK_SysTickDelay(20000);
			}
			
			if (score_map[position_x-1][position_y] == 1 || score_map[position_x+1][position_y] == 1) // Condition to determine 1 ship sunk (horizontally)
			{
				hits++; // a ship sunk
			}
			
			if (score_map[position_x][position_y-1] == 1 || score_map[position_x][position_y+1] == 1) // Condition to determine 1 ship sunk (vertically)
			{
				hits++; // a ship sunk
			}
			
			position_x = 0; // Reset X-coordinate
			position_y = 0;  // Reset Y-coordinate
			
			if (hits == 5) // Win condition (hit all 5 ships)
			{
				state = GameW; // Transfer to the User-win state
				LCD_clear();
				break;
			}
				
			state = Prepare; // Back to set coordinate state
			LCD_clear();
			break;
		}
			
		if (shots > 0) // Miss condition
		{
			position_x = 0;  // Reset X-coordinate
			position_y = 0;  // Reset Y-coordinate
			
			state = Prepare; // Back to set coordinate state
			clear_LCD();
			break;
		}
		
		if (shots == 0 && hits < 5) // Lost condition (do not sink all the ships and run out of ammo)
		{
			state = GameL; // Transfer to the Gameover state
			LCD_clear();
			break;
		}		
	}
}

void User_win(void) // Win screen
{
	int beep = 0; // Variable to set the number of beep time
	int i = 0; // Column in array score_map[] 
	int j = 0; // Row in array score_map[]
	int size = 0; // Maximum size of the map is 64 (8x8)

	TIMER0->TCSR &= ~(1<<30); // Stop TIMER0 counting
	PC->DOUT &= ~(1<<7); 
	PC->DOUT &= ~(1<<6);		
	PC->DOUT &= ~(1<<5);		
	PC->DOUT &= ~(1<<4);		
	
	printS(30, 20, "You Win");
	
	while (1)
	{
		if (beep == 0) // 1 time only
		{
			for (int time = 0; time < 10; time++) // Buzzer beep 5 times
			{
				PB->DOUT ^= 1<<11;
				CLK_SysTickDelay(20000);
			}
			beep = 1;
		}
				
		if (SW1 > 0) // Press SW1 to back to Welcome screen
		{
			state = Welcome; // Transfer to the Welcome state
			SW1 = 0;
			shots = 16; // Reset the number of ammo
			hits = 0; // Reset the number of ship sunk
			position_x = 0;  // Reset X-coordinate
			position_y = 0;  // Reset Y-coordinate
			
			while (size < 64) // Clear the score_map
			{
				score_map[i][j] = 0;
				i++; // Go to new column in the array
				if (i == 8)
				{
					j++; // Move to next row in the array
					i = 0; // Reset back to the first collumn in the array
				}
				size++;
			}
				
			LCD_clear();
			break;
		}
	}
}

void Gameover(void) // Gameover screen
{
	int beep = 0; // Variable to set the number of beep time
	int i = 0; // Column in array score_map[] 
	int j = 0; // Row in array score_map[]
	int size = 0; // Maximum size of the map is 64 (8x8)
	
	TIMER0->TCSR &= ~(1<<30); // Stop TIMER0 counting
	PC->DOUT &= ~(1<<7); 
	PC->DOUT &= ~(1<<6);		
	PC->DOUT &= ~(1<<5);		
	PC->DOUT &= ~(1<<4);	

	printS(30, 20, "Gameover");
	
	while (1)
	{
		if (beep == 0) // 1 time only
		{
			for (int time = 0; time < 10; time++) // Buzzer beep 5 times
			{
				PB->DOUT ^= 1<<11;
				CLK_SysTickDelay(20000);
			}
			beep = 1;
		}
		
		if (SW1 > 0) // Press SW1 to back to Welcome screen
		{
			state = Welcome; // Transfer to the Welcome state
			SW1 = 0;
			shots = 16; // Reset the number of ammo
			hits = 0; // Reset the number of ship sunk
			position_x = 0;  // Reset X-coordinate
			position_y = 0;  // Reset Y-coordinate
			
			while (size < 64) // Clear the score_map
			{
				score_map[i][j] = 0;
				i++; // Go to new column in the array
				if (i == 8)
				{
					j++; // Move to next row in the array
					i = 0; // Reset back to the first collumn in the array
				}
				size++;
			}
			
			LCD_clear();
			break;
		}
	}
}

void System_Config(void) // Clock and Timer configuration
{
	SYS_UnlockReg(); // Unlock protected registers
  
	CLK->PWRCON |= (1<<0); // 12 Mhz CLK
	while(!(CLK->CLKSTATUS & 1<<0)); // wait the CLK to be stable
    
  CLK->CLKSEL0 &= ~(0b111<<0); // Clear all bits - select bit for 12 Mhz clk
	CLK->PWRCON &= ~(1<<7); // Normal mode
  CLK->CLKDIV &= ~(0x0F<<0); // Clock divider is 1
	
	// UART0 enable
	CLK->CLKSEL1 &= ~(0b11<<24); // UART0 clock source is 22.1184 MHz
	CLK->CLKSEL1 |= (0b11<<24); // UART0 clock source is 22.1184 MHz
	CLK->CLKDIV &= ~(0xF<<8); // Clock divider is 1
	CLK->APBCLK |= (1<<16); // Enable UART0 clock
  
	// SPI3 enable
  CLK->APBCLK |= (1<<15); // SPI3 enable
	
	// TIMER0 configuration
	CLK->CLKSEL1 &= ~(0b111<<8); // Select bit for 12Mhz CLK in TIMER0
	CLK->APBCLK |= (1<<2); // Enable TIMER0
	
	TIMER0->TCSR &= ~(0xFF<<0); // Clear all bits
	TIMER0->TCSR |= (1<<26); // Reset TIMER0
	
	TIMER0->TCSR &= ~(0b11<<27); // Clear bits
	TIMER0->TCSR |= (0b01<<27); // Periodic mode
	TIMER0->TCSR &= ~(1<<24); // Timer mode
	
	TIMER0->TCSR |= (1<<29); // Enable interrupt bit
	
	TIMER0->TCMPR = 5999; // (0.0005 / (1 / 12000000)), frequency: 2000Hz
	
	NVIC->ISER[0] |= (1<<8); // Enable control register
	NVIC->IP[2] |= (3<<6); // Set priority
	
	SYS_LockReg();  // Lock protected registers    
}

void SPI3_Config(void) // SPI3 configuration
{
	SYS->GPD_MFP |= (1<<11); // 1: PD11 is configured for alternative function
  SYS->GPD_MFP |= (1<<9); // 1: PD9 is configured for alternative function
  SYS->GPD_MFP |= (1<<8); // 1: PD8 is configured for alternative function
 
  SPI3->CNTRL &= ~(1<<23); // 0: disable variable clock feature
  SPI3->CNTRL &= ~(1<<22); // 0: disable two bits transfer mode
  SPI3->CNTRL &= ~(1<<18); // 0: select Master mode
  SPI3->CNTRL &= ~(1<<17); // 0: disable SPI interrupt    
  SPI3->CNTRL |= (1<<11); // 1: SPI clock idle high 
  SPI3->CNTRL &= ~(1<<10); // 0: MSB is sent first   
  SPI3->CNTRL &= ~(3<<8); // 00: one transmit/receive word will be executed in one data transfer
   
  SPI3->CNTRL &= ~(31<<3); // Transmit/Receive bit length
	SPI3->CNTRL |= (9<<3); // 9: 9 bits transmitted/received per data transfer
    
  SPI3->CNTRL |= (1<<2);  // 1: Transmit at negative edge of SPI CLK       
  SPI3->DIVIDER = 0; // SPI clock divider. SPI clock = HCLK / ((DIVIDER+1)*2). HCLK = 50 MHz
}

void UART0_Config(void) // UART0 configuration
{
	PB->PMD &= ~(0b11<<0); // Set input for PB.0
	SYS->GPB_MFP |= (1<<0); // GPB_MFP[0] = 1 -> PB.0 is UART0 RX pin

	UART0->LCR |= (0b11<<0); // 8 data bit
	UART0->LCR &= ~(1<<2); // One stop bit	
	UART0->LCR &= ~(1<<3); // No parity bit
	
	UART0->FCR |= (1<<1); // Clear RX FIFO
	UART0->FCR |= (1<<2); // Clear TX FIFO
	UART0->FCR &= ~(0xF<<4); // RX FIFO Interrupt Trigger Level is 1 byte
	UART0->IER |= (1<<0); // Enable interrupt for receive data

	//--> Mode 0, Baud rate = UART_CLK/[16*(A+2)] = 22.1184 MHz/[16*(142+2)]= 9600 bps
	UART0->BAUD &= ~(0b11<<28); // Mode 0	
	UART0->BAUD &= ~(0xFFFF<<0); // Clear the Baud rate divider
	UART0->BAUD |= 142; // A = 142 -> Baud rate = 9600 bps
	
	NVIC->ISER[0] |= (1<<12); // Enable control register
	NVIC->IP[3] &= ~(3<<6); // Set priority
}

void GPIO_Config(void) // GPIO configuration
{
	PB->PMD &= ~(0b11<<22); // Clear all bits of PB11
	PB->PMD |= (0b01<<22); // Output push-pull for PB11
	
	PC->PMD &= ~(0b11<<24); // Clear LED5
	PC->PMD |= (0b01<<24); // Output Push-pull LED5
	
	PC->PMD &= ~(0b11<<28); // Clear LED7
	PC->PMD |= (0b01<<28); // Output Push-pull LED7
	
	PC->PMD &= ~(0b11<<30); // Clear LED8
	PC->PMD |= (0b01<<30); // Output Push-pull LED8
	
	PC->PMD &= ~(0xFF<<8); // Clear 4-7 segments
	PC->PMD |= (0b01010101<<8); // Output Push-pull 7 segments
	
	PE->PMD &= ~(0xFFFF<<0); // Clear all digits in 7 seg
	PE->PMD |= (0b0101010101010101<<0); // Output Push-pull for all digits in 7 segments
	
	PB->PMD &= ~(0b11<<30); // Input-PB15
	PB->IMD &= ~(1<<15);
	PB->IEN |= (1<<15); // Enable interrupt
	NVIC->ISER[0] |= (1<<3); // Enable control register
	NVIC->IP[0] &= ~(3<<30); // Set priority
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
	SPI3->SSR |= (1<<0);  
	SPI3->TX[0] = temp;
	SPI3->CNTRL |= (1<<0);
	while(SPI3->CNTRL & (1<<0));
	SPI3->SSR &= ~(1<<0);
}

void LCD_data(unsigned char temp)
{
	SPI3->SSR |= (1<<0);  
	SPI3->TX[0] = 0x0100 + temp;
	SPI3->CNTRL |= (1<<0);
	while(SPI3->CNTRL & (1<<0));
	SPI3->SSR &= ~(1<<0);
}

void LCD_clear(void)
{	
	int16_t i;
	LCD_SetAddress(0x0, 0x0);			  								  
	for (i = 0; i < 132 *8; i++)
	{
		LCD_data(0x00);
	}
}

void LCD_SetAddress(uint8_t PageAddr, uint8_t ColumnAddr)
{
	LCD_command(0xB0 | PageAddr);
  LCD_command(0x10 | (ColumnAddr>>4) & 0xF); 
  LCD_command(0x00 | (ColumnAddr & 0xF));
}

void KeyPadEnable(void)
{
	GPIO_SetMode(PA, BIT0, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT1, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT2, GPIO_MODE_QUASI); 
  GPIO_SetMode(PA, BIT3, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT4, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT5, GPIO_MODE_QUASI);
}

uint8_t KeyPadScanningX(void) // Scan for x-coordinate
{
	PA0 = 1; PA1 = 1; PA2 = 0; PA3 = 1; PA4 = 1; PA5 = 1;
  if (PA3 == 0)
	{
		position_x = 0;
	}
  if (PA4 == 0)
	{
		position_x = 3;
	}
  if (PA5 == 0) 
	{
		position_x = 6;
	}
  
	PA0 = 1; PA1 = 0; PA2 = 1; PA3 = 1; PA4 = 1; PA5 = 1;
  if (PA3 == 0)
	{
		position_x = 1;
	}
  if (PA4 == 0)
	{
		position_x = 4;
	}
  if (PA5 == 0)
	{
		position_x = 7;
	}
  
	PA0 = 0; PA1 = 1; PA2 = 1; PA3 = 1; PA4 = 1; PA5 = 1;
  if (PA3 == 0) 
	{
		position_x = 2;
	}
  if (PA4 == 0)
	{
		position_x = 5;
	}
  return 0;
}

uint8_t KeyPadScanningY(void) // Scan for y-coordinate
{
	PA0 = 1; PA1 = 1; PA2 = 0; PA3 = 1; PA4 = 1; PA5 = 1;
  if (PA3 == 0)
	{
		position_y = 0;
	}
  if (PA4 == 0)
	{
		position_y = 3;
	}
  if (PA5 == 0) 
	{
		position_y = 6;
	}
  
	PA0 = 1; PA1 = 0; PA2 = 1; PA3 = 1; PA4 = 1; PA5 = 1;
  if (PA3 == 0)
	{
		position_y = 1;
	}
  if (PA4 == 0)
	{
		position_y = 4;
	}
  if (PA5 == 0)
	{
		position_y = 7;
	}
  
	PA0 = 0; PA1 = 1; PA2 = 1; PA3 = 1; PA4 = 1; PA5 = 1;
  if (PA3 == 0) 
	{
		position_y = 2;
	}
  if (PA4 == 0)
	{
		position_y = 5;
	}
  return 0;
}

void Num_Shots_Display(int i) // Display num of shots 
{
	if (i == 0)
	{
		PE->DOUT = pattern[0];
	}
	
	else if (i == 1)
	{
		PE->DOUT = pattern[1];
	}	
	
	else if (i == 2)
	{
		PE->DOUT = pattern[2];
	}	
	
	else if (i == 3)
	{
		PE->DOUT = pattern[3];
	}	
	
	else if (i == 4)
	{
		PE->DOUT = pattern[4];
	}	
	
	else if (i == 5)
	{
		PE->DOUT = pattern[5];
	}	
	
	else if (i == 6)
	{
		PE->DOUT = pattern[6];
	}	
	
	else if (i == 7)
	{
		PE->DOUT = pattern[7];
	}	
	
	else if (i == 8)
	{
		PE->DOUT = pattern[8];
	}	
	
	else if (i == 9)
	{
		PE->DOUT = pattern[9];
	}	
}

void X_Display(void) // Display num of shots (1x)
{
	if (position_x == 0)
	{
		PE->DOUT = pattern[1];
	}
	
	else if (position_x == 1)
	{
		PE->DOUT = pattern[2];
	}	
	
	else if (position_x == 2)
	{
		PE->DOUT = pattern[3];
	}	
	
	else if (position_x == 3)
	{
		PE->DOUT = pattern[4];
	}	
	
	else if (position_x == 4)
	{
		PE->DOUT = pattern[5];
	}	
	
	else if (position_x == 5)
	{
		PE->DOUT = pattern[6];
	}	
	
	else if (position_x == 6)
	{
		PE->DOUT = pattern[7];
	}	
	
	else if (position_x == 7)
	{
		PE->DOUT = pattern[8];
	}	
}

void Y_Display(void) // Display num of shots (1x)
{
	if (position_y == 0)
	{
		PE->DOUT = pattern[1];
	}
	
	else if (position_y == 1)
	{
		PE->DOUT = pattern[2];
	}	
	
	else if (position_y == 2)
	{
		PE->DOUT = pattern[3];
	}	
	
	else if (position_y == 3)
	{
		PE->DOUT = pattern[4];
	}	
	
	else if (position_y == 4)
	{
		PE->DOUT = pattern[5];
	}	
	
	else if (position_y == 5)
	{
		PE->DOUT = pattern[6];
	}	
	
	else if (position_y == 6)
	{
		PE->DOUT = pattern[7];
	}	
	
	else if (position_y == 7)
	{
		PE->DOUT = pattern[8];
	}	
}
/*
// Picture.h library
extern unsigned char ship[128*64] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x3D,0x7D,0x7D,0x7D,0x7D,0x3D,0xFF,0xFF,0xFF,0xFF,0x24,0x24,0x04,0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xC0,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x9C,0x9C,0x9C,0xBE,0xBE,0xBE,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xFF,0xFF,0xFF,0xFF,0xA3,0xA3,0xA1,0x21,0x21,0x21,0x10,0x10,0x10,0x18,0x18,0x18,0xFF,0xFF,0xFF,0x9E,0x9E,0x9E,0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x07,0x07,0x07,0x0F,0x1F,0x1F,0x1F,0x1D,0x1D,0x1F,0x1F,0x1F,0x1F,0x1D,0x1D,0x1F,0x1F,0x1F,0x1F,0x1D,0x1D,0x1D,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1E,0x1E,0x1E,0x0F,0x07,0x07,0x06,0x02,0x02,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
*/
