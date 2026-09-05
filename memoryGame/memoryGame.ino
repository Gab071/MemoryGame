#include <LiquidCrystal.h>
#include "pitches.h"

#define LedBlue 12
#define LedRed 11
#define LedYellow 10
#define LedGreen 9
#define Buzzer 8
#define SwBlue 7
#define SwRed 6
#define SwYellow 5
#define SwGreen 4

#define GameStart 100
#define GameContinues 1
#define GameOver 0

// Pins connected to the LCD
LiquidCrystal lcd(13, 3, A0, A1, A2, 2);

void GuessCurrentSequence(int currentRound, int Sequence[]);
void ReadStatesOfTheButtons(int &progress, int Sequence[], int currentRound);
void ShowCurrentSequence(int countcurrentRound, int Sequence[]);
int GuessNextElement(int &prevRandNum, int &guessedInARow);
void PlayGameTone(int index);
void PlayGameTones();
void PlayErrorTone();
void PlayCoinSound();
void ChangeStateOfDiodes(bool state);
void RestartGame();
void ShowProgressOnLCD(int currentRound, int progress);
void PlayVictoryTone(); 


bool PressedWrongButton = false;
int GameMode = GameStart; 

const int Tones[] = { NOTE_C3, NOTE_E3, NOTE_G3, NOTE_C4 };
const int SequenceLength = 6; // That many rounds person needs to guess to win
const int LedAmount = 4;

const int Leds[]    = { LedBlue, LedRed, LedYellow, LedGreen };
const int Buttons[] = { SwBlue, SwRed, SwYellow, SwGreen };


void setup() {
  Serial.begin(9600); // for debugging purposes
  lcd.begin(16, 2);

  randomSeed(analogRead(0));

  pinMode(Buzzer, OUTPUT);
  
  pinMode(LedBlue, OUTPUT);
  pinMode(LedRed, OUTPUT);
  pinMode(LedYellow, OUTPUT);
  pinMode(LedGreen, OUTPUT);

  pinMode(SwBlue, INPUT_PULLUP);
  pinMode(SwRed, INPUT_PULLUP);
  pinMode(SwYellow, INPUT_PULLUP);
  pinMode(SwGreen, INPUT_PULLUP);

}

void loop() {

  static int randNumber = -1;
  static int prevRandNum = -1;
  static int guessedInARow = 0;
  static int currentRound = 0; // Round determines how much more player needs to press buttons in a round
  static int Sequence[SequenceLength] = {0};
  if(GameMode == GameStart)
  {
    // Changing matrix values to have less errors,
    // because value 0 is used to index matrixes Leds[] and Buttons[]
    for(int i=0; i<SequenceLength; i++)
    {
      Sequence[i] = -1;
    }
    randNumber = -1;
    prevRandNum = -1;
    guessedInARow = 0;
    currentRound = 0;
    PressedWrongButton = false;
    lcd.clear();

    GameMode = GameContinues;
  }


  if(GameMode == GameContinues)
  {
    randNumber = GuessNextElement(prevRandNum, guessedInARow);
    Sequence[currentRound] = randNumber;
    currentRound++;

    ShowCurrentSequence(currentRound, Sequence);
    GuessCurrentSequence(currentRound, Sequence);
    // heart of the program, checks if all the rounds were guessed correctly 
    if(currentRound == SequenceLength && !PressedWrongButton) 
    {
      GameMode = GameOver;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You've won!");
      PlayVictoryTone();
    }
    delay(1000);
  }

  if(GameMode == GameOver)
  {
    RestartGame();
  }
}

// Function that is responsible for checking if correct sequence is being guessed
// after succsesfull sequence coin sound is played
void GuessCurrentSequence(int currentRound, int Sequence[])
{
  int progress = 0;
  bool guessedIncorectly = false;
  ShowProgressOnLCD(currentRound, progress);
  while(progress < currentRound && GameMode == GameContinues)
  {
    ReadStatesOfTheButtons(progress, Sequence, currentRound);

    if(GameMode == GameOver) break;
  }

  if(GameMode == GameContinues)
  {
      PlayCoinSound();
  }
}

