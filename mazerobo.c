
#include <Servo.h>

// Pin Definitions
#define LEFT_MOTOR_FORWARD 3
#define LEFT_MOTOR_BACKWARD 4
#define RIGHT_MOTOR_FORWARD 5
#define RIGHT_MOTOR_BACKWARD 6
#define IR_SENSOR_LEFT A0
#define IR_SENSOR_CENTER A1
#define IR_SENSOR_RIGHT A2
#define TRIG_PIN 7
#define ECHO_PIN 8
#define SERVO_PIN 9

Servo myServo;
int pathHistory[100][2]; // Array to store coordinates of the path
int pathIndex = 0;

void setup() {
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  pinMode(IR_SENSOR_CENTER, INPUT);
  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Initial position of servo
  Serial.begin(9600);
}

void loop() {
  if (detectObstacle()) {
    avoidObstacle();
  } else {
    followLine();
  }
}

void followLine() {
  int leftSensorValue = digitalRead(IR_SENSOR_LEFT);
  int centerSensorValue = digitalRead(IR_SENSOR_CENTER);
  int rightSensorValue = digitalRead(IR_SENSOR_RIGHT);
  logPath();

  if ((centerSensorValue == HIGH)&&(leftSensorValue == HIGH)&&(rightSensorValue == LOW)) {
    turnLeft();//straight or go left
  }
  if ((centerSensorValue == LOW)&&(leftSensorValue == HIGH)&&(rightSensorValue == LOW)) {
    turnLeft();//left l
  }
  if ((centerSensorValue == LOW)&&(leftSensorValue == HIGH)&&(rightSensorValue == HIGH)) {
   turnLeft();//t junc
  }
  if ((centerSensorValue == HIGH)&&(leftSensorValue == HIGH)&&(rightSensorValue == HIGH)) {
    turnLeft(){
    if((centerSensorValue == HIGH)&&(leftSensorValue == HIGH)&&(rightSensorValue == HIGH))
    {retrieveTreasure();
    returnToStart();}
    else moveForward();
  }
  if ((centerSensorValue == HIGH)&&(leftSensorValue == LOW)&&(rightSensorValue == HIGH)) {
    moveForward();//straight or right
  }
  if ((centerSensorValue == HIGH)&&(leftSensorValue == LOW)&&(rightSensorValue == LOW)) {
    moveForward();//straight
  }
  if ((centerSensorValue == LOW)&&(leftSensorValue == LOW)&&(rightSensorValue == HIGH)) {
    turnRight();//right l junc
  }
  } else {
    turnLeft();//dead end
  }
}

void moveForward() {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
}

void turnRight() {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
}

void turnLeft() {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
}

void avoidObstacle() {
  stopMotors();
  delay(500);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
  delay(500);
  stopMotors();
  
  turnLeft();
  delay(500);
}

bool detectObstacle() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.034) / 2;
  
  return distance < 20;
}

void retrieveTreasure() {
  myServo.write(90);
  delay(1000);
  myServo.write(0);
}

void logPath() {
  pathHistory[pathIndex][0] = 0;
  pathHistory[pathIndex][1] = 0;
  pathIndex++;
  if (pathIndex >= 100) pathIndex = 0;
}

void returnToStart() {
  for (int i = pathIndex - 1; i >= 0; i--) {
  }
  stopMotors();
}