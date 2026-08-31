/*

This is a project licensed under the MIT License
Project by ARTITULE

More details can be found on the GitHub page:
https://github.com/ARTITULE/FlappyINO

*/

#define EEPROM_KEY 0xA1
#define KEY_EE_ADDR 0
#define S1_HI_SCR_ADDR 1
#define S2_HI_SCR_ADDR 3
#define S3_HI_SCR_ADDR 5
#define SETTINGS_ADDR 7

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

struct {
    int8_t showAnimation = 1;
    int8_t showBackground = 1;
    int8_t showStarsBackground = 1;
} settings;

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
        EEPROM.put(SETTINGS_ADDR, settings);
    }
    else {
        EEPROM.get(SETTINGS_ADDR, settings);
    }

    ADMUX = DEFAULT << 6 | 0b1110;
    ADCSRA = 1 << ADEN | 0b101;
    for (uint8_t i = 0; i < 8; i++) {
        ADCSRA |= 1 << ADSC;
        while (ADCSRA & (1 << ADSC));
    }
}

void Settings() {
    while (1) {
        main_button.tick();
        static uint8_t menuPointer = 0;

        if (main_button.click()) {
            menuPointer += 1;
            if (menuPointer > SETTING_AMOUNT) {
                menuPointer = 0;
            }
        }

        if (main_button.hold()) {

            switch (menuPointer) {

            case 0:
                settings.showAnimation += (settings.showAnimation == 0 ? 1 : -1);
                EEPROM.put(SETTINGS_ADDR, settings);
                break;
            case 1:
                settings.showBackground += (settings.showBackground == 0 ? 1 : -1);
                EEPROM.put(SETTINGS_ADDR, settings);
                break;
            case 2:
                settings.showStarsBackground += (settings.showStarsBackground == 2 ? -2 : 1);
                EEPROM.put(SETTINGS_ADDR, settings);
                break;
            case SETTING_AMOUNT:
                return;
                break;
            
            default:
                return;
                break;
            }
        }

        static uint32_t settingTimer;
        if (millis() - settingTimer >= 1000 / MENU_FPS) {
            settingTimer = millis();

            EEPROM.get(SETTINGS_ADDR, settings);

            oled.clear();
            oled.roundRect(0, 0, 127, 63, OLED_STROKE);
            printCentered("Settings", 1 * 8, 1, oled);
            drawSettingBox(3, "Show Animation:", settings.showAnimation, "No", "Yes", "", oled);
            drawSettingBox(4, "Show Backg.:", settings.showBackground, "No", "Yes", "", oled);
            drawSettingBox(5, "Show Stars:", settings.showStarsBackground, "No", "All", "Menu", oled);
            oled.setCursor(4, SETTING_AMOUNT + 3);
            oled.print("Return (Hold)");
            oled.setCursor(127 - 6, menuPointer + 3);
            oled.print("<");
            if (settings.showStarsBackground == 1 || settings.showStarsBackground == 2) {

                oled.drawBitmap(0, 0, bitmap__Background, 64, 64);
                oled.drawBitmap(63, 0, bitmap__Background, 64, 64);
            }
            oled.update();
        } 
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
        static int16_t moonScore;
        static int16_t earthScore;
        EEPROM.get(S1_HI_SCR_ADDR, moonScore);
        EEPROM.get(S2_HI_SCR_ADDR, earthScore);

        static bool earthLocked;
        static bool saturnLocked;

        if (moonScore < GAME_COMPLETE_SCORE) {
            earthLocked = true;
            saturnLocked = true;
        }
        if (moonScore >= GAME_COMPLETE_SCORE) {
            earthLocked = false;
        }
        if (earthScore < GAME_COMPLETE_SCORE) {
            saturnLocked = true;
        }
        if (moonScore >= GAME_COMPLETE_SCORE && earthScore >= GAME_COMPLETE_SCORE) {
            saturnLocked = false;
        }
        
        if (main_button.hold()) {

            switch (MenuPointer) {
            case 0:
                FBirdGame(S1_HI_SCR_ADDR, 0, settings.showAnimation, settings.showBackground, settings.showStarsBackground, GAME_COMPLETE_SCORE, oled, main_button);
                return;
            case 3:
                if (!earthLocked) {
                    FBirdGame(S2_HI_SCR_ADDR, 1, settings.showAnimation, settings.showBackground, settings.showStarsBackground, GAME_COMPLETE_SCORE, oled, main_button);
                    return;
                }
            case 6:
                if (!saturnLocked) {
                    FBirdGame(S3_HI_SCR_ADDR, 2, settings.showAnimation, settings.showBackground, settings.showStarsBackground, GAME_COMPLETE_SCORE, oled, main_button);
                    return;
                }
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
                drawMainMenuCard(S1_HI_SCR_ADDR, "Moon", "Steady", bitmap_Moon, drawHold, 0, 0, GAME_COMPLETE_SCORE, oled);
                break;
            case 3:
                drawMainMenuCard(S2_HI_SCR_ADDR, "Earth", "Rush", bitmap_Earth, drawHold, earthLocked, 1, GAME_COMPLETE_SCORE, oled);
                break;
            case 6:
                drawMainMenuCard(S3_HI_SCR_ADDR, "Saturn", "Frenzy", bitmap_Saturn, drawHold, saturnLocked, 2, GAME_COMPLETE_SCORE, oled);
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
    else if (main_button.hold()) {
        Settings();
    }

    if (millis() - LoopTimer >= 1000 / MENU_FPS) {

        oled.clear();
        if (settings.showStarsBackground == 1 || settings.showStarsBackground == 2) {
            oled.drawBitmap(0, 0, bitmap__Background, 64, 64);
            oled.drawBitmap(63, 0, bitmap__Background, 64, 64);
        }
        oled.drawBitmap(((124 - 90) / 2), 10, bitmap__FlappyBirdLogo, 90, 24);
        printCentered("Press To Play", 38, 1, oled);
        printCentered("Hold for Settings", 7 * 8 , 1, oled);
        batCheckDraw(oled);
        oled.update();
    }
}
