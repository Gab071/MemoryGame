#include "Controller.h"
#include "pitches.h"

template <size_t SeqLen>
Controller<SeqLen>::Controller(const int Leds[], const uint8_t Buttons[], uint8_t Buzzer, 
uint8_t numInputs, const int Tones[], LiquidCrystal &lcdRef) : ledPins(Leds), buttonPins(Buttons), 
buzzerPin(Buzzer), numInputs(numInputs), tones(Tones), lcd(lcdRef)
{
  mode = GameStart;
  currentRound = 0;
  randNum = -1;
  guessedInARow = 0;
  prevRandNum = -1;
}

template <size_t SeqLen>  
void Controller<SeqLen>::Begin()
{
  pinMode(buzzerPin, OUTPUT);
  for(int i = 0; i < numInputs; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

template <size_t SeqLen>
void Controller<SeqLen>::Update()
{
  static bool PressedWrongButton;
  if(mode == GameStart)
  {
    // Changing matrix values to have less errors,
    // because value 0 is used to index matrixes Leds[] and Buttons[]
    for(int i=0; i<SeqLen; i++)
    {
      Sequence[i] = -1;
    }
    randNum = -1;
    prevRandNum = -1;
    guessedInARow = 0;
    currentRound = 0;
    PressedWrongButton = false;
    lcd.clear();

    mode = GameContinues;
  }


  if(mode == GameContinues)
  {
    GuessNextElement();
    Sequence[currentRound] = randNum;
    currentRound++;

    ShowCurrentSequence();
    TypeInCurrentSequence(PressedWrongButton);
    // heart of the program, checks if all the rounds were guessed correctly 
    if(currentRound == SeqLen && !PressedWrongButton) 
    {
      mode = GameOver;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You've won!");
      PlayVictoryTone();
    }
    delay(1000);
  }

  if(mode == GameOver)
  {
    RestartGame();
  }
}

template <size_t SeqLen>
void Controller<SeqLen>::GuessNextElement()
{
  randNum = random(numInputs); 
  if(randNum == prevRandNum)
  {
    guessedInARow++;

    if(guessedInARow > 3)
    {
      while(randNum == prevRandNum)
      {
        randNum = random(numInputs);
      }
      guessedInARow = 0;
    }
  }
  else
  {
    guessedInARow = 0;
  }
  
  prevRandNum = randNum;
}

template <size_t SeqLen>
void Controller<SeqLen>::ShowCurrentSequence()
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

template <size_t SeqLen>
void Controller<SeqLen>::TypeInCurrentSequence(bool &PressedWrongButton)
{
  int progress = 0;
  ShowProgressOnLCD(progress);
  while(progress < currentRound && mode == GameContinues)
  {
    ReadStatesOfTheButtons(progress, PressedWrongButton);

    if(mode == GameOver) break;
  }

  if(mode == GameContinues)
  {
      PlayCoinSound();
  }
}

template <size_t SeqLen>
void Controller<SeqLen>::ReadStatesOfTheButtons(int &progress, bool &PressedWrongButton)
{
  for(int sw = 0; sw < numInputs; sw++)
    {
      if(digitalRead(buttonPins[sw]) == LOW)
      {
        PlayGameTone(sw);
        if(sw == Sequence[progress])
        {
          progress++;
          ShowProgressOnLCD(progress);
        }
        else
        {
          mode = GameOver;
          PressedWrongButton = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Game Over");
          PlayErrorTone();
        }
      }
      if(mode == GameOver) break;
    }
}

template <size_t SeqLen>
void Controller<SeqLen>::ShowProgressOnLCD(int progress)
{
  lcd.setCursor(0,1);
  lcd.print("Progress: ");
  lcd.print(progress);
  lcd.print("/");
  lcd.print(currentRound);
}

template <size_t SeqLen>
void Controller<SeqLen>::PlayGameTone(int index) 
{
  if (index >= 0 && index < numInputs) 
  {
    digitalWrite(ledPins[index], HIGH);
    tone(buzzerPin, tones[index], 250); 
    delay(300);

    digitalWrite(ledPins[index], LOW);                        
    noTone(buzzerPin);
    delay(50);
  }
}

template <size_t SeqLen>
void Controller<SeqLen>::PlayGameTones()
{
  for(int i = 0; i < numInputs; i++)
  {
    tone(buzzerPin, tones[i], 250);
    delay(300);
  }
  noTone(buzzerPin);
}

template <size_t SeqLen>
void Controller<SeqLen>::PlayErrorTone() 
{
  for(int i=0; i < 4; i++)
  {
    ChangeStateOfDiodes(HIGH);
    tone(buzzerPin, NOTE_C2, 200);
    delay(200);

    ChangeStateOfDiodes(LOW);
    tone(buzzerPin, NOTE_G2, 280);
    delay(280);
  }
  noTone(buzzerPin);
}

template <size_t SeqLen>
void Controller<SeqLen>::PlayCoinSound() 
{
  ChangeStateOfDiodes(HIGH);
  tone(buzzerPin, NOTE_B5, 100);
  delay(100); 

  tone(buzzerPin, NOTE_E6, 400);
  delay(400);

  ChangeStateOfDiodes(LOW);
  noTone(buzzerPin);
}

template <size_t SeqLen>
void Controller<SeqLen>::PlayVictoryTone() 
{
  int winNotes[] = { NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_G5 };
  int winDurations[] = { 120, 120, 120, 250, 120, 400 };

  ChangeStateOfDiodes(HIGH); 

  for (int i = 0; i < 6; i++) 
  {
    tone(buzzerPin, winNotes[i], winDurations[i] - 20); 
    delay(winDurations[i]);
    noTone(buzzerPin);
    delay(20);
  }

  ChangeStateOfDiodes(LOW); 
}

template <size_t SeqLen>
void Controller<SeqLen>::ChangeStateOfDiodes(bool state)
{
  for(int led=0; led<numInputs; led++)
  {
    digitalWrite(ledPins[led], state);
  }
}

template <size_t SeqLen>
void Controller<SeqLen>::RestartGame()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press blue swi-"); // I know that switch has 1 syllable 
  lcd.setCursor(0, 1);
  lcd.print("tch to restart");
  while(mode == GameOver)
  {
    if(digitalRead(buttonPins[0]) == LOW)
    {
      mode = GameStart;
      while(digitalRead(buttonPins[0]) == LOW){delay(10);}
    }
  }
} 
