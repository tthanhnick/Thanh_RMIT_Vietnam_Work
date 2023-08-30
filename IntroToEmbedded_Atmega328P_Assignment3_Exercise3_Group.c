#ifndef F_CPU 
#define F_CPU 16000000UL
#endif



#include <avr/io.h>
#include <avr/interrupt.h>

 volatile int counter = 0;
 volatile bool oneMinute = false;

//The initial hour and minute
 int initial_hour_1 = 0;
 int initial_hour_2 = 0;
 int initial_minute_1 = 0;
 int initial_minute_2 = 0;

//Set the alarm time 
 int alarm_hour_1 = 1;
 int alarm_hour_2 = 2;
 int alarm_minute_1 = 4;
 int alarm_minute_2 = 5;

 bool alarm_status = false;
 bool alarm_flashing_lock = false;
 int alarm_limit = 0;

int main(void){

  
  //First 7-segment set up
  DDRC |= (1<<4); //A
  DDRC |= (1<<5); //B
  DDRD |= (1<<2); //C
  DDRD |= (1<<3); //D
  DDRD |= (1<<4); //E
  DDRD |= (1<<5); //F
  DDRD |= (1<<6); //G

  // Second 7-segment set up
  DDRD |= (1<<7); //A
  DDRB |= (1<<0); //B
  DDRB |= (1<<1); //C
  DDRB |= (1<<2); //D
  DDRB |= (1<<3); //E
  DDRB |= (1<<4); //F
  DDRB |= (1<<5); //G

  //Alarm LED
  DDRD |= (1<<1);
  
  //INPUT
  DDRC &= ~(1<<0); // BUTTON 1
  DDRC &= ~(1<<1); // BUTTON 2
  DDRC &= ~(1<<2); // BUTTON 3
  DDRC &= ~(1<<3); // BUTTON 4
  
  //Setup Timer 1 Interrupt
  TCCR1B |= (1 << WGM12); // Turn on the CTC mode for Timer 1
  TCCR1B |= (1 << CS12 )|(1<<CS10); // Set up Timer 1 with the prescaler of 1024
  OCR1A = 15624; // equals 1 second     // Set CTC compare value to 0.125Hz at 16 MHz AVR clock , with a prescaler of 1024
  TIMSK1 = 1<<OCIE1A;     // Enable Output Compare A Match Interrupt
  sei(); //Enable the Global Interrupt Bit

  //This variable used by display button, switch from view hour to view minute
  int time_press = 0;

  //Set up the time after plugged in the usb, **CRUCIAL STEPS**
  set_up_initial_time();

  //Monitor time
  while (true){
    
    //FUNCTIONALITY FOR BUTTON 1, SWITCH FROM SET MODE TO TIMER MODE
    if( !(PINC & (1<<PINC0)) && (PINC | (0<<PINC1)) && (PINC | (0<<PINC2)) && (PINC | (0<<PINC3) )){
      //Delay to avoid floating
      _delay_ms(250);
      //Set the initial time
      set_up_initial_time();
      
    }
    
    //FUNCTIONALITY FOR BUTTON 4, DISPLAY BUTTON
    if(!(PINC & (1<<PINC3)) && (PINC | (0<<PINC0)) && (PINC | (0<<PINC2)) && (PINC | (0<<PINC1) )){
      _delay_ms(250);
      time_press += 1;
    }

    
    //DISPLAY THE TIME OUT
    // Ternary operator
    time_press % 2 == 0 ? display_initial_hour() : display_initial_minute();

    //Update the system time to keep it running
    update_system_time();

    //Check alarm time
    check_alarm_time();
  }
  
}

