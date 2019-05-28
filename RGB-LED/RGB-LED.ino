/*
 * RGB LED
 * 
 * Use RGB LED and create colors.
 * 
 */

// PINS
#define BLUE  (3)
#define GREEN (5)
#define RED   (6)

// Fade period
#define FADE_PERIOD (10)

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

void loop() {
  int red_value = 255;
  int green_value = 0;
  int blue_value = 0;

  // Fade from red to green
  for (int i=0; i<255; i++) {
    red_value -= 1;
    green_value += 1;
    analogWrite(RED, red_value);
    analogWrite(GREEN, green_value);
    delay(FADE_PERIOD);
  }

  // Fade from green to blue
  for (int i=0; i<255; i++) {
    green_value -= 1;
    blue_value += 1;
    analogWrite(GREEN, green_value);
    analogWrite(BLUE, blue_value);
    delay(FADE_PERIOD);
  }

  // Fade from blue to red
  for (int i=0; i<255; i++) {
    blue_value -= 1;
    red_value += 1;
    analogWrite(BLUE, blue_value);
    analogWrite(RED, red_value);
    delay(FADE_PERIOD);
  }
}
