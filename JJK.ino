#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_CENTER 375
#define SERVORANGE 75

#define SERVO_MIN (SERVO_CENTER - SERVORANGE)
#define SERVO_MAX (SERVO_CENTER + SERVORANGE)
#define SERVO_FREQ 60

int xPulse[5] = {SERVO_CENTER, SERVO_CENTER, SERVO_CENTER, SERVO_CENTER, SERVO_CENTER};
int yPulse[5] = {SERVO_CENTER, SERVO_CENTER, SERVO_CENTER, SERVO_CENTER, SERVO_CENTER};

int targetX[5], targetY[5], speed[5], waitTime[5];
int step[5] = {0, 0, 0, 0, 0};
unsigned long lastUpdate[5] = {0, 0, 0, 0, 0};
unsigned long lastMove[5] = {0, 0, 0, 0, 0};
bool moving[5] = {true, true, true, true, true};

void setup() {
  Serial.begin(9600);
  Serial.println("Random Eye Movement");
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  randomSeed(analogRead(0) + analogRead(1) + analogRead(2));

  for (int i = 0; i < 5; i++) {
    targetX[i] = random(SERVO_MIN, SERVO_MAX);
    targetY[i] = random(SERVO_MIN, SERVO_MAX);
    speed[i] = random(2, 6);
    waitTime[i] = random(50, 1500);
  }
}

void moveEye(int index) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastMove[index] >= (15 / speed[index])) {
    if (xPulse[index] != targetX[index]) {
      xPulse[index] += (xPulse[index] < targetX[index]) ? 1 : -1;
      pwm.setPWM(index * 2, 0, xPulse[index]);
    }
    if (yPulse[index] != targetY[index]) {
      yPulse[index] += (yPulse[index] < targetY[index]) ? 1 : -1;
      pwm.setPWM(index * 2 + 1, 0, yPulse[index]);
    }

    lastMove[index] = currentMillis;

    if (xPulse[index] == targetX[index] && yPulse[index] == targetY[index]) {
      moving[index] = false;
    }
  }
}

void loop() {
  unsigned long currentMillis = millis();
  for (int i = 0; i < 5; i++) {
    if (moving[i]) {
      moveEye(i);
    } else if (currentMillis - lastUpdate[i] >= waitTime[i]) {
      targetX[i] = random(SERVO_MIN, SERVO_MAX);
      targetY[i] = random(SERVO_MIN, SERVO_MAX);
      speed[i] = random(2, 6);
      waitTime[i] = random(50, 1500);
      moving[i] = true;
      lastUpdate[i] = currentMillis;
    }
  }
}