//Timer counter for one second
ISR (TIMER1_COMPA_vect){
  counter += 1;
  if(counter > 59){
    counter = 0;
    oneMinute = true;
  }
  
}
//***************************MOST IMPORTANT FUNCTIONS*********************************
void update_system_time(){

  if(oneMinute == true){
    initial_minute_2 += 1;
    oneMinute = false;
  }

  if(initial_minute_2 > 9){
    initial_minute_2 = 0;
    initial_minute_1 += 1;
  }

  if(initial_minute_1 > 5){
    initial_minute_2 = 0;
    initial_minute_1 = 0;

        if( initial_hour_1 == 0 && initial_hour_2 > 9  ){

          initial_hour_2 = 0;
          initial_hour_1 += 1;
     
        }

        else if(initial_hour_1 == 1 && initial_hour_2 > 9){
      
         initial_hour_2 = 0;
         initial_hour_1 += 1;
     
        }

       else if(initial_hour_1 == 2 && initial_hour_2 >= 3){
        
        initial_hour_1 = 0;
        initial_hour_2 = 0;
        initial_minute_1 = 0;
        initial_minute_2 = 0;
        
       }

      else{
        initial_hour_2 += 1;
      }
    
  }

}

//Function to verify if alarm is met
void check_alarm_time(){
  
  //If system time meet alarm time                                                                                                                        // 50 for 5 seconds
  if( ((initial_hour_1 == alarm_hour_1) && (initial_hour_2 == alarm_hour_2) && (initial_minute_1 == alarm_minute_1) && (initial_minute_2 == alarm_minute_2)) && alarm_limit <= 50  ){
    //If the flashing alarm is not locked
    if(alarm_flashing_lock == false){
     PORTD |= (1<<1);
     //This one is 50 because we delay for 10Hz => f = 1/T => T = 0.1(seconds) => on for 0.05 seconds and off for 0.05 seconds
    _delay_ms(50);
    PORTD &= ~(1<<1);
    _delay_ms(50);
    alarm_limit += 1; // Increment
    }

    //Alarm limit is designated at 50 because we need to flash for 5 seconds so we take 5 seconds divided with 0.1 as 1 period we will receive 50, 50 means we need to repeat 50 cycles to yields 5 seconds.
    
  }

  //If the system pass the alarm mark, we release the lock and reset the limit
  if( (initial_hour_1 != alarm_hour_1) || (initial_hour_2 != alarm_hour_2) || (initial_minute_1 != alarm_minute_1) || (initial_minute_2 != alarm_minute_2) ){
    alarm_flashing_lock = false;
    alarm_limit = 0;
  }
  
  //If alarm limit > 50, meaning it has completed flashing for 5 seconds with the frequency of 10hz, we lock the flashing mechanism
  if( ((initial_hour_1 == alarm_hour_1) && (initial_hour_2 == alarm_hour_2) && (initial_minute_1 == alarm_minute_1) && (initial_minute_2 == alarm_minute_2)) && alarm_limit > 50  ){
    alarm_flashing_lock = true;
  }
  
}

//********************2ND MOST IMPORTANT FUNCTIONS******************************
//
//Function to set up the initial time, travel through 2 phases
void set_up_initial_time(){

  set_up_initial_time_hour();

  set_up_initial_time_minute();
  
  
}

