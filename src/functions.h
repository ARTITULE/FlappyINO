#ifndef FUNCTIONS
#define FUNCTIONS

#include <Arduino.h>
#include <GyverOLED.h>
#include "config.h"

void WaitingScreen(GyverOLED <SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled);
void batCheckDraw(GyverOLED <SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled);


#endif