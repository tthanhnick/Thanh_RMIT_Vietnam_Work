#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
volatile int count = 0; //count value
//Clock signal of GPIO2 16 cycle - need 1s between GPIO1 and GPIO2 so need to add 1 elements to array
int one_cycle[33] = {0, 0, 1, 0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1};
//hardcoded data in GPIO3
int data[16] = {1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0};

int main(void)
{
    int elapsed = 0;// count the elapsed interval
    
    DDRD &= ~(1 << 2);//Input button PIN D2

    //Output PIN
    DDRD |= (1 << 3); //GPIO1
    DDRD |= (1 << 4); //GPIO2
    DDRD |= (1 << 5); //GPIO3

    //  //Setup timer Interrupt
    TCCR1B |= (1 << WGM12);              // Turn on the CTC mode for Timer 1
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set up Timer 1 with the prescaler of 1024
    OCR1A = 15624;                       // Set CTC compare value to 0.5Hz at 16 MHz AVR clock , with a prescaler of 1024
    TIMSK1 = 1 << OCIE1A;                // Enable Output Compare A Match Interrupt
    
    while (1)
    {
        if (!(PIND & (1 << PIND2))) //button press
        {
           _delay_ms(500);//Reduce bouncing effect
          PORTD |= (1 << 3);//GPIO1 Turn on immidiately
          sei(); //Enable the Global Interrupt Bit
        }
        if (count >= 1)
            PORTD |= (1 << 3);//Keep GPIO1 ON until the 2 cycle end
        if (one_cycle[count] == 0) //if not at rising edge
        {
            PORTD &= ~(1 << 4); //Turn off GPIO2
            PORTD &= ~(1 << 5); //Turn off GPIO3
        }
        else if (one_cycle[count] == 1) //if at rising edge
        {
            PORTD |= (1 << 4); //Turn on GPIO2
            /*
              Odd number =2*increment number+1
              => increment number=(Odd number-1)/2 
              */
            if (data[(count - 1) / 2] == 1)
            {
                PORTD |= (1 << 5); //Turn on GPIO3
            }
            else if (data[(count - 1) / 2] == 0)
            {
                PORTD &= ~(1 << 5); //Turn off GPIO3
            }
        }
        else if (count == 33) //when reach one cycle
        {
            count = 0; //reset the counter
            elapsed++; //count the cycle
        }
        if (elapsed == 2) //if we reach two cycles
        {
            cli();//Disable the Global Interrupt Bit
            //Turn off all GPIOS
            PORTD &= ~(1 << 3);
            PORTD &= ~(1 << 4);
            PORTD &= ~(1 << 5);
            elapsed = 0; //reset cycle to default
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    count++; //count value
}
