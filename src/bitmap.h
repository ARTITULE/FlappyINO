#ifndef BITMAPS_H
#define BITMAPS_H

#include <Arduino.h> 

#define NUM_STARS 11

extern const uint8_t StarX[NUM_STARS] PROGMEM;
extern const uint8_t StarY[NUM_STARS] PROGMEM;
extern const unsigned char bitmap__Bird_Normal [] PROGMEM;
extern const unsigned char bitmap__Bird_Flapped [] PROGMEM;
extern const uint8_t bitmap__Background[] PROGMEM;
extern const uint8_t bitmap__FlappyBirdLogo[] PROGMEM;
extern const uint8_t bitmap__Low_pipe[] PROGMEM;
extern const uint8_t bitmap__Middle_pipe[] PROGMEM;
extern const uint8_t bitmap__Top_pipe[] PROGMEM;
extern const uint8_t bitmap_Moon[] PROGMEM;
extern const uint8_t bitmap_Earth[] PROGMEM;
extern const uint8_t bitmap_Jupiter[] PROGMEM;



#endif