// Function that reads states of the buttons and ends the game if incorrect button
// is pressed. Also displays current progress on LCD
void ReadStatesOfTheButtons(int &progress, int Sequence[], int currentRound)
{
  for(int sw = 0; sw < LedAmount; sw++)
    {
      if(digitalRead(Buttons[sw]) == LOW)
      {
        PlayGameTone(sw);
        if(sw == Sequence[progress])
        {
          progress++;
          ShowProgressOnLCD(currentRound, progress);
        }
        else
        {
          GameMode = GameOver;
          PressedWrongButton = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Game Over");
          PlayErrorTone();
        }
      }
      if(GameMode == GameOver) break;
    }
}

// Function that just shows current sequence on LED's and 
// displays current round on LCD 
void ShowCurrentSequence(int currentRound, int Sequence[])
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Round: ");
  lcd.print(currentRound);
  lcd.setCursor(0, 1);
  lcd.print("Watch...");
  for(int i=0; i<currentRound; i++)
  {
    PlayGameTone(Sequence[i]);
  }
}

// Function that uses function random() to guess next element (number) for the sequence that
// will be played. If same number is guessed 4 times in a row function guesses new number until
// it is different from previous guessed number
int GuessNextElement(int &prevRandNum, int &guessedInARow)
{
  int randNumber = 0; // Variable to add more moves to the sequence after correct guess
  randNumber = random(4); // Returns number from 0 to 3 (excluding 4)
  if(randNumber == prevRandNum)
  {
    guessedInARow++;

    if(guessedInARow > 3)
    {
      while(randNumber == prevRandNum)
      {
        randNumber = random(4);
      }
      guessedInARow = 0;
    }
  }
  else
  {
    guessedInARow = 0;
  }
  
  prevRandNum = randNumber;
  
  return randNumber;
}

// Function is responsible for playing game tones on buzzer and
// turning on and off LED's
void PlayGameTone(int index) 
{
  if (index >= 0 && index < 4) 
  {
    digitalWrite(Leds[index], HIGH);
    tone(Buzzer, Tones[index], 250); 
    delay(300);

    digitalWrite(Leds[index], LOW);                        
    noTone(Buzzer);
    delay(50);
  }
}

// Function is responsible for playing 4 tones in sequence
// Not used in the code (used for debuggin purposes)
void PlayGameTones(){
  for(int i = 0; i < 4; i++)
  {
    tone(Buzzer, Tones[i], 250);
    delay(300);
  }
  noTone(Buzzer);
}

// Function is responsible for playing error tone on buzzer and
// Turning on and off all 4 LED's
void PlayErrorTone() 
{
  for(int i=0; i < 4; i++)
  {
    ChangeStateOfDiodes(HIGH);
    tone(Buzzer, NOTE_C2, 200);
    delay(200);

    ChangeStateOfDiodes(LOW);
    tone(Buzzer, NOTE_G2, 280);
    delay(280);
  }
  noTone(Buzzer);
}

// Plays after succsesfull guess of the current sequence
void PlayCoinSound() 
{
  ChangeStateOfDiodes(HIGH);
  tone(Buzzer, NOTE_B5, 100);
  delay(100); 

  tone(Buzzer, NOTE_E6, 400);
  delay(400);

  ChangeStateOfDiodes(LOW);
  noTone(Buzzer);
}

// Function like digitalWrite() but for all 4 LED's at the same time
void ChangeStateOfDiodes(bool state)
{
  for(int led=0; led<LedAmount; led++)
  {
    digitalWrite(Leds[led], state);
  }
}

// If button next to blue LED is pressed, game restarts
void RestartGame()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press blue swi-"); // I know that switch has 1 syllable 
  lcd.setCursor(0, 1);
  lcd.print("tch to restart");
  while(GameMode == GameOver)
  {
    if(digitalRead(SwBlue) == LOW)
    {
      GameMode = GameStart;
      while(digitalRead(SwBlue)==LOW){}
    }
  }
} 

// Function that shows current round progress on LCD
void ShowProgressOnLCD(int currentRound, int progress)
{
  lcd.setCursor(0,1);
  lcd.print("Progress: ");
  lcd.print(progress);
  lcd.print("/");
  lcd.print(currentRound);
}

// Function that plays a victory tone if player won all rounds 
void PlayVictoryTone() 
{
  int winNotes[] = { NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_G5 };
  int winDurations[] = { 120, 120, 120, 250, 120, 400 };

  ChangeStateOfDiodes(HIGH); 

  for (int i = 0; i < 6; i++) 
  {
    tone(Buzzer, winNotes[i], winDurations[i] - 20); 
    delay(winDurations[i]);
    noTone(Buzzer);
    delay(20);
  }

  ChangeStateOfDiodes(LOW); 
}




