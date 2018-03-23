//
//  driver.c
//  
//
//  Created by Joseph Powell on 3/23/18.
//

#include <stdio.h>
#include "lcd.h"


/*
 Use the "PROGMEM" attribute to store the strings in the ROM
 instead of in RAM.
 */
#ifdef NIBBLE_HIGH
const unsigned char str1[] PROGMEM = ">> at328-5.c hi <<90";
#else
const unsigned char str1[] PROGMEM = ">> at328-5.c lo <<90";
#endif
const unsigned char str2[] PROGMEM = ">> USC EE459L <<7890";


int main(void) {
    
    lcd_init();                 // Initialize the LCD display
    lcd_moveto(0, 0);
    lcd_stringout_P((char *)str1);      // Print string on line 1
    
    lcd_moveto(1, 0);
    lcd_stringout_P((char *)str2);      // Print string on line 2
    
    lcd_clear();
    
    
    char displayChar;
    while (1) {                 // Loop forever
        
        displayChar = Keypad();
        
        if(displayChar > 34 && displayChar < 58)
        {
            lcd_moveto(0,0);
            lcd_writedata(displayChar);
        }
    }
    
    return 0;   /* never reached */
}
