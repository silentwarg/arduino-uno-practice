/*
 * Servo
 * 
 * Control servo motor using the arduino's servo library.
 * 
 */

#include <Servo.h>

// PINS
#define SERVO_PIN (9)

// Create a servo object to control the servo
Servo servo;
int position = 0;

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
}

void loop() {
  for (position=0; position <= 180; position++) {
    servo.write(position);
    delay(10);
  }
  for (position = 180; position >= 0; position--) {
    servo.write(position);
    delay(10);
  }
}
