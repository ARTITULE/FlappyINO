#ifndef FLAPPYBIRD_GAME
#define FLAPPYBIRD_GAME

#include <Arduino.h>
#include <GyverOLED.h>
#include <EncButton.h>
#include "config.h"

void FBirdGame(int8_t EEPROM_ADDR, int8_t Difficulty, int8_t setting1, int8_t setting2, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &display, Button &main_button);

#endif // FLAPPYBIRD_GAME