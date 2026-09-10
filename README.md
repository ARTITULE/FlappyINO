# Arduino Flappy Bird Console

This project is a simple handheld Flappy Bird–inspired game built using an Arduino(Atmega328p Chip) and a 7-pin SPI display. It recreates the classic side-scrolling obstacle-avoidance gameplay in a compact embedded system.

There is also a PCB based version that is smaller and more power efficient enabling you to play on the go.

Building the PCB version is advised as it is the smaller version and it uses the 3D printed case so that you can play on the go.

See the build video:

[![Video Title](https://img.youtube.com/vi/vfzLBbyyz0U/maxresdefault.jpg)](https://youtu.be/vfzLBbyyz0U)

## Version 2 Release

V2 adds a lot of new features like:

- A redesigned case
- A game progression system
- A settings menu
- Better UI, etc.

All the new additions are marked **New in V2**

## Features

- Smooth side-scrolling graphics on a 7-pin SPI display
- Single-button control
- Game and level progression system (**New in V2**)
- Landing and takeoff animation (**New in V2**)
- Settings menu to adjust visual settings. Extra elements like animations, backgrounds and star filled skies can be toggled off to increase performance (**New in V2**)
- Stylish UI (**New in V2**)
- Small form factor futuristic case for easy play on the go
- Up to a 36 hours of play time on a single charge (with a 300mah battery)
- On screen battery indicator
- Spot for a keychain ring to carry on the go (**New in V2**)
- Lightweight game loop optimized for microcontrollers
- Fully open-source hardware and software
- For more features see [Game Mechanics](#game-mechanics)

## Game Mechanics

- There are three difficulties: Steady, Rush, Frenzy (Easy, Medium, Hard). (**New in V2**)
- Each press of the button makes the bird flap upward.
- Gravity continuously pulls the bird downward.
- Collision with the ground, ceiling, or a pipe ends the game.
- Time your flaps to pass between pipes, each pipe is worth 1 point. But be careful the more points you get the harder the game becomes.
- All high scores for all the difficulties are saved in EEPROM and do not reset.
- Upon reaching 100 points you clear the difficulty. (**New in V2**)
- After clearing the difficulty you have the ability to play the **ENDLESS MODE**. Try to get as far as possible with no mistakes while the game is trying its best to make you fail. (**New in V2**)
- Harder difficulties are locked by default, clear previous difficulties to unlock further ones. (**New in V2**)

## Hardware Requirements

If you want to try out a non PCB version

- **Arduino board** (Uno, Nano, or any ATmega328P-based board)
- **7-pin SPI display**
- **Push button** for player input
- **Breadboard or custom PCB**
- **Power source** (USB or battery)

## PCB

![Pcb_Screenshot](Pic/Pcb_Screenshot.png)

### PCB Hardware Requirements

- **Atmega328P Chip**
- **A 16Mh SMD Crystal Oscillator** (If you are using a Atmega328P that is clocked at 16Mh)
- **6mmx6mm SMD Pushbutton**
- **TP4056 Charging module** (you should change the ISET resitor to a 10K one)
- **A Small LI-Po Battery** (Under 40mmx20mmx6mm )
- **A Small 2 Position 6 Pin Switch**
- **3 0402 100nf Ceramic Capasitors** (Not actually needed just improve stability)
- **2 0402 10k Resistors** (Also not technically needed)

### PCB Schematic

For anyone interested here is the schematic.

![Pcb_Schematic](Pic/Schematic.svg)

### Typical Display Pinout

| Display Pin | Description     | Connects To        |
|-------------|-----------------|--------------------|
| VCC         | Power           | 5V or              |
| GND         | Ground          | GND                |
| SCL / SCK   | SPI Clock       | D13 (SCK)          |
| SDA / MOSI  | SPI Data        | D11 (MOSI)         |
| RES         | Reset           | D8                 |
| DC          | Data/Command    | D9                 |
| CS          | Chip Select     | D10                |

### Button Pinout

| PushButton Pin | Connects To  |
|----------------|--------------|
| Pin 1          | GND          |
| Pin 2          | D3           |

## 3D Printed Case

The PCB is made to be used with a 3D printed case. The case files can be found in the '3D Files' folder.

![3D_Printed_Case](Pic/Fusion360_Image.png)

The case is made to be assembled with 4 M3 Heat-set inserts. A battery under 40mm in length, 19mm in width and 5mm in height is preferred.

## Software Requirements

- Visual Studio Code with PlatformIO installed.(All the necessary libraries are going to be automatically downloaded by PIO upon compilation)

## Installation

### Disclaimer: This guide presumes that you have installed and configured the PlatformIO extension for VS Code

1. Clone or download this repository.
2. Open the folder in VS Code.
3. Select the environment as "nano328new" if you are using a arduino board. If you are trying to upload to the pcb select the "pcbusbasp" environment(You will need the UsbASP itself and the driver if you are on Windows).
4. Compile and flash the firmware by clicking the "PlatformIO: Upload" button.

### Disclaimer: Use the guide below if you want to flash the outdated ArduinoIDE version

1. Clone or download this repository.
2. Drag the display library and the button library into the Libraries folder of the ArduinoIDE.
3. Open any one of the  `.ino` files.
4. Adjust pin definitions to match your wiring if you need to.
5. Upload the sketch to your Arduino.
6. For the PCB you are going to need the USB-ASP and the drivers
7. If you are using a custom Atmega328p Core then you can run the microcontroller with 8Mh internal clock then the 16Mh crystal is not needed.

## Useful Information

1. For the PCB version the microcontroller clocked at 16Mh might not want to work with the small voltage when the battery is discharged. In that case running it at a 8Mh clock is advised.

## Contributing

Pull requests are welcome.