//Function to set up initial time hour
void set_up_initial_time_hour(){

  //Turn on LED, showing that they are handling hour section
  PORTD |= (1<<1);
  //Get the value from the global variables
  int hour_left_digit = initial_hour_1;
  int hour_right_digit = initial_hour_2;

  //Button to change between left digit and right digit
  bool shifting_left_and_right = false;
  //Button to go to minute set up
  bool button_4_pressed = false;

  //Set them up, using do-while
  do{

    //Flashing digit to indicate, FALSE means left side and TRUE means right side
    if(shifting_left_and_right == false){
      //if it is left side, we hold the right digit on
      show_the_digit_right(hour_right_digit);
      //Flash the left digit
      if(counter % 2 == 0){
        turn_off_all_digit_left();
      }
      else if(counter % 2 != 0){
        show_the_digit_left(hour_left_digit);
      }
    }

    //If we are on right digit
    if(shifting_left_and_right == true){
      //Hold the left digit on
      show_the_digit_left(hour_left_digit);
      //Flash the right digit
      if(counter % 2 == 0){
        turn_off_all_digit_right();
      }
      else if(counter % 2 != 0){
        show_the_digit_right(hour_right_digit);
      }
    }

    //Press this button, which is button 4 on the right most side, will take the program to set up minute
    if(!(PINC & (1<<PINC3)) && (PINC | (0<<PINC0)) && (PINC | (0<<PINC2)) && (PINC | (0<<PINC1) )){
      _delay_ms(1000);
      button_4_pressed = true;
    }

    //Reset the counter to avoid + 1 on minute after, improve time accuracy
    if(counter > 1){
      counter = 0;
    }

    //Digit button - button 3 - false means LEFT side and true means RIGHT side
    if(!(PINC & (1<<PINC2)) && (PINC | (0<<PINC0)) && (PINC | (0<<PINC1)) && (PINC | (0<<PINC3) )){
      _delay_ms(250); // avoid bouncing
      //Change direction
      if(shifting_left_and_right == true){
        shifting_left_and_right = false;
      }
      else if(shifting_left_and_right == false){
        shifting_left_and_right = true;
      }
    }
    //SETTING UP THE TIME, included condition for them to range from 0 -> 24, minimize error such as hour = 26
    // Value button - button 2 - increasing depends on which side we are on
    if(!(PINC & (1<<PINC1)) && (PINC | (0<<PINC0)) && (PINC | (0<<PINC2)) && (PINC | (0<<PINC3) ) && shifting_left_and_right == false ){
      _delay_ms(250);
      if( hour_left_digit < 1 && hour_right_digit >= 5){
        hour_left_digit += 1;
      }
      else if( hour_left_digit < 2 && hour_right_digit <= 4){
        hour_left_digit += 1;
      }
      else{
        hour_left_digit = 0;
      }
    }
    //This one is for increasing value on right digit
    else if(!(PINC & (1<<PINC1)) && (PINC | (0<<PINC0)) && (PINC | (0<<PINC2)) && (PINC | (0<<PINC3) ) && shifting_left_and_right == true){
      _delay_ms(250);
      if( (hour_left_digit == 0 || hour_left_digit == 1) && hour_right_digit <= 9){
        hour_right_digit += 1;
      }
      else if(hour_left_digit == 2 && hour_right_digit < 3){
        hour_right_digit += 1;
      }
      else{
        hour_right_digit = 0;
      }
    }
    
  }while(button_4_pressed != true);
  button_4_pressed = false;

  //Assign the value for hour
  initial_hour_1 = hour_left_digit;
  initial_hour_2 = hour_right_digit;
  
}

