//Declare the library
#include <avr/io.h>

int main ( void )
{
  unsigned char Count_Var = 0; //Declare a counter variable and initialize it to 0
  DDRB |= (1 << 5); // Set pin as output
  TCCR1B |= (1 << CS12 )|(1<<CS10); // Set up the prescaler to 1024 (CS12 and CS10)
  while(1)
  {
    //Check the timer value if the count matches 0.5 second (this is from the pre-vious task)
    if ( TCNT1 >= 62499) //Calculated target time count
    {
      TCNT1 = 0; // Reset timer value
      Count_Var ++;   //Increment the counter variable each time by 1
      if (Count_Var == 1) // 8 seconds period 4 seconds up and 4 seconds down
      {
        Count_Var = 0; // Reset counter variable
        PORTB ^= (1 << 5); // Trigger the pin
      }
    }
  }
} 
