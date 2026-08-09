
#include "functions.h"
#include "EEPROM.h"
#include "bitmap.h"


void printCentered(const char* text, uint8_t row, uint8_t scale, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {
  uint8_t textWidth = strlen(text) * 6 * scale;
  int x = (128 - textWidth) / 2;
  if (x < 0) x = 0;
 
  oled.setScale(scale);
  oled.setCursorXY(x, row);
  oled.print(text);
}


void WaitingScreen(GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {
    oled.clear();
    oled.setScale(4);
    oled.setCursor(54, 3);
    oled.print("3");
    oled.update();
    delay(400);

    oled.clear();
    oled.setCursor(54, 3);
    oled.print("2");
    oled.update();
    delay(400);

    oled.clear();
    oled.setCursor(54, 3);
    oled.print("1");
    oled.update();
    delay(400);

    oled.clear();
    oled.setCursor(54, 3);
    oled.print("0");
    oled.update();
}

void batCheckDraw(GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {
    static uint32_t measureTimer = millis() + 3500;
    static uint8_t batCharge = 0;

    if (millis() - measureTimer >= 3000) {
        measureTimer = millis();
        ADCSRA |= 1 << ADSC;
        while (ADCSRA & (1 << ADSC))
            ;
        batCharge = constrain(map((INTERNAL_REF * 1024UL) / ADC, BATTERY_EMPTY, BATTERY_FULL, 0, 12), 0, 12);
    }

    oled.setCursorXY(110, 2);
    oled.drawByte(0b00111100);
    oled.drawByte(0b00111100);
    oled.drawByte(0b11111111);
    for (uint8_t i = 0; i < 12; i++) {
        if (i < 12 - batCharge)
            oled.drawByte(0b10000001);
        else
            oled.drawByte(0b11111111);
    }
    oled.drawByte(0b11111111);
}

void drawMainMenuCard(int8_t EEPROM_ADDR, const char* planet, const char* difficulty, const uint8_t* bitmap, bool drawHold, byte dimensionIndex, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {

    int8_t rectDimensions[3][2] = {
        { 3, 19 },
        { 23, 39 },
        { 43, 59 }
    };
    oled.clear();
    oled.drawBitmap(55, 0, bitmap__Background, 64, 64);
    oled.setCursor(55, 1);
    oled.setScale(2);
    oled.print(planet);
    oled.setScale(1);
    oled.setCursor(55, 3);
    oled.print(difficulty);
    oled.setScale(1);
    oled.setCursor(55,5);
    oled.print("Reach :");
    oled.setCursor(103, 5);
    oled.print(EEPROM.read(EEPROM_ADDR));
    if (drawHold) {
        printCentered("Hold to descend", 54, 1, oled);
    }
    oled.roundRect(0, 0, 127, 63, OLED_STROKE);
    oled.clear(0, 0, 1, 63);
    oled.drawBitmap(0, 0, bitmap_verticalPattern, 8, 64);
    oled.dot(0, 0);
    oled.dot(1, 0);
    oled.dot(1, 63);
    oled.drawBitmap(4, 4, bitmap, 48, 48);
    oled.rect(0, rectDimensions[dimensionIndex][0], 1, rectDimensions[dimensionIndex][1], 1);
    oled.update();

}