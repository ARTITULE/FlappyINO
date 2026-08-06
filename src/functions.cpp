
#include "functions.h"


void WaitingScreen(GyverOLED <SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {
	oled.clear(); oled.setScale(4);
	oled.setCursor(54, 3); oled.print("3");
	oled.update();
	delay(400);

	oled.clear(); oled.setCursor(54, 3); oled.print("2");
	oled.update();
	delay(400);

	oled.clear(); oled.setCursor(54, 3); oled.print("1");
	oled.update();
	delay(400);

	oled.clear(); oled.setCursor(54, 3); oled.print("0");
	oled.update();
}

void batCheckDraw(GyverOLED <SSD1306_128x64, OLED_BUFFER, OLED_SPI, OLED_CS, OLED_DC, OLED_RST> &oled) {
    static uint32_t measureTimer = millis() + 3500;  
    static uint8_t batCharge = 0;                    

    if (millis() - measureTimer >= 3000) {
        measureTimer = millis();
        ADCSRA |= 1 << ADSC;                
        while (ADCSRA & (1 << ADSC));      
        batCharge = constrain(map((INTERNAL_REF * 1024UL) / ADC, BATTERY_EMPTY, BATTERY_FULL, 0, 12), 0, 12);
    }

    oled.setCursorXY(110, 2);                             
    oled.drawByte(0b00111100);                           
    oled.drawByte(0b00111100);                            
    oled.drawByte(0b11111111);                            
    for (uint8_t i = 0; i < 12; i++) {                    
        if (i < 12 - batCharge)oled.drawByte(0b10000001);  
        else oled.drawByte(0b11111111);                     
    }    
    oled.drawByte(0b11111111);                          
}