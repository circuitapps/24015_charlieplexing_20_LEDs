
/*
  Controlling 20 LEDs using 5 output pins on AtTiny85 microcontroller via charlieplexing

  Operation: Demonstration of how 20 LEDs can be controlled individually using only 5 IO pins on AtTiny85 microcontroller.
  
  LEDs are assumed to be organized as as 4 x 5 matrix as shown below. Oxy is an LED where x = OUT_x (positive voltage pin) y = OUT_y (GND pin)
  as per OUT_ pin definitions in the code.

      0     1     2     3     4
  0   O12   O13   O23   O34   O35
  1   O21   O31   O32   O43   O53
  2   O14   O15   O24   O25   O45
  3   O41   O51   O42   O52   O54

  by circuitapps
  January 2024
*/
#include <avr/io.h>

#define NUM_LED_ROWS 4
#define NUM_LED_COLS 5

// PORT B DEFINITIONS
#define PB5_PIN1 PB5
#define PB4_PIN3 PB4
#define PB3_PIN2 PB3
#define PB2_PIN7 PB2
#define PB1_PIN6 PB1
#define PB0_PIN5 PB0

// Application pin definitions used in the code for simplicity
#define OUT_1 PB2_PIN7
#define OUT_2 PB1_PIN6
#define OUT_3 PB0_PIN5
#define OUT_4 PB3_PIN2
#define OUT_5 PB4_PIN3

// LED pin HIGH/LOW row address definitions
#define O12 0
#define O13 1
#define O14 2
#define O15 3
#define O21 4
#define O31 5
#define O41 6
#define O51 7

#define O23 8
#define O24 9
#define O25 10
#define O32 11
#define O42 12
#define O52 13

#define O34 14
#define O35 15
#define O43 16
#define O53 17

#define O45 18
#define O54 19

#define HIGH_PIN 0
#define LOW_PIN 1

// 20 x 2 matrix. Col 0 is the pin that needs to go HIGH, Col 1 is the pin that needs to go LOW for lighting up the LED assigned to the row index.
unsigned char *Oxy_pin_matrix[] = {
                                      (unsigned char[]){OUT_1, OUT_2},
                                      (unsigned char[]){OUT_1, OUT_3},
                                      (unsigned char[]){OUT_1, OUT_4},
                                      (unsigned char[]){OUT_1, OUT_5},
                                      (unsigned char[]){OUT_2, OUT_1},
                                      (unsigned char[]){OUT_3, OUT_1},
                                      (unsigned char[]){OUT_4, OUT_1},
                                      (unsigned char[]){OUT_5, OUT_1},

                                      (unsigned char[]){OUT_2, OUT_3},
                                      (unsigned char[]){OUT_2, OUT_4},
                                      (unsigned char[]){OUT_2, OUT_5},
                                      (unsigned char[]){OUT_3, OUT_2},
                                      (unsigned char[]){OUT_4, OUT_2},
                                      (unsigned char[]){OUT_5, OUT_2},

                                      (unsigned char[]){OUT_3, OUT_4},
                                      (unsigned char[]){OUT_3, OUT_5},
                                      (unsigned char[]){OUT_4, OUT_3},
                                      (unsigned char[]){OUT_5, OUT_3},

                                      (unsigned char[]){OUT_4, OUT_5},
                                      (unsigned char[]){OUT_5, OUT_4}
};

// Following is the LED matrix map definition
unsigned char led_matrix[NUM_LED_ROWS][NUM_LED_COLS] = {
                                                          {O12, O13, O23, O34, O35},
                                                          {O21, O31, O32, O43, O53},
                                                          {O14, O15, O24, O25, O45},
                                                          {O41, O51, O42, O52, O54}
};


void led_on(unsigned char led_row, unsigned char led_col)
{
  unsigned char led_id = led_matrix[led_row][led_col];  // get the LED ID in the layout matrix

  unsigned char high_pin = Oxy_pin_matrix[led_id][HIGH_PIN];
  unsigned char low_pin = Oxy_pin_matrix[led_id][LOW_PIN];

  pinMode(high_pin, OUTPUT);
  pinMode(low_pin, OUTPUT);

  digitalWrite(high_pin, HIGH);
  digitalWrite(low_pin, LOW);
}

