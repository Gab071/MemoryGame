#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

enum GameMode {GameStart, GameContinues, GameOver};

template <size_t SeqLen>
class Controller {
  // Matrix responsible for keeping current state of the game
  int Sequence[SeqLen];

  // Hardware & Pin Mapping
  const int *ledPins;
  const uint8_t *buttonPins;
  const uint8_t buzzerPin;
  LiquidCrystal &lcd;

  // Game constants
  const int sequenceLength;
  const int *tones;
  const int numInputs; // amount of: LED's || Buttons || Tones

  // Game state
  GameMode mode;
  int currentRound; // Round determines how much more player needs to press buttons in a round
  int randNum; // Variable to add more moves to the sequence after winning the round
  int guessedInARow; 
  int prevRandNum;

  /// Function that uses function random() to guess next element (number) for the sequence that
  /// will be played. If same number is guessed 4 times in a row function guesses new number until
  /// it is different from previous guessed number
  void GuessNextElement();

  /// Function that just shows current sequence on LED's and 
  /// displays current round on LCD 
  void ShowCurrentSequence();

  /// Function that is responsible for checking if correct sequence is being guessed
  /// after succsesfull sequence coin sound is played
  void TypeInCurrentSequence(bool &PressedWrongButton);

  /// Function that reads states of the buttons and ends the game if incorrect button
  /// is pressed. Also displays current progress on LCD
  void ReadStatesOfTheButtons(int &progress, bool &PressedWrongButton);

  /// Function that shows current round progress on LCD
  void ShowProgressOnLCD(int progress);
  
  /// Function is responsible for playing game tones on buzzer and
  /// turning on and off LED's
  void PlayGameTone(int index);

  /// Function is responsible for playing 4 tones in sequence
  /// Not used in the code (used for debuggin purposes)
  void PlayGameTones();

  /// Function is responsible for playing error tone on buzzer and
  /// Turning on and off all 4 LED's 
  void PlayErrorTone();

  /// Plays after succsesfull guess of the current sequence
  void PlayCoinSound();
  
  // Function that plays a victory tone if player won all rounds 
  void PlayVictoryTone();   

  // Function like digitalWrite() but for all 4 LED's at the same time
  void ChangeStateOfDiodes(bool state);

  // If button next to blue LED is pressed, game restarts
  void RestartGame();

  public:

  Controller(const int Leds[], const uint8_t Buttons[], uint8_t Buzzer, 
  uint8_t numInputs, const int Tones[], LiquidCrystal &lcdRef);
  
  void Begin();
  void Update();
};





#endif