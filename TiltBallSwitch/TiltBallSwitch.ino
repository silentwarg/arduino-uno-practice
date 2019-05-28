/*
 * Tilt Ball Switch
 * 
 * Turn on/off LED using tilt ball switch.
 * 
 */

// PINS
#define LED_PIN         (13)
#define TILT_SWITCH_PIN (2)

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TILT_SWITCH_PIN, INPUT);
  digitalWrite(TILT_SWITCH_PIN, HIGH);
}

void loop() {
  if (digitalRead(TILT_SWITCH_PIN) == HIGH) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);
  }
}