//Function to set up initial time minute
void set_up_initial_time_minute(){

  //Turn off LED, indicating that they are handling minute section
  PORTD &= ~(1<<1);
  
  int minute_left_digit = initial_minute_1;
  int minute_right_digit = initial_minute_2;
  //FALSE means left side, TRUE means right side
  bool shifting_left_and_right = false;
  //Button to break the loop and enter the main function again.
  bool button_1_pressed = false;

  
  //Set them up
  do{
    //Show them which digit they are adjusting, if FALSE means left side and TRUE means right side
    //If user are adjusting left digit
    if(shifting_left_and_right == false){
      //Hold right digit on
      show_the_digit_right(minute_right_digit);
      //Flash the left digit
      if(counter % 2 == 0){
        turn_off_all_digit_left();
      }
      else if(counter % 2 != 0){
        show_the_digit_left(minute_left_digit);
      }
    }
    //If user are adjusting right digit
    if(shifting_left_and_right == true){
      //Hold left digit on
      show_the_digit_left(minute_left_digit);
      //Flash right digit
      if(counter % 2 == 0){
        turn_off_all_digit_right();
      }
      else if(counter % 2 != 0){
        show_the_digit_right(minute_right_digit);
      }
    }

    //Reset the counter to avoid + 1 on minute, improve accuracy
    if(counter > 1){
      counter = 0;
    }
    
    //Press this button, which is button 1 on the right most side, will take the program to set up minute
    if(!(PINC & (1<<PINC0)) && (PINC | (0<<PINC1)) && (PINC | (0<<PINC2)) && (PINC | (0<<PINC3) )){
      _delay_ms(200);
      counter = 0; //Improve accuracy when setting up minute, prevent + 1 after set up
      button_1_pressed = true; // Break the loop and jump to main
    }

    //Change left and right, FALSE MEANS LEFT SIDE, TRUE MEANS RIGHT SIDE
    if(!(PINC & (1<<PINC2)) && (PINC | (0<<PINC0)) && (PINC | (0<<PINC1)) && (PINC | (0<<PINC3) )){
      _delay_ms(250); // avoid bouncing
      //Change direction
      if(shifting_left_and_right == true){
        shifting_left_and_right = false;
      }
      else if(shifting_left_and_right == false){
        shifting_left_and_right = true;
      }
    }
    //SETTING UP THE TIME
    //If the increasing value is pressed and we are on left side
    if(!(PINC & (1<<PINC1)) && (PINC | (0<<PINC0)) && (PINC | (0<<PINC2)) && (PINC | (0<<PINC3) ) && shifting_left_and_right == false ){
      _delay_ms(250);
      minute_left_digit += 1;
      if(minute_left_digit > 5){
        minute_left_digit = 0;
      }
    }
    //If the increasing value is pressed and we are on right side
    else if(!(PINC & (1<<PINC1)) && (PINC | (0<<PINC0)) && (PINC | (0<<PINC2)) && (PINC | (0<<PINC3) ) && shifting_left_and_right == true){
      _delay_ms(250);
      minute_right_digit += 1;
      if(minute_right_digit > 9){
        minute_right_digit = 0;
      }
    }
    

    
  }while(button_1_pressed != true);
  button_1_pressed = false;
 
  //Assign the minute value for global variables
  initial_minute_1 = minute_left_digit;
  initial_minute_2 = minute_right_digit;
  
  
}

// **********************************************SUPPORTIVE FUNCTION TO DISPLAY THE NUMBER************************************************************
void display_alarm_hour(){
  show_the_digit_left(alarm_hour_1);
  show_the_digit_right(alarm_hour_2);
}

void display_alarm_minute(){
  show_the_digit_left(alarm_minute_1);
  show_the_digit_right(alarm_minute_2);
}

void display_initial_hour(){
  show_the_digit_left(initial_hour_1);
  show_the_digit_right(initial_hour_2);
  PORTD |= (1<<1);
}

void display_initial_minute(){
  show_the_digit_left(initial_minute_1);
  show_the_digit_right(initial_minute_2);
  PORTD &= ~(1<<1);
}

void show_the_digit_left(int digit){
  if(digit == 0){
    display_number_0_left_digit();
  }
  else if(digit == 1){
    display_number_1_left_digit();
  }
  else if(digit == 2){
    display_number_2_left_digit();
  }
  else if(digit == 3){
    display_number_3_left_digit();
  }
  else if(digit == 4){
    display_number_4_left_digit();
  }
  else if(digit == 5){
    display_number_5_left_digit();
  }
  else if(digit == 6){
    display_number_6_left_digit();
  }
  else if(digit == 7){
    display_number_7_left_digit();
  }
  else if(digit == 8){
    display_number_8_left_digit();
  }
  else if(digit == 9){
    display_number_9_left_digit();
    
  }
  
}

void show_the_digit_right(int digit){
  if(digit == 0){
    display_number_0_right_digit();
  }
  else if(digit == 1){
    display_number_1_right_digit();
  }
  else if(digit == 2){
    display_number_2_right_digit();
  }
  else if(digit == 3){
    display_number_3_right_digit();
  }
  else if(digit == 4){
    display_number_4_right_digit();
  }
  else if(digit == 5){
    display_number_5_right_digit();
  }
  else if(digit == 6){
    display_number_6_right_digit();
  }
  else if(digit == 7){
    display_number_7_right_digit();
  }
  else if(digit == 8){
    display_number_8_right_digit();
  }
  else if(digit == 9){
    display_number_9_right_digit();
    
  }
  
}


