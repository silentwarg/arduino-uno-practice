/*
 * Passive Buzzer
 * 
 * Create sounds using passive buzzer
 * by generating square waves with different frequencies.
 * 
 */

#include "pitches.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// PINS
#define BUZZER (8)

int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, 
  NOTE_F5, NOTE_G5, NOTE_A5, 
  NOTE_B5, NOTE_B6  
};
int duration = 500;

void setup() {
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  for (int i=0; i<ARRAY_SIZE(melody); i++) {
    // Output a different note every 0.5 seconds
    tone(BUZZER, melody[i], duration);
    delay(1000);
  }
  delay(2000);
}
