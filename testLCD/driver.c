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
const unsigned char str1[] PROGMEM = "ee459";
#else
const unsigned char str1[] PROGMEM = "2";
#endif
const unsigned char str2[] PROGMEM = "group 11";


int main(void) {
    
    lcd_init();                 // Initialize the LCD display
    lcd_clear();
    lcd_moveto(0, 0);
    lcd_stringout_P((char *)str1);      // Print string on line 1
    
    lcd_moveto(1, 0);
    lcd_stringout_P((char *)str2);      // Print string on line 2

    int count = 0;
    
    char displayChar;
    while (1) {                 // Loop forever
        
        displayChar = Keypad();
        if (displayChar != '<'){
        if(displayChar > 34 && displayChar < 58)
        {
            lcd_moveto(3,count);
            lcd_writedata(displayChar);
        }
        int k;
        for(k=0; k<120; k++){
            lcd_wait();
        }
        count++;
        }
    }
    
    return 0;   /* never reached */
}
