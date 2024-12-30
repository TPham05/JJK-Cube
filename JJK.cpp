
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_CENTER 375
#define SERVORANGE 100

 #define SERVO_MIN (SERVO_CENTER - SERVORANGE)
 #define SERVO_MAX (SERVO_CENTER + SERVORANGE)
 #define SERVO_FREQ 60

 int xPulse[5] = {SERVO_CENTER,SERVO_CENTER,SERVO_CENTER,SERVO_CENTER,SERVO_CENTER};
 int yPulse[5] = {SERVO_CENTER,SERVO_CENTER,SERVO_CENTER,SERVO_CENTER,SERVO_CENTER};

 int targetX[5], targetY[5], speed[5], waitTime[5];


 int step[5] = {0,0,0,0,0};
 unsigned long lastUpdate[5] = {0,0,0,0,0};
 bool moving[5] = {true,true,true,true,true};

 void setup (){
  Serial.begin(9600);
  Serial.println("Random Eye Movement");
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  randomSeed(analogRead(0) + analogRead(1) + analogRead(2));

  for(int i = 0; i < 5 ;i++){
    targetX[i] = random(SERVO_MIN, SERVO_MAX);
    targetY[i] = random(SERVO_MIN, SERVO_MAX);
    speed[i] = random(2,6);
    waitTime[i] = random(50, 1500);
  }

 }
void moveEye(int index,int &xPulse, int &yPulse, int targetX, int targetY, int speed){
    for(int i = 0 ; i <= 100; i++){
      int currentXPulse = map(i,0,100,xPulse, targetX);
      int currentYPulse = map(i,0,100,yPulse, targetY);
      pwm.setPWM(index * 2, 0 , currentXPulse);
      pwm.setPWM(index*2 +1 , 0 , currentYPulse);
      delay(15/speed);
    }

    xPulse = targetX;
    yPulse = targetY;
  }

 void loop(){
    unsigned long currentMillis = millis();
    for(int i =0 ; i <5 ;i++){
      if(currentMillis - lastUpdate[i] >= waitTime[i]){
        if(moving[i]){
          moveEye(i, xPulse[i], yPulse[i], targetX[i], targetY[i], speed[i]);
          moving[i] = false;
        } else{
          targetX[i] = random(SERVO_MIN, SERVO_MAX);
          targetY[i] = random(SERVO_MIN, SERVO_MAX);
          speed[i] = random(2, 6);
          waitTime[i] = random(50, 1500);
          moving[i] = true;
        }
        lastUpdate[i] = currentMillis;
      }
    }
 }
