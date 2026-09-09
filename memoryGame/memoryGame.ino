#include <Arduino.h>
#include <LiquidCrystal.h>
#include "Controller.h"
#include "pitches.h"

// Hardware & Pin mapping
const int LED_PINS[]    = {A0, A1, 3, 2}; // Blue, Red, Yellow, Green
const uint8_t BUTTON_PINS[] = {13, 12, 11, 4};    // Blue, Red, Yellow, Green
const uint8_t BUZZER_PIN    = A2;
const int GAME_TONES[]     = { NOTE_C3, NOTE_E3, NOTE_G3, NOTE_C4 };
const int numInputs = 4;
const int SequenceLength = 6;

LiquidCrystal lcd(10, 9, 8, 7, 6, 5);

Controller<SequenceLength> game(LED_PINS, BUTTON_PINS, BUZZER_PIN, numInputs, GAME_TONES, lcd);

void setup() 
{
  Serial.begin(9600); // for debugging purposes
  lcd.begin(16, 2);

  randomSeed(analogRead(A3));

  game.Begin();
}

void loop() 
{
  game.Update();
}





