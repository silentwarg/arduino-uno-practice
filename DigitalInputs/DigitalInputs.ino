/*
 * Digital Inputs
 * 
 * Use push switch as a digital input to 
 * turn on and turn off the LED.
 * 
 */

// PINS
#define LED_PIN     (5)
#define BUTTON1_PIN (8)
#define BUTTON2_PIN (9)

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BUTTON1_PIN) == LOW) {
    digitalWrite(LED_PIN, HIGH);
  }
  if (digitalRead(BUTTON2_PIN) == LOW) {
    digitalWrite(LED_PIN, LOW);
  }
}
