/*maximum value 255
*(SPEED,SPEED) moves forward
(-SPEED, -SPEED) moves backward
(-SPEED, SPEED) rotation to left, or if inversed, rotation to right
*/

#include <QTRSensors.h>

#define NUM_SENSORS   2     // number of sensors used
#define TIMEOUT       1500  // waits for 1500 microseconds for sensor outputs to go low
#define MOTOR2_PIN1  3
#define MOTOR2_PIN2  5
#define MOTOR1_PIN1  6
#define MOTOR1_PIN2  9

// color sensors are connected to pin 7 digital and 8 digital
QTRSensorsRC qtrrc((unsigned char[]) {7, 8},NUM_SENSORS, TIMEOUT);
unsigned int sensorValues[NUM_SENSORS];
unsigned int SPEED;
void setup() {
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  delay(5000);
  //for debugging only, sending values to PC
  //Serial.begin(9600);
}

void loop() {
  // read raw sensor color values
  qtrrc.read(sensorValues);
  
  /* read distance sensor: 
   *    - object situated at 100 cm, it gives values 110-120
   *    - for a closer object (50 cm) will give 250-260
   *    - most close object touching the robot 540-560
   *    - the ring i think has like 1 meter diameter
  */    
  int objectDistance = analogRead(0);
  //Serial.println(distance);
   
  /*
   * Check if we are in the safe zone, sensors on black or dark color give 2500,
   * For white area usually drops under 500
   * sensorValues[0] - values of right color sensor
   * sensorValues[1] - values of left color sensor
  */
  if (sensorValues[0] > 800 && sensorValues[1] > 800){
    //in the safe zone if no threat is detected rotate in place with moderate speed 
      if (objectDistance < 250) {
      SPEED = 60; 
      go(-SPEED,SPEED);
    }
    //in the safe zone, if the adversary is closer, accelerate to full speed
    //noticed that when accelaration jumps with more than 100 units the robo falls back
    if (objectDistance > 250 && objectDistance < 400) {
      stop();
      SPEED = 200;
      go(SPEED,SPEED); 
      delay(1000); 
    }
    if (objectDistance > 400) {
      SPEED = 255;
      go(SPEED,SPEED);
      delay(1000);    
    }  
  }
  //if we are in the danger zone go back 1 second and rotate 1 second
  else {
     stop();
     SPEED = 180;
     go(-SPEED,-SPEED);
     delay(1000);
     go(SPEED,-SPEED);
     delay(1000);  
  }  
}

void go(int speedLeft, int speedRight) {
  if (speedLeft > 0) {
    analogWrite(MOTOR1_PIN1, speedLeft);
    analogWrite(MOTOR1_PIN2, 0);
  } 
  else {
    analogWrite(MOTOR1_PIN1, 0);
    analogWrite(MOTOR1_PIN2, -speedLeft);
  }
 
  if (speedRight > 0) {
    analogWrite(MOTOR2_PIN1, speedRight);
    analogWrite(MOTOR2_PIN2, 0);
  }else {
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, -speedRight);
  }
}
void stop(){
  go(0,0);
  delay(100);
}

