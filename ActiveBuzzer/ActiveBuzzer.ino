/*
 * Active Buzzer
 * 
 * Create sounds using active buzzer.
 * 
 */

// PINS
#define BUZZER (12)

void setup() {
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // generate a square wave 
  // with 1 millisecond period.
  for (int i=0; i<80; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(1);
    digitalWrite(BUZZER, LOW);
    delay(1);
  }

  // generate a square wave 
  // with 2 milliseconds period.
  for (int i=0; i<80; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(2);
    digitalWrite(BUZZER, LOW);
    delay(2);
  }
}
