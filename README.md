# Memory Game

## Table of Contents

1. [Overview](#overview)
2. [Hardware Components](#hardware-components)
3. [Pin Mapping](#pin-mapping)
4. [How to Run](#how-to-run)
5. [Additional Notes](#additional-notes)

## Overview

I wanted to create an interesting game by using Arduino board programmed in C++ (via Arduino IDE). The final result was a memory game also known as "Simon says". How to play: 
1. **Start the Game**: Once the code is running and all components are connected, the LCD display will prompt you to start (press the blue switch).
2. **Watch and Listen**: Game will generate a random sequence of blinking LEDs accompanied by buzzer tones.
3. **Repeat the Pattern**: Repeat the exact sequence by pressing the corresponding buttons in the correct order.
4. **Another Round** Each successful attempt adds one new step to the sequence. 
5. **Victory or Game Over**: 
* **Victory**: Completing all rounds displays a victory message on LCD accompanied by a winning melody.

* **Game Over**: Pressing an incorrect button immediately causes in a loss accompanied by a message on LCD and losing melody.

To repeat the game press the switch again.   
    

## Hardware Components

| Component | Quantity | Comments |
| ---- | ----- | ----- |
| Arduino Uno R3 | 1 | It is possible to use different Arduino board |
| LCD HD44780 | 1 | To display instructions |
| Red LED | 1 | - |
| Blue LED | 1 | - |
| Yellow LED | 1 | - |
| Green LED | 1 | - |
| Buzzer | 1 | without an internal oscilator (without generator) |
| Buttons | 4 | - |
| Resistor $1\ \mathsf{k\Omega}$ | 4 | Limiting current for LEDs |
| Potentiometer $5\ \mathsf{k\Omega}$ | 1 | To control LCD's contrast |
| Breadboard | 1 | - |
| Jumper wires | - | - |


## Pin Mapping 

The LCD pins are listed in physical order (from left to right on the display module). 

| LCD pin | Arduino pin or power supply | Comments |
|----|-----|-----|
|1. VSS/GND | GND | - |
|2. VDD | +5V | - |
|3. V0 | Potentiometer | Connect to the wiper (middle pin) of the potentiometer | 
|4. RS | 10 | Arduino pin |
|5. RW | GND | - |
|6. E | 9 | Arduino pin |
|7-10. | - | Not connected |
|11. D4 | 8 | Arduino pin | 
|12. D5 | 7 | Arduino pin |
|13. D6 | 6 | Arduino pin |
|14. D7 | 5 | Arduino pin |
|15. A | +5V | - |
|16. K | GND | - |

If you are using an LCD module other than the HD44780, make sure to check whether pin A (backlight anode) requires an external current-limiting resistor.

| Component | Arduino pin | Comments |
| ---- | ----- | ----- |
| Blue LED | A0 | - |
| Red LED | A1 | - |
| Yellow LED | A2 | - |
| Green LED | 3 | - |
| (Blue) Button | 13 | button next to the blue LED |
| (Red) Button | 12 | button next to the red LED |
| (Yellow) Button | 11 | button next to the yellow LED |
| (Green) Button | 4 | button next to the green LED |
| Buzzer | 2 | button responsible for the start of the reaction test |

Note: Remember to connect power (5V and GND) to the outer pins of the potentiometer so that it functions properly as a voltage divider for the LCD contrast. Since all digital pins were in use, three of the LEDs were connected to analog input pins (ADC pins).

## How to Run

### Step 1

Install and launch an IDE capable of compiling and uploading Arduino code (e.g., [Arduino IDE](https://www.arduino.cc/en/software/)).

### Step 2 

Connect all components according to the pin mapping above. 

### Step 3 

Connect your board to the PC. Upload and run the code. 

## Additional Notes

* It is possible to play more rounds than currently is set. You just have to change the value called *Sequence Length*:

```
const int SequenceLength = 6;
```

You can change it in the main file memoryGame.ino line 12. The minimum value that you have to set for program to work correctly is 1 and max is 99. 
