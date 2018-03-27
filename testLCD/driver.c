//
//  driver.c
//
//
//  Created by Joseph Powell on 3/23/18.
//

#include <stdio.h>
#include <cstring>
#include "lcd.h"

// TO DO
// char manipulations with password check and append
// Order SD card and cable
// Need power strips
// Attach keypad

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
<<<<<<< HEAD
void unlock();
void readPassword();

volatile char setPasscode[6], tempPasscode[6];
volatile int passCount;
volatile int position, state;
=======
void start();

volatile int count, state;
>>>>>>> e5f1cf61181f72f6859dfff84f93516ee739354d

enum states
{
    initState,
    unlockState,
    lockState
};

int main(void)
{
    
    lcd_init();                 // Initialize the LCD display
    lcd_clear();
    lcd_moveto(0, 0);
<<<<<<< HEAD

    // Initialize Global Variables
    state = initState;
    position = 0;
    passCount = 0;

    
    while (1)
    {                 // Loop forever
        // State Machine
        if(state == initState)
        {
            initial();
        }
        else if(state == unlockState)
        {
            unlock();
        }
        else if(state == lockState)
        {
            lock();
        }
        
        readPassword();

    }
    return 0;
}


void typeKeys()
{
    char displayChar;
    displayChar = Keypad();
    
    // Capturing into passcodes
    if(state == initState)
    {
        // Add to setPasscode
    }
    else if(state == unlock)
    {
        // Add to tempPasscode
    }
    else if(state == lockState)
    {
        // Add to tempPasscode
    }
    
    // Display to LCD
=======
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
>>>>>>> e5f1cf61181f72f6859dfff84f93516ee739354d
    if (displayChar != '<')
    {
        if(displayChar > 34 && displayChar < 58)
        {
<<<<<<< HEAD
            lcd_moveto(2,position);
=======
            lcd_moveto(1,count);
>>>>>>> e5f1cf61181f72f6859dfff84f93516ee739354d
            lcd_writedata(displayChar);
            passCount++;
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
        if(position < 5)
        {
            position++;
        }
        else
        {
            position = 0;
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
<<<<<<< HEAD
}

// Initial setup display
void initial()
{
    
    const unsigned char init_screen[] = "Set password:";
    lcd_init();
    lcd_clear();
    lcd_moveto(0,0);
    lcd_stringout((char *)init_screen);
    lcd_moveto(2,0);
}

// Reads in 6 digit passcode and changes state accordingly
void readPassword()
{
    lcd_moveto(2,0);
    while(passCount < 6)
    {
        typeKeys();
    }
    if(passCount == 6)          // Conditions change state
    {
        if(state == initState)
        {
            state = unlockState;
        }
        else if(state == unlockState)
        {
            state = lockState;
        }
        else if(state == lockState)
        {
            state = unlockState;
        }
        passCount = 0;          // Reset condition
    }
    
}

// Displays lock screen
void lock()
{
    char lock_screen[] = "LOCKED";
    lcd_init();
    lcd_clear();
    lcd_moveto(0,0);
    lcd_stringout((char *)lock_screen);
    lcd_moveto(2,0);

}
// Display unlock screen
void unlock()
{
    char unlock_screen[] = "UNLOCKED";
    char enter[] = "Enter password:";
    lcd_init();
    lcd_clear();
    lcd_moveto(0,0);
    lcd_stringout((char *)unlock_screen);
    lcd_moveto(1,0);
    lcd_stringout((char *)enter);
    lcd_moveto(2,0);

}

=======
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
>>>>>>> e5f1cf61181f72f6859dfff84f93516ee739354d
