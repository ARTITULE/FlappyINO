#ifndef CONFIG_H
#define CONFIG_H

// The lines below set the RST, DC and CS pins of the OLED module
// Leave them as default for the PCB version
#define OLED_RST 8
#define OLED_DC 9
#define OLED_CS 10

// The pin that you should connect the button to
#define BUTTON_PIN 3

// Do not touch these settings unless you know what you are doing :3
#define MENU_FPS 30
#define GAME_FPS 30
#define SAVE_AMOUNT 3
#define SETTING_AMOUNT 3

#define INTERNAL_REF 1075L
#define BATTERY_FULL 4200
#define BATTERY_EMPTY 2900

// The line below sets the score you need to clear a difficulty.
// See the README "Game Completion Score" section to get more information about what is the right value for you.
#define GAME_COMPLETE_SCORE 50

#endif