void turn_both_digit_off(){
  turn_off_all_digit_left();
  turn_off_all_digit_right();
}

void turn_both_digit_on(){
  turn_on_all_digit_left();
  turn_on_all_digit_right();
}

void turn_off_all_digit_left(){

  PORTC &= ~(1<<4); //A
  PORTC &= ~(1<<5); //B
  PORTD &= ~(1<<2); //C
  PORTD &= ~(1<<3); //D
  PORTD &= ~(1<<4); //E
  PORTD &= ~(1<<5); //F
  PORTD &= ~(1<<6); //G

}

void turn_on_all_digit_left(){
  PORTC |= (1<<4); //A
  PORTC |= (1<<5); //B
  PORTD |= (1<<2); //C
  PORTD |= (1<<3); //D
  PORTD |= (1<<4); //E
  PORTD |= (1<<5); //F
  PORTD |= (1<<6); //G
}

void turn_off_all_digit_right(){
  PORTD &= ~(1<<7); //A
  PORTB &= ~(1<<0); //B
  PORTB &= ~(1<<1); //C
  PORTB &= ~(1<<2); //D
  PORTB &= ~(1<<3); //E
  PORTB &= ~(1<<4); //F
  PORTB &= ~(1<<5); //G
}

void turn_on_all_digit_right(){
  PORTD |= (1<<7); //A
  PORTB |= (1<<0); //B
  PORTB |= (1<<1); //C
  PORTB |= (1<<2); //D
  PORTB |= (1<<3); //E
  PORTB |= (1<<4); //F
  PORTB |= (1<<5); //G
}
//--------------------------------------------------------END--------------------------------------------------------------------------------
//********************************************************************************************************************************************
//------------------------------------------FUNCTION SECTION FOR FIRST 7-SEGMENT LED----------------------------------------------------------
void display_number_0_left_digit(){
  //Turn off 
  turn_off_all_digit_left();
  //Turn on
  PORTC |= (1<<4); //A
  PORTC |= (1<<5); //B
  PORTD |= (1<<2); //C
  PORTD |= (1<<3); //D
  PORTD |= (1<<4); //E
  PORTD |= (1<<5); //F
}

void display_number_1_left_digit(){
  //Turn off 
  turn_off_all_digit_left();
  //Turn on
  
  PORTC |= (1<<5); //B
  PORTD |= (1<<2); //C
  
}

void display_number_2_left_digit(){

  //Turn off 
  turn_off_all_digit_left();
  
  //Turn on
  PORTC |= (1<<4); //A
  PORTC |= (1<<5); //B
  PORTD |= (1<<3); //D
  PORTD |= (1<<4); //E
  PORTD |= (1<<6); //G
}

void display_number_3_left_digit(){
  
  //Turn off 
  turn_off_all_digit_left();
  
  //Turn on
  PORTC |= (1<<4); //A
  PORTC |= (1<<5); //B
  PORTD |= (1<<2); //C
  PORTD |= (1<<3); //D
  PORTD |= (1<<6); //G
}

void display_number_4_left_digit(){
    //Turn off 
  turn_off_all_digit_left();
  
  //Turn on
  PORTC |= (1<<5); //B
  PORTD |= (1<<2); //C
  PORTD |= (1<<5); //F
  PORTD |= (1<<6); //G
}

void display_number_5_left_digit(){
  //Turn off 
  turn_off_all_digit_left();
  
  //Turn on
  PORTC |= (1<<4); //A
  PORTD |= (1<<2); //C
  PORTD |= (1<<3); //D
  PORTD |= (1<<5); //F
  PORTD |= (1<<6); //G
}

void display_number_6_left_digit(){
  //Turn off 
  turn_off_all_digit_left();
  
  //Turn on
  PORTC |= (1<<4); //A
  PORTD |= (1<<2); //C
  PORTD |= (1<<3); //D
  PORTD |= (1<<4); //E
  PORTD |= (1<<5); //F
  PORTD |= (1<<6); //G
}

