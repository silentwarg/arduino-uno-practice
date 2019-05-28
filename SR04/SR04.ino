/*
 * Ultrasonic Sensor
 * 
 * Using HC-SR04 ultrasonic sensor to measure distance.
 * 
 */

#include <hcsr04.h>

#define ECHO_PIN    (11)
#define TRIGGER_PIN (12)

hcsr04_t hcsr04;
long distance;

void setup() {
  hcsr04_init(&hcsr04, ECHO_PIN, TRIGGER_PIN);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  distance = hcsr04_distance(&hcsr04);
  Serial.print(distance);
  Serial.println("cm");
  delay(1000);
}
