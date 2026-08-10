#ifndef FUNCTIONS
#define FUNCTIONS

#include <Arduino.h>
#include <GyverOLED.h>
#include "config.h"

void printCentered(const char* text, uint8_t row, uint8_t scale, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled);
void WaitingScreen(GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled);
void batCheckDraw(GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled);
void drawMainMenuCard(int8_t EEPROM_ADDR, const char* planet, const char* difficulty, const uint8_t* bitmap, bool drawHold, byte dimensionIndex, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled);
void drawSettingBox(int8_t row, const char* settingName, int8_t setting, const char* opt1, const char* opt2, const char* opt3, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled);

#endif