void display_number_7_left_digit(){
    //Turn off 
  turn_off_all_digit_left();
  
  //Turn on
  PORTC |= (1<<4); //A
  PORTC |= (1<<5); //B
  PORTD |= (1<<2); //C
}

void display_number_8_left_digit(){
  //Turn on
  PORTC |= (1<<4); //A
  PORTC |= (1<<5); //B
  PORTD |= (1<<2); //C
  PORTD |= (1<<3); //D
  PORTD |= (1<<4); //E
  PORTD |= (1<<5); //F
  PORTD |= (1<<6); //G
}

void display_number_9_left_digit(){
      //Turn off 
  turn_off_all_digit_left();
  
  //Turn on
  PORTC |= (1<<4); //A
  PORTC |= (1<<5); //B
  PORTD |= (1<<2); //C
  PORTD |= (1<<3); //D
  PORTD |= (1<<5); //F
  PORTD |= (1<<6); //G
}



//-------------------------------------------------------------------END-----------------------------------------------------------------------
//*********************************************************************************************************************************************
//------------------------------------------FUNCTION SECTION FOR SECOND 7-SEGMENT LED--------------------------------------------------------
void display_number_0_right_digit(){
  //Turn off
  turn_off_all_digit_right();

  //Turn on
  PORTD |= (1<<7); //A
  PORTB |= (1<<0); //B
  PORTB |= (1<<1); //C
  PORTB |= (1<<2); //D
  PORTB |= (1<<3); //E
  PORTB |= (1<<4); //F
  
  
  
}

void display_number_1_right_digit(){
  //Turn off
  turn_off_all_digit_right();

  //Turn on
  PORTB |= (1<<0); //B
  PORTB |= (1<<1); //C
  
}

void display_number_2_right_digit(){

  //Turn off
  turn_off_all_digit_right();

  //Turn on
  PORTD |= (1<<7); //A
  PORTB |= (1<<0); //B
  PORTB |= (1<<2); //D
  PORTB |= (1<<3); //E
  PORTB |= (1<<5); //G
  
}

void display_number_3_right_digit(){
   //Turn off
  turn_off_all_digit_right();

  //Turn on
  PORTD |= (1<<7); //A
  PORTB |= (1<<0); //B
  PORTB |= (1<<1); //C
  PORTB |= (1<<2); //D
  PORTB |= (1<<5); //G
}

void display_number_4_right_digit(){
   //Turn off
  turn_off_all_digit_right();

  //Turn on
  PORTB |= (1<<0); //B
  PORTB |= (1<<1); //C
  PORTB |= (1<<4); //F
  PORTB |= (1<<5); //G
}

void display_number_5_right_digit(){
  turn_off_all_digit_right();

  //Turn on
  PORTD |= (1<<7); //A
  PORTB |= (1<<1); //C
  PORTB |= (1<<2); //D
  PORTB |= (1<<4); //F
  PORTB |= (1<<5); //G
}

void display_number_6_right_digit(){
  //Turn off
  turn_off_all_digit_right();

  //Turn on
  PORTD |= (1<<7); //A
  PORTB |= (1<<1); //C
  PORTB |= (1<<2); //D
  PORTB |= (1<<3); //E
  PORTB |= (1<<4); //F
  PORTB |= (1<<5); //G
}

void display_number_7_right_digit(){
  //Turn off
  turn_off_all_digit_right();

  //Turn on
  PORTD |= (1<<7); //A
  PORTB |= (1<<0); //B
  PORTB |= (1<<1); //C
  
}

void display_number_8_right_digit(){
  turn_on_all_digit_right();
}

void display_number_9_right_digit(){
  //Turn off
  turn_off_all_digit_right();

  //Turn on
  PORTD |= (1<<7); //A
  PORTB |= (1<<0); //B
  PORTB |= (1<<1); //C
  PORTB |= (1<<2); //D
  PORTB |= (1<<4); //F
  PORTB |= (1<<5); //G
}

//-------------------------------------------------------------------END OF FILE----------------------------------------------------------------------
