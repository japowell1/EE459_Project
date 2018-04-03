//
//  driver.c
//
//
//  Created by Joseph Powell on 3/23/18.
//

#include <stdio.h>
#include <string.h>
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
void unlock();
void readPassword();
char isCorrect;

volatile char setPasscode[7], tempPasscode[7];
volatile int passCount;
volatile int position, state;

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
    
    // Initialize Global Variables
    state = initState;
    position = 0;
    passCount = 0;
    isCorrect = 1;
    
    
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
    
    char* currentPos = setPasscode;

    // Display to LCD
    if (displayChar != '<')
    {
        if(displayChar > 34 && displayChar < 58)
        {
            // Capturing into passcodes

            if(state == initState)
            {
                // Add to setPasscode
                setPasscode[passCount] = displayChar;
                if(passCount == 6)
                {
                    setPasscode[6] = '\0';
                }
            }
            else if(state == lockState)
            {
                // Add to tempPasscode
                if(setPasscode[passCount] != displayChar)
                {
                    isCorrect = 0;
                }
            }
            else if(state == unlockState)
            {
                // Add to tempPasscode
                if(setPasscode[passCount] != displayChar)
                {
                    isCorrect = 0;
                }
            }
            
            lcd_moveto(2,position);
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
//            isCorrect = 1;
        }
    }
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
        else
        {
            if(isCorrect)
            {
                
                if(state == unlockState)
                {
                    state = lockState;
                }
                else if(state == lockState)
                {
                    state = unlockState;
                }
            }
            else if(!isCorrect)
            {
                
                if(state == unlockState)
                {
                    state = unlockState;
                }
                else if(state == lockState)
                {
                    state = lockState;
                }
            }
        }
    
        
        passCount = 0;          // Reset condition
        isCorrect = 1;
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
