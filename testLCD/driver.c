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
const unsigned char str1[] PROGMEM = "Set password:";
#else
const unsigned char str2[] PROGMEM = "LOCKED";
#endif
const unsigned char str3[] PROGMEM = "Enter password:";

void typeKeys();
void initial();
void lock();
void start();

volatile int count, state;

int main(void) {
    
    lcd_init();                 // Initialize the LCD display
    lcd_clear();
    lcd_moveto(0, 0);
    //    lcd_stringout_P((char *)str1);      // Print string on line 1
    //
    //    lcd_moveto(1, 0);
    //    lcd_stringout_P((char *)str2);      // Print string on line 2
    
    
    //    char password[];
    
    state = 0;
    count = 0;
    initial();
    while (1) {                 // Loop forever
        

//        typeKeys();
//
//        char key;
//        key = Keypad();
//        if (key == '*') {
//            ;
//        }
        
        
        //        if(state==0)
        //            initial();
        //        if(state==1)
        //            lock();
        //        if(state==2)
        //            start();
    }
return 0;
}


void typeKeys(){
    char displayChar;
    displayChar = Keypad();
    if (displayChar != '<')
    {
        if(displayChar > 34 && displayChar < 58)
        {
            lcd_moveto(1,count);
            lcd_writedata(displayChar);
        }
        int k;
        for(k=0; k<120; k++)
        {
            lcd_wait();
        }
        //        if(count > 19){
        //            count = 0;
        //        }
        //        if(displayChar=='*'){
        //            state = 1;
        //        }
        //        if(displayChar=='#'){
        //            state = 2;
        //        }
        count++;
    }
}


void initial(){

    const unsigned char init_screen[] = "Set password:";
    lcd_init();
    lcd_clear();
    lcd_moveto(0,0);
    lcd_stringout((char *)init_screen);
}

void readPassword()
{
    lcd_moveto(1,0);
    int i;
    for (i=0; i<7; i++)
    {
        typeKeys();
    }
}

//void lock(){
//    char lock_screen[] = "LOCKED";
//    lcd_init();
//    lcd_clear();
//    lcd_moveto(0,0);
//    lcd_stringout_P((char *)lock_screen);
//}
//
//void start(){
//    char start_screen[] = "Enter password:";
//    lcd_init();
//    lcd_clear();
//    lcd_moveto(1,0);
//    lcd_stringout_P((char *)start_screen);
//    int i;
//    for (i=0; i<7; i++) {
//        typeKeys();
//    }
//}
