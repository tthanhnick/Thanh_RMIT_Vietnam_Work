#include <avr/io.h>
#include <avr/interrupt.h>

//enumerate the light to state
enum light_state
{
    a_c,
    b_d,
    cross_road,
    START_UP
};
enum light_state state;

const int a_light = 1;
const int b_light = 2;
const int c_light = 3;
const int d_light = 4;
const int walk_light = 0;

volatile int two_second = 1;
volatile bool four_elapsed = false;
//volatile bool twosec_elapsed = false;
volatile bool button_pressed = false;

volatile int count = 0; //initialize couting value
volatile int timer_sec = 1;
/* Simplicity
On Green
Off Red 
*/

int main(void)
{
    DDRD &= ~(1 << 2); //Push button
    DDRB |= (1 << a_light) | (1 << b_light) | (1 << c_light) | (1 << d_light) | (1 << walk_light); //Traffic light

    TCCR1B |= (1 << WGM12);              //CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set up the prescaler to 1024 (CS12 and CS10)
    OCR1A = 15624; //Two second timer

    TIMSK1 = 1 << OCIE1A; // Enable Output Compare A Match Interrupt
    
    DDRD |= (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 5)|(1<<6)|(1<<7); //7 segemtn LED
    PORTD &= ~((1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 5)|(1<<6)|(1<<7)); //All LED need to be off

    //enable interrupts
    EIMSK |= (1 << INT0);  //enable PORTD2 as external interrupt
    EICRA |= (1 << ISC00); // Detect on falling edge
    EIFR |= (1 << INTF0);  // Clear flag
    sei();                 //Enable the Global Interrupt Bit
     while (1)
    {
        if (button_pressed)
        { //button press
          number_counting2(); //count down
            if (two_second == 2) //when 2 second
            {
                switch (state)
                {
                case cross_road: //b pedestrian on
                    PORTB |= (1 << walk_light) | (1 << b_light);
                    PORTB &= ~((1 << c_light) | (1 << d_light) | (1 << a_light));
                    state = b_d;
                    break;
                case b_d: //b d on
                    PORTB &= ~((1 << a_light) | (1 << c_light) | (1 << walk_light));
                    PORTB |= (1 << b_light) | (1 << d_light);
                    state = cross_road;
                    break;
                case START_UP: //reset state
                    PORTB &= ~((1 << a_light) | (1 << b_light) | (1 << c_light) | (1 << d_light) | (1 << walk_light));
                    state = cross_road;
                    break;
                default:
                    state = cross_road;
                    break;
                }
                button_pressed = false;
                two_second =1;
            }
        }
        else
        { //no button press
            number_counting4();
            if (four_elapsed) //When 4 seconds
            {
                switch (state)
                {
                case a_c: //a c on
                    PORTB |= (1 << a_light) | (1 << c_light);
                    PORTB &= ~((1 << b_light) | (1 << d_light) | (1 << walk_light));
                    state = b_d;
                    break;
                case b_d: //b_d on
                    PORTB &= ~((1 << a_light) | (1 << c_light) | ((1 << walk_light)));
                    PORTB |= (1 << b_light) | (1 << d_light);
                    state = a_c;
                    break;
                case START_UP: //reset state
                    PORTB &= ~((1 << a_light) | (1 << b_light) | (1 < c_light) | (1 < d_light) | (1 << walk_light));
                    state = a_c;
                    break;
                default:
                    state = a_c;
                    break;
                }
                four_elapsed = false;
            }
        }
    }
}
ISR(INT0_vect)
{
    button_pressed = true;
}
ISR(TIMER1_COMPA_vect) //0.5 seconds? how can we expand to 4 seconds?
{
    sei(); //enable global interrupt bit
    //twosec_elapsed = true; //0.5 seconds has been elapsed.
    if (two_second == 4)   // 4 second has passed
    {
        four_elapsed = true; // Indicate has 4 seconds already
        two_second = 1; //reset to default
    }
    else
    {
        two_second++;
    }
    count++;
}
//number count down process
void number_counting4(){
 displaySEG();
        if (count == 1)
            timer_sec = 1;
        else if (count == 2)
            timer_sec = 2;
        else if (count == 3)
            timer_sec = 3;
        else if (count == 4)
        {
            timer_sec = 4;
            count = 0;
        }
}
void number_counting2(){
 displaySEG();
    if (count == 1)
        timer_sec = 1;
    else if (count == 2)
        timer_sec = 2;
    count = 0;
}
void displaySEG() //routine for display LED
{
    if (timer_sec == 1)
    {
        PORTD = 0b10010000; //Number 1 
    }
    else if (timer_sec == 2)
    {
        PORTD = 0b01111001; //Number 2 
    }
    else if (timer_sec == 3)
    {
        PORTD = 0b11011001; //Number 3 
    }
     else if (timer_sec == 4)
    {
        PORTD = 0b10010011; //Number 4 
    }
}
