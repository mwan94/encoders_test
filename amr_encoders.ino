/*
  Created by: Amr Wa
              CSULA

  Project: Raytheon Radar Guided Rescue Robot 

  Created on: 02/26/2018
  
  Task: Read data from the encoders on the Hercules Motor Controller Library using interrupts

        Left Motor Encoder is attached to digital pin  2  --> Interrupt pin is 0
        Right Motor Encoder is attached to digital pin 3  --> Interrupt pin is 1
        For more info, refer to: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  */
  
#include <Hercules.h>
#define encoder_L 2  //defining the left encoder digital pin
#define encoder_R 3  //defining the right encoder digital pin


volatile int rps_L = 0;                                   //use volatile to store the variable directly to RAM instead of memory register, useuful for time-sensitive situations
unsigned int rpm_L = 0;                                   //variable to store rpm value

volatile int rps_R = 0;                                   //use volatile to store the variable directly to RAM instead of memory register, useuful for time-sensitive situations
unsigned int rpm_R = 0;                                   //variable to store rpm value


double radius = 0.0425;                                 //variable to store the radius of the Hercules platform wheels

double temp_L = 0;                                      //variable to store the calculated speed in meter per minute for left wheels
double calcSpeed_L = 0;                                 //variable to store the calculated speed in cm per seconds for left wheels

double temp_R = 0;                                      //variable to store the calculated speed in meter per minute for left wheels
double calcSpeed_R = 0;                                 //variable to store the calculated speed in cm per seconds for left wheels


double prev_time = 0;                                   //variable to store time
double pi = 3.1415;


void setup(){

 MOTOR.begin();                                                 //Initialize DC Motors
 Serial.begin(57600);                                           //Establish and set the communication baud rate between computer and the Hercules Motor Controller
 pinMode(encoder_L, INPUT_PULLUP);                              //enable internal pull-up resistor since the encoder has a Hall sensor on it, Hall sensor is an open-collector device meaning that the output current will be sunk
 pinMode(encoder_R, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(2), rps_countL, RISING);    //Call the fucntion when an interrupt occurs for the left encoder
 attachInterrupt(digitalPinToInterrupt(3), rps_countR, RISING);    //Call the fucntion when an interrupt occurs for the right encoder

}

void loop(){

 forward_movement();                                            //Call the function named 'forward_movement', see the function below - it is for movement

 
  
 if (millis() - prev_time == 1000){  //if loop to update everyone second, milis() will give us time since the program started running. 
 
 detachInterrupt(digitalPinToInterrupt(2));    //Disable interrupt for the left encoder when calculating to avoid any interruptions
 detachInterrupt(digitalPinToInterrupt(3));    //Disable interrupt for the left encoder when calculating to avoid any interruptions


 rpm_L = (rps_L * 30);  // Converting Revs per second to RPM. For two pulses per second, we will multiply by
 rpm_R = (rps_R * 30);  // Converting Revs per second to RPM. For two pulses per second, we will multiply by

 temp_L = rpm_L * (2 * pi * radius);    //calculate speed in meter per minute
 calcSpeed_L = (temp_L * 1.66);       // conver the speed in meter per minute to cm per seconds, we multiple by 1.66 {[ (1 meter/ 1 minute) * (1 minute / 60 second) * (100 cm / 1 meter)] = 1.66 } 
 
 temp_R = rpm_R * (2 * pi * radius);    //calculate speed in meter per minute
 calcSpeed_R = (temp_R * 1.66);       // conver the speed in meter per minute to cm per seconds, we multiple by 1.66 {[ (1 meter/ 1 minute) * (1 minute / 60 second) * (100 cm / 1 meter)] = 1.66 } 
  
 Serial.print("Left Wheel RPM = "); 
 Serial.print(rpm_L); 
 Serial.print("\t Left Wheel RPS= ");
 Serial.print(rps_L);
 Serial.print("\t Left Wheel Speed= "); 
 Serial.print(calcSpeed_L);
 Serial.println(" cm/s");
 Serial.println();
 Serial.print("Right Wheel RPM = "); 
 Serial.print(rpm_R); 
 Serial.print("\t Right Wheel RPS= ");
 Serial.print(rps_R);
 Serial.print("\t Right Wheel Speed= "); 
 Serial.print(calcSpeed_R);
 Serial.println(" cm/s");

 
 reset(); // call the reset function
  
 attachInterrupt(digitalPinToInterrupt(2), rps_countL, RISING); //enable interrupt again for left encoder
 attachInterrupt(digitalPinToInterrupt(3), rps_countR, RISING); //enable interrupt again for left encoder

  }
}


void rps_countL(){ // this code will be executed every time an interrupt occurs - left encoder
  rps_L++;       // increment Revs/second
}

void rps_countR(){ // this code will be executed every time an interrupt occurs - right encoder
  rps_R++;       // increment Revs/second
}

void reset(){   //when this function is called, the below lines will be executed -- see below
 
  rps_L = 0;                           // rest the Rev per second counter to 0 
  rps_R = 0;
  prev_time = millis();                // Uptade prev_time  to be used for next iteration of the loop function
}

void forward_movement(){ //a function I created in order to be called in the main program
  
  MOTOR.setSpeedDir1(50, DIRF);    //Move left side wheels forward at speed of PWM = 30%
  MOTOR.setSpeedDir2(10, DIRF);    //Move right side wheels forward at speed of PWM = 30%

}




