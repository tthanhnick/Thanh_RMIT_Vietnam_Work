#ifndef F_CPU
#define F_CPU 16000000UL //Oscilate at 16Mhz
#endif

//Declare io and delay library
#include <avr/io.h>
#include <util/delay.h>

/*  
  Common Cathode 7 segment LED
  0bDP,C,D,E,B,A,F,G - LED map
  It is from Pin 7 to Pin 0 of Port D
  All number are real and none negative number =>unsigned char
  Global Array 
  */
unsigned char number_5[] = {0b01100111};
unsigned char number_1[] = {0b01001000};
unsigned char number_3[] = {0b01101101};

//Main program
int main(void){
  DDRD=0xFF;//All ports D are output
  PORTD=0x00;//All LED need to be off
  DDRB &= ~(1<<DDB0);  //Input pin B0
  DDRB &= ~(1<<DDB1);  //Input Pin B1
    while(1){
     if(!(PINB & (1<<PINB0))){ //Button 1 is pressed
      for(int i=0;i<10;i++){ //Flash 10 times
        for(int i=0;i<8;i++){ //Print array out
          PORTD=number_1[i]; //Print number 1
            }
          _delay_ms(500); //delay 1s
            for(int i=0;i<8;i++){
              PORTD=0x00; //Turn off LED
          }
        _delay_ms(500);
    }
   }
    else if(!(PINB & (1<<PINB1))){ //Button 2 is pressed
      for(int i=0;i<3;i++){ //Flash 3 times
        for(int i=0;i<8;i++){ //Print array out
          PORTD=number_3[i]; //Print number 1
          }
          _delay_ms(2500); //delay 5s
            for(int i=0;i<8;i++){
              PORTD=0x00; //Turn off LED
          }
        _delay_ms(2500);
    }
   }
    else{ //Not Press any buttons
     for(int i=0;i<8;i++){ //Print Array out
        PORTD=number_5[i]; //Number 5
        } 
    }
  }
}
