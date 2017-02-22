#include "AFMotor.h"

AF_DCMotor MOTOR_1 (3, MOTOR12_64KHZ); // define motor on channel 3 with 64KHz PWM
AF_DCMotor MOTOR_2 (4, MOTOR12_64KHZ); // define motor on channel 4 with 64KHz PWM

const int TRIG = A0;  // trig pin of Ultrasonic Sensor
const int ECHO = A1;  // echo pin of Ultrasonic Sensor

void setup() {
  
  Serial.begin(9600);
  MOTOR_1.setSpeed(255);  // set the max speed for motor 1
  MOTOR_2.setSpeed(255);  // set the max speed for motor 2
  pinMode(TRIG, OUTPUT);  // trig sends the signal
  pinMode(ECHO, INPUT);   // echo receives the signal
}

int stuck = 0;  // variable to check if the robot cannot scan the object

void loop() {
  unsigned long duration; // store variable for the calculation of time
  int distance;           // store variable for the distance

  /* send signal from trig */
  digitalWrite(TRIG,0); // turn off pin trig
  delay(2);
  digitalWrite(TRIG,1); // send signal
  delay(5);
  digitalWrite(TRIG,0); // turn off pin trig

  /* calculate the time and distance */
  duration = pulseIn(ECHO,HIGH);      // calculate the length of the pulse
  distance = int(duration/2/29.412);  // calculate the distance
  Serial.print(distance);Serial.println("cm");

  /* robot runs backward and turn right
     if the robot is near object */
  if (distance < 10)
  {
    MOTOR_1.run(BACKWARD);  // motor 1 run backward
    MOTOR_2.run(BACKWARD);  // motor 2 run backward
    delay(1000);            // run backward in 1 second
    MOTOR_1.run(BACKWARD);  
    MOTOR_2.run(FORWARD);   // motor 2 run forward
    delay(500);
    stuck = 0;              // reset the stuck variable 
  }
  /* robot run forward or be stopped 
     until stuck reaches 800 */
  else
  {
    MOTOR_1.run(FORWARD);
    MOTOR_2.run(FORWARD);
    stuck++;
    /* robot runs backward and turn left */
    if (stuck == 800)
    {
      MOTOR_1.run(BACKWARD);
      MOTOR_2.run(BACKWARD);
      delay(1000);
      MOTOR_1.run(FORWARD);
      MOTOR_2.run(BACKWARD);
      delay(500);
      stuck = 0;
    }
  }
}
