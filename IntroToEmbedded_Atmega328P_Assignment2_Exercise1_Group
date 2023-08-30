#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

int main(void){

  DDRD &= ~(1<<2); //Button 1 Interrupt INT0
  DDRD &= ~(1<<3); //Button 2 Interrupt INT1


  DDRB |= (1<<0); //OUTPUT LED 1
  DDRB |= (1<<1); //OUTPUT LED 2

  //Setup INT0 Interrupt
  EIMSK |= (1<<INT0);
  EICRA |= (1<<ISC01);
  sei();

  //Setup INT1 Interrupt
  EIMSK |= (1<<INT1);
  EICRA |= (1<<ISC11);
  sei();

  

  //Loop
  while (1)
  {
    //LED 2
    PORTB |= (1<<1);
    _delay_ms(1000);
    PORTB &= ~(1<<1);
    _delay_ms(1000);
  


  }



}

//External interrupt for LED 1
ISR ( INT0_vect )
{
  PORTB |= (1<<PORTB0); //Turn on
}

ISR (INT1_vect){

  //PORTB ^= (0<<PORTB0);
  PORTB &= ~(1<<PORTB0); //Turn off

}





  
