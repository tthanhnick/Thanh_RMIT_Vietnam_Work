#ifndef F_CPU
#define F_CPU 16000000UL
#endif


#include <avr/io.h>
#include <avr/interrupt.h>

int volatile counter_1_sec = 0;
int volatile counter_2_sec = 0;
int volatile counter_3_sec = 0;
int volatile counter_4_sec = 0;

int main(void)
{
  DDRD |= (1 << 1); // Set LED as output
  TCCR1B |= (1 << WGM12); // Turn on the CTC mode for Timer 1
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set up Timer 1 with the prescaler of 1024
  TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B);     // Enable Output Compare A and B Match Interrupt
  OCR1A = 46872;    // Set the Period of 4 seconds 
  OCR1B = 15624;    // Set the duty cycle = OCR1B/OCR1A  //Time off
  sei();        //Enable the Global Interrupt Bit


  
  while (1)
  {
    //If 3 seconds variable happens 3 times, we change the OCR1A value for 2 seconds
    if(counter_3_sec >= 3){
      OCR1A = 31248;
    }

    //If 2 seconds variable happens 1 time, we change the value of ORCR1A into 1 seconds
    if(counter_2_sec >= 1){
      OCR1A = 15624;
    }

    //If 1 second variable happens 3 times, we change the OCR1A to fits with 4 seconds interval
    if(counter_1_sec >= 3){
      OCR1A = 62499;
    }

    //When it is at 3 seconds interval, turn on and turn off when counter_3_sec is even or odd
    if(OCR1A == 46872){
      if(counter_3_sec % 2 == 0){
        PORTD &= ~(1<<1);
      }
      else if(counter_3_sec % 2 != 0){
        PORTD |= (1<<1);
      }
    }
    

    if(OCR1A == 31248){
      if(counter_2_sec % 2 == 0){
        PORTD |= (1<<1);
      }
    }


    if(OCR1A == 15624){
      if(counter_1_sec % 2 == 0){
        PORTD &= ~(1<<1);
      }
      else if(counter_1_sec % 2 != 0){
        PORTD |= (1<<1);
      }
    }

    if(OCR1A == 62499){
      //4 seconds mark
      if(counter_4_sec % 2 == 0){
        PORTD |= (1<<1);
      }
      //after 4 seconds interval
      else if(counter_4_sec % 2 != 0){
        PORTD &= ~(1<<1);
        DDRD &= (0<<1); //TURN OFF
        
      }
    }
    
  }
  
}

//ISR for OCR1A
ISR(TIMER1_COMPA_vect)
{
   //Increase based on OCR1A value
  if(OCR1A == 15624){
    counter_1_sec += 1;
  }

  if(OCR1A == 31248){
    counter_2_sec += 1;
  }
  
  if(OCR1A == 46872){
    counter_3_sec += 1;
  }
  
  if(OCR1A == 62499){
    counter_4_sec += 1;
  }
  
}

ISR(TIMER1_COMPB_vect)
{
  
  
}
