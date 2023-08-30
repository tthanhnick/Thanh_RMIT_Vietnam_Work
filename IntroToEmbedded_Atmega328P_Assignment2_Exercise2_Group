#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void){
    DDRB |=(1<<DDB0); //OUTPUT B0
    DDRD &=~(1<<DDD3); //INPUT D3
    TCNT2=0; //Initialize Counter

   //Timer 2
    TCCR2A |= (1 << WGM21)  ; // set CTC mode
    TCCR2B |= (1 << CS22) ;//Set prescaler to 64
    OCR2B = 249; // set compare value for interrupt at 250 Hz
    
    TIMSK2 |= (1 << OCIE2B); // set output compare interrupt enable

    EICRA |= (1 << ISC10); //Any logical interrupt change
    EIMSK |= (1 << INT1);  //External request 0
    sei(); // enable global interrupt
    while(1){

    } 
}
ISR(TIMER2_COMPB_vect){
PORTB^=(1<<PORTB0); //Blink LED
}
ISR(INT1_vect)
{
    PORTB &= ~(1 << PORTB0); //Turn Off LED
}
