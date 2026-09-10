#ifndef CONFIG_H
#define CONFIG_H

#define OLED_RST 8
#define OLED_DC 9
#define OLED_CS 10

#define BUTTON_PIN 3
#define MENU_FPS 30
#define GAME_FPS 30
#define SAVE_AMOUNT 3
#define SETTING_AMOUNT 3

#define INTERNAL_REF 1075L
#define BATTERY_FULL 4200
#define BATTERY_EMPTY 2900

// The line below sets the score you need to clear a difficulty.
// See the README "Game Completion Score" section to get more information
#define GAME_COMPLETE_SCORE 50

#endif