void led_off(unsigned char led_row, unsigned char led_col)
{
  unsigned char led_id = led_matrix[led_row][led_col];  // get the LED ID in the layout matrix

  unsigned char high_pin = Oxy_pin_matrix[led_id][HIGH_PIN];
  unsigned char low_pin = Oxy_pin_matrix[led_id][LOW_PIN];

  // Disconnect pins to turn them off!
  pinMode(high_pin, INPUT);
  pinMode(low_pin, INPUT);
}

//////////////////////////////////////////////////////////////////

void   basic_test()
{
  for(int i = 0; i < NUM_LED_ROWS ; ++i)
  {
    for(int j = 0; j < NUM_LED_COLS ; ++j)
    {
      led_on(i,j);
      delay(100);
      led_off(i,j);
    }
    
  }

}


void column_scan()
{

  for(int j = 0; j < NUM_LED_COLS ; ++j)
  {
    for(int i = 0; i < NUM_LED_ROWS ; ++i)
    {
      led_on(i,j);
      delay(20);
      led_off(i,j);
    }
    delay(100);
  }

}

void row_scan()
{

  for(int i = 0; i < NUM_LED_ROWS ; ++i)
  {
    for(int j = 0; j < NUM_LED_COLS ; ++j)
    {
      led_on(i,j);
      delay(20);
      led_off(i,j);
    }
    delay(100);
  }

}

void row_snake()
{
  #define MOVE_RIGHT 0
  #define MOVE_LEFT 1

  int current_row = 0;
  int current_col = 0;
  unsigned char current_dir = MOVE_RIGHT;

  while(current_row < NUM_LED_ROWS)
  {
    
    if( current_dir == MOVE_RIGHT)
    {
      while(current_col < NUM_LED_COLS)
      {
        led_on(current_row, current_col);
        delay(50);
        led_off(current_row, current_col);
        ++current_col;
      }
      current_col = NUM_LED_COLS - 1;  // For next LED
    }
    else
    {// Moving left
      while(current_col >= 0)
      {
        led_on(current_row,current_col);
        delay(50);
        led_off(current_row,current_col);
        --current_col;
      }
      current_col = 0;  // For next LED
    }

    current_dir = ~current_dir;  // toggle move direction
    ++current_row;
  }

}

void column_snake()
{
  #define MOVE_UP 0
  #define MOVE_DOWN 1

  int current_row = 0;
  int current_col = 0;
  unsigned char current_dir = MOVE_DOWN;

  while(current_col < NUM_LED_COLS)
  {
    
    if( current_dir == MOVE_DOWN)
    {
      while(current_row < NUM_LED_ROWS)
      {
        led_on(current_row, current_col);
        delay(50);
        led_off(current_row, current_col);
        ++current_row;
      }
      current_row = NUM_LED_ROWS - 1;
    }
    else
    {// Moving up
      while(current_row >= 0)
      {
        led_on(current_row, current_col);
        delay(50);
        led_off(current_row, current_col);
        --current_row;
      }
      current_row = 0;
    }

    current_dir = ~current_dir;  // toggle move direction
    ++current_col;
  }

}

void column_knight_rider()
{
  #define MOVE_RIGHT 0
  #define MOVE_LEFT 1

  int current_row = 0;
  int current_col = 0;
  unsigned char current_dir = MOVE_RIGHT;

  int num_loops = 4;  // repeating the feature four times in a row

  while(num_loops)
  {
    if(current_dir == MOVE_RIGHT)
    {
      while(current_col < NUM_LED_COLS)
      {
        for(int i = 0; i < NUM_LED_ROWS ; ++i )
        {
          led_on(i, current_col);
          delay(20);
          led_off(i, current_col);
        }
        delay(50);
        ++current_col;
      }
      current_col = NUM_LED_COLS - 1;  // For moving to left

    }
    else
    {
      while(current_col >= 0)
      {
        for(int i = 0; i < NUM_LED_ROWS ; ++i )
        {
          led_on(i, current_col);
          delay(1);
          led_off(i, current_col);
        }
        delay(50);
        --current_col;
      }
      current_col = 0;  // For moving to right

    }
    
    current_dir = ~current_dir;  // toggle move direction
    --num_loops;
  }

}


