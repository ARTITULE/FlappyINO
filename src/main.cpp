/*

This is a project licensed under the MIT License
Project by ARTITULE

More details can be found on the GitHub page:
https://github.com/ARTITULE/FlappyINO

*/

#define EEPROM_KEY 0xB2
#define KEY_EE_ADDR 0
#define S1_HI_SCR_ADDR 1
#define S2_HI_SCR_ADDR 3
#define S3_HI_SCR_ADDR 5

#define OLED_SPI_SPEED 4000000ul

#include <Arduino.h>
#include "bitmap.h"
#include "FlappyBird_Game.h"
#include "functions.h"
#include "config.h"

#include <GyverOLED.h>
#include <EncButton.h>
#include <EEPROM.h>
#include <SPI.h>

GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> oled;
Button main_button(BUTTON_PIN);

void setup() {

    oled.init();
    oled.clear();
    oled.setContrast(255);


    if (EEPROM[KEY_EE_ADDR] != EEPROM_KEY) {
        EEPROM[KEY_EE_ADDR] = EEPROM_KEY;
        EEPROM[S1_HI_SCR_ADDR] = 0;
        EEPROM[S1_HI_SCR_ADDR + 1] = 0;
        EEPROM[S2_HI_SCR_ADDR] = 0;
        EEPROM[S2_HI_SCR_ADDR + 1] = 0;
        EEPROM[S3_HI_SCR_ADDR] = 0;
        EEPROM[S3_HI_SCR_ADDR + 1] = 0;
    }

    ADMUX = DEFAULT << 6 | 0b1110;
    ADCSRA = 1 << ADEN | 0b101;
    for (uint8_t i = 0; i < 8; i++) {
        ADCSRA |= 1 << ADSC;
        while (ADCSRA & (1 << ADSC))
            ;
    }
}

void MainMenu() {

    while (1) {
        main_button.tick();
        static uint8_t MenuPointer = 0;

        if (main_button.click()) {
            MenuPointer += 3;
            if (MenuPointer >= SAVE_AMOUNT * 3) {
                MenuPointer = 0;
            }
        }

        if (main_button.hold()) {

            switch (MenuPointer) {
            case 0:
                FBirdGame(S1_HI_SCR_ADDR, 0, oled, main_button);
                return;
            case 3:
                FBirdGame(S2_HI_SCR_ADDR, 1, oled, main_button);
                return;
            case 6:
                FBirdGame(S3_HI_SCR_ADDR, 2, oled, main_button);
                return;
            }
        }


        static uint32_t MenuDrawTimer = millis();
        static bool drawHold = true;
        static uint8_t drawCounter = 0;
        if (millis() - MenuDrawTimer >= 1000 / MENU_FPS) {
            MenuDrawTimer = millis();
            drawCounter ++;

            if (drawCounter >= 20) {
                drawCounter = 0;
                drawHold = !drawHold;
            }


            switch (MenuPointer) {
            case 0:
                drawMainMenuCard(S1_HI_SCR_ADDR, "Moon", "Steady", bitmap_Moon, drawHold, 0, oled);
                break;
            case 3:
                drawMainMenuCard(S2_HI_SCR_ADDR, "Earth", "Rush", bitmap_Earth, drawHold, 1, oled);
                break;
            case 6:
                drawMainMenuCard(S3_HI_SCR_ADDR, "Saturn", "Frenzy", bitmap_Saturn, drawHold, 2, oled);
                break;
            }
        }
    }
}

void loop() {

    main_button.tick();
    static uint32_t LoopTimer = millis();

    if (main_button.click()) {
        MainMenu();
    }

    if (millis() - LoopTimer >= 1000 / MENU_FPS) {

        oled.clear();
        oled.drawBitmap(0, 0, bitmap__Background, 64, 64);
        oled.drawBitmap(63, 0, bitmap__Background, 64, 64);
        oled.drawBitmap(19, 15, bitmap__FlappyBirdLogo, 90, 24);
        oled.setCursorXY(25, 45);
        oled.setScale(1);
        oled.print("Press To Play");
        batCheckDraw(oled);
        oled.update();
    }
}
