#include <avr/io.h>
#include <avr/interrupt.h>

using namespace std;


enum light_state
{
    north,
    west,
    START_UP
};
enum light_state state;

int volatile second = 0;

const int n_green = 2;
const int n_red = 3;
const int w_green = 4;
const int w_red = 5;

const int pin_a = 3;
const int pin_b = 4;
const int pin_c = 5;
const int pin_d = 6;
const int pin_e = 7;
const int pin_f = 0;
const int pin_g = 1;


int main(void)
{
    //Traffic light LED setup
    DDRB |= (1 << n_red) | (1 << n_green) | (1 << w_red) | (1 << w_green);
    
    //Pedestrian light
    DDRD |= (1<<0);
    
    //Output setup
    //7 segment LED
    DDRD |= (1<<pin_a);
    DDRD |= (1<<pin_b);
    DDRD |= (1<<pin_c);
    DDRD |= (1<<pin_d);
    DDRD |= (1<<pin_e);
    DDRB |= (1<<pin_f);
    DDRB |= (1<<pin_g);

    //  //Setup timer Interrupt
    cli();
    TCCR1B |= (1 << WGM12); // Turn on the CTC mode for Timer 1
    TCCR1B |= (1 << CS12 )|(1<<CS10); // Set up Timer 1 with the prescaler of 1024
    OCR1A = 15426;    // Set CTC compare value to 0.125Hz at 16 MHz AVR clock , with a prescaler of 1024
    TIMSK1 = 1<<OCIE1A;     // Enable Output Compare A Match Interrupt
    sei();        //Enable the Global Interrupt Bit


    while (1)
    {
        
        if(second == 1){
          display_number_1();
        }
        else if(second == 2){
          display_number_2();
        }
        else if(second == 3){
          display_number_3();
          
        }
        
        
        
        if (second > 3) //flag 1 CTC mode
        {   
            
            
             switch (state)
            {
            case north: //car go north
                PORTB |= (1 << w_red) | (1 << n_green);
                PORTB &= ~((1 << w_green) | (1 << n_red));
                state = west;
                break;
            case west:
                PORTB &= ~((1 << w_red) | (1 << n_green));
                PORTB |= (1 << w_green) | (1 << n_red);
                state = north;
                break;
            case START_UP:
                PORTB &= ~((1 << n_red) | (1 << n_green) | (1 << w_red) | (1 << w_green));
                state = north;
                break;
            default:
                state = north;
                break;
            }
            second = 0;
            
        
            
        }

        
        
    }
}


ISR (TIMER1_COMPA_vect){

  second = second + 1;

}


void turn_off_all(){

    PORTD &= ~(1<<pin_a);
    PORTD &= ~(1<<pin_b);
    PORTD &= ~(1<<pin_c);
    PORTD &= ~(1<<pin_d);
    PORTD &= ~(1<<pin_e);
    PORTB &= ~(1<<pin_f);
    PORTB &= ~(1<<pin_g);

}


void display_number_1(){
  
  //Turn off segments
  turn_off_all();

  //Turn on segment
  PORTD |= (1<<pin_b);
  PORTD |= (1<<pin_c);
  
  
}

void display_number_2(){

  turn_off_all();

  PORTD |= (1<<pin_a);
  PORTD |= (1<<pin_b);
  PORTD |= (1<<pin_d);
  PORTD |= (1<<pin_e);
  PORTB |= (1<<pin_g);
  
}

void display_number_3(){

  turn_off_all();

  PORTD |= (1<<pin_a);
  PORTD |= (1<<pin_b);
  PORTD |= (1<<pin_c);
  PORTD |= (1<<pin_d);
  PORTB |= (1<<pin_g);
  
}

void display_number_4(){

  turn_off_all();

  PORTD |= (1<<pin_b);
  PORTD |= (1<<pin_c);
  PORTB |= (1<<pin_f);
  PORTB |= (1<<pin_g);
  
  
  
}