void row_knight_rider()
{
  #define MOVE_DOWN 0
  #define MOVE_UP 1

  int current_row = 0;
  int current_col = 0;
  unsigned char current_dir = MOVE_DOWN;

  int num_loops = 4;  // repeating the feature four times in a row

  while(num_loops)
  {
    if(current_dir == MOVE_DOWN)
    {
      while(current_row < NUM_LED_ROWS)
      {
        for(int j = 0; j < NUM_LED_COLS ; ++j )
        {
          led_on(current_row, j);
          delay(20);
          led_off(current_row, j);
        }
        delay(50);
        ++current_row;
      }
      current_row = NUM_LED_ROWS - 1;  // For moving up

    }
    else
    {
      while(current_row >= 0)
      {
        for(int j = 0; j < NUM_LED_COLS ; ++j )
        {
          led_on(current_row, j);
          delay(1);
          led_off(current_row, j);
        }
        delay(50);
        --current_row;
      }
      current_row = 0;  // For moving down

    }
    
    current_dir = ~current_dir;  // toggle move direction
    --num_loops;
  }

}

void random_blips()
{
  unsigned char rand_col, rand_row, rand_delay, rand_sustain;
  unsigned char repetitions = 20;
  
  while(repetitions)
  {
    rand_col = random(0, NUM_LED_COLS);
    rand_row = random(0, NUM_LED_ROWS);
    rand_delay = random(20, 50);
    rand_sustain = random(100, 200);

    led_on(rand_row, rand_col);
    delay(rand_delay);
    led_off(rand_row, rand_col);

    --repetitions;
    delay(rand_sustain);
  }

}

void perimeter_scan()
{
  #define WAIT_TIME_MS 30
  int i, j;

  // Move across Row = 0
  i = 0;

  for(j = 0; j < NUM_LED_COLS ; ++j)
  {
    led_on(i,j);
    delay(WAIT_TIME_MS);
    led_off(i,j);
  }    

  // Move down the last column
  j = NUM_LED_COLS - 1;
  for(i = 0; i < NUM_LED_ROWS ; ++i)
  {
    led_on(i,j);
    delay(WAIT_TIME_MS);
    led_off(i,j);
   }    

  // Move left along last row
  i = NUM_LED_ROWS - 1;

  for(j = NUM_LED_COLS - 1; j >= 0 ; --j)
  {
    led_on(i,j);
    delay(WAIT_TIME_MS);
    led_off(i,j);
  } 

  // Move up the first column
  j = 0;
  for(i = NUM_LED_ROWS - 1; i >= 0 ; --i)
  {
    led_on(i,j);
    delay(WAIT_TIME_MS);
    led_off(i,j);
   }   
}

// the setup function runs once when you press reset or power the board
void setup()
{
  // Initially, all pins are defined as input to detach them from the LED network.
  pinMode(OUT_1, INPUT);
  pinMode(OUT_2, INPUT);
  pinMode(OUT_3, INPUT);
  pinMode(OUT_4, INPUT);
  pinMode(OUT_5, INPUT);
}

// the loop function runs over and over again forever

void loop()
{
  
  // Basic test: Light each LED, move right and down gradually.
  basic_test();
  
  // Feature 1: Lighting each column of LEDs (one at a time)
  column_scan();

  // Feature 2: Lighting each row of LEDs (one at a time)
  row_scan();

  // Feature 3: One LED traverses each row starting from top moving to the bottom.
  row_snake();

  // Feature 4: One LED traverses each column starting from top moving to the bottom.
  column_snake();

  // Feature 5: Each column of LEDs moves from left to right and back.
  column_knight_rider();

  // Feature 6: Each row of LEDs moves from top to bottom and back.
  row_knight_rider();

  // Feature 7: Random LEDs lighting up in the matrix at random intervals.
  random_blips();

  // Feature 8: Perimeter scan. LEDs circling the perimeter LEDs only
  perimeter_scan();

  // Feature 9: Down the stairs. LED moving down the stairs.
  //down_the_stairs();

  // Feature 10: Up the stairs. LED moving up the stairs.
  //up_the_stairs();

  // Feature 11: Curtains close. LEDs light up from the edges moving to the center
  // curtains_closing();

  // Feature 12: Curtains open. LEDs light up from the center moving to the edges
  // curtains_opening();

  // Feature 13: Each column represents the amplitude of audio within a frequency range.
  //vumeter_simulator();

  // Feature 14: Display the word "HELLO" one letter at a time.
  //display_hello();

}
