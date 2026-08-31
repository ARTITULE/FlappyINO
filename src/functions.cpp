
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

void drawMainMenuCard(int8_t EEPROM_ADDR, const char* planet, const char* difficulty, const uint8_t* bitmap, bool drawHold, bool isLocked, byte dimensionIndex, int16_t gameCompleteScore, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {

    int8_t rectDimensions[3][2] = {
        { 3, 19 },
        { 23, 39 },
        { 43, 59 }
    };
    int16_t bestScore;
    static int8_t firstPass;
    firstPass ++;
    if (firstPass >= 120) {
        firstPass = 0;
    }
    EEPROM.get(EEPROM_ADDR, bestScore);
    oled.clear();
    oled.drawBitmap(55, 0, bitmap__Background, 64, 64);
    oled.setCursor(55, 1);
    oled.setScale(2);
    oled.print(planet);
    oled.setScale(1);
    oled.setCursor(55, 3);
    oled.print(difficulty);
    if (bestScore >= gameCompleteScore) {
        oled.drawBitmap(108, 23, bitmap_Trophy, 16, 16);
    }
    else if (bestScore < gameCompleteScore && isLocked != 1) {
        oled.drawBitmap(108, 23, bitmap_lockOpen, 16, 16);
    }
    else {
        oled.drawBitmap(108, 23, bitmap_lockClosed, 16, 16);
    }
    oled.setCursor(55,5);
    oled.print("Reach :");
    oled.setCursor(103, 5);
    oled.print(bestScore);
    if (drawHold && isLocked != 1) {
        printCentered("Hold to descend", 54, 1, oled);
    }
    else if (isLocked) {
        if (firstPass < 40) {
            printCentered("Locked", 54, 1, oled);
        }
        else if (firstPass > 40 && firstPass < 80) {
            printCentered("Clear prior levels", 54, 1, oled);
        }
        else if (firstPass > 80) {
            printCentered("To unlock", 54, 1, oled);
        }
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

void drawSettingBox(int8_t row, const char* settingName, int8_t setting, const char* opt1, const char* opt2, const char* opt3, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {

    oled.setCursor(4, row);
    oled.print(settingName);
    oled.setCursor((strlen(settingName) + 1) * 6 + 4, row);
    if (setting == 0) {
        oled.print(opt1);
    }
    else if (setting == 1) {
        oled.print(opt2);
    }
    else if (setting == 2) {
        oled.print(opt3);
    }
    
}

void showDifficultyClearScreen(const char* difficulty, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {

    oled.clear();

    printCentered(difficulty, 1 * 8, 1, oled);
    printCentered("CLEAR!", 3 * 8, 3, oled);
    oled.line(8, 47, 119, 47);
    printCentered("PRESS TO CONTINUE", 7 * 8, 1, oled);

}

void showInfoScreen(const char* difficulty, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {

    oled.clear();
    oled.drawBitmap(0, 0, bitmap__Background, 64, 64);
    oled.drawBitmap(63, 0, bitmap__Background, 64, 64);
    printCentered("You have cleared the", 0 * 8, 1, oled);
    printCentered(difficulty, 1 * 8, 1, oled);
    printCentered("Would you like to", 2 * 8, 1, oled);
    printCentered("EXIT", 4 * 8, 1, oled);
    printCentered("or continue with", 5 * 8 + 4, 1, oled);
    printCentered("ENDLESS MODE", 7 * 8, 1, oled);
}

void landingAnimation(bool isLanding, GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {
        
    int16_t shipPos = isLanding ? 0 : 63;
    float offset = 0;
    float totalOffset = isLanding ? 0 : 1000;
    int16_t groundPos = isLanding ? 104 : 48;
    float offsetAmount = isLanding ? 5 : 0.5;
    int8_t frameCounter = 0;
    int8_t shakeX = 0;
    int8_t shakeY = 0;
    int8_t groundShakeX = 0;
    int8_t groundShakeY = 0;
    bool animationNotDone = true;
    bool showFire = !isLanding;
    bool addShake = !isLanding;

    while(1) {
        
        if (animationNotDone) {
            static uint32_t offsetTimer = millis();

            if (millis() - offsetTimer >= 20) {
   
                offsetTimer = millis();
                
                static uint32_t fireTimer = millis();
                if (isLanding) {
                    if (millis() - fireTimer >= 400 && totalOffset >= 550 && offsetAmount > 1) {
                        fireTimer = millis();
                        showFire = !showFire;
                    }
                    else if (offsetAmount <= 1 && animationNotDone == true) {
                        showFire = true;
                    }
                    else if (offsetAmount <= 1 && animationNotDone == false) {
                        showFire = false;
                    }
                }
                else {
                    showFire = true;
                }


                if (isLanding) {
                    if (totalOffset <= 600) {
                        offsetAmount = 5;
                    }
                    else if (totalOffset > 600 && totalOffset <= 800) {
                        offsetAmount = 3;
                    }
                    else if (totalOffset > 950 && totalOffset <= 980) {
                        offsetAmount = 1;
                        addShake = true;
                    }
                    else if (totalOffset > 995) {
                        offsetAmount = 0.5;
                        addShake = true;
                    }


                    if (shipPos < 63) {
                        shipPos++;
                    }


                    if (offset > -62) {
                        offset -= offsetAmount;
                        totalOffset += offsetAmount;
                    }
                    else if (offset <= -62) {
                        offset = 0;
                    }   
                    if (totalOffset >= 1000) {
                        groundPos--;
                        if (groundPos <= 63 - 15) {
                            animationNotDone = false;
                        }
                    } 
                }

                else {
                    if (totalOffset > 995) {
                        offsetAmount = 0.5;
                        addShake = true;
                    }
                    else if (totalOffset > 965 && totalOffset <= 980) {
                        offsetAmount = 1;
                        addShake = true;
                    }
                    else if (totalOffset > 600 && totalOffset <= 800) {
                        offsetAmount = 3;
                        addShake = false;
                    }
                    else if (totalOffset <= 600) {
                        offsetAmount = 5;
                        addShake = false;
                    }



                    if (shipPos > 0 && totalOffset <= 63 * 5) {
                        shipPos--; 
                        if (shipPos <= 0) {
                            animationNotDone = false;
                        }
                    }



                    if (offset < 0) {
                        offset += offsetAmount;
                        totalOffset -= offsetAmount;
                    }
                    else if (offset >= -0) {
                        offset = -62;
                    }   
                    if (totalOffset >= 0) {
                        groundPos++;

                    } 

                }

        
            }
        }


        static uint32_t drawTimer = millis();
        if (millis() - drawTimer >= (1000 / MENU_FPS)) {

            drawTimer = millis();
            oled.clear();
            if (addShake && animationNotDone) {
                if (!isLanding) {
                    groundShakeX = random(0, 2);
                    groundShakeY = random(0, 2);
                }
                shakeX = random(0, 2);
                shakeY = random(0, 2);                
            }

            for (uint8_t i = 0; i < NUM_STARS; i++) {
                uint8_t sPosX = pgm_read_byte(&StarX[i]);
                uint8_t sPosY = pgm_read_byte(&StarY[i]);
                uint8_t y = (sPosY + offset < 0 ? (sPosY + offset + 63) : (sPosY + offset));
                for (uint8_t tile = 0; tile < 2; tile++) {
                    oled.dot(sPosX + tile * 64, y, 1);
                        
                }
            }
            oled.drawBitmap(29 + groundShakeX, groundPos - 40 + groundShakeY, bitmap_LandingPadArm, 16, 48);
            oled.drawBitmap(63 - 24 + groundShakeX, groundPos - 5 + groundShakeY, bitmap_LandingPad, 48, 16);
            if (animationNotDone == false) {
                oled.clear(46, 43, 80, 48);
            }
            oled.drawBitmap(63 - 24 + shakeX, shipPos - 60 + shakeY, bitmap_Spaceship, 48, 56);
            if (showFire && animationNotDone) {
                oled.drawBitmap(63 - 4 + shakeX, shipPos - 14 + shakeY, bitmap_Fire, 8, 8);
            }
            oled.drawBitmap(0 + groundShakeX, groundPos + groundShakeY, bitmap_BackgroundMoon, 128, 16);
            oled.update();
        
            if (animationNotDone == false && isLanding) {
                frameCounter ++;
                if (frameCounter >= MENU_FPS * 1.5) {
                    return;
                }
            }
            else if (animationNotDone == false && not isLanding) {
                return;
            }
        }

    }

}