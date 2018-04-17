//
//  driver.c
//
//
//  Created by Joseph Powell on 3/23/18.
//

#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
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





#define TIME 1000

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
volatile char isCorrect;
volatile char buzz;

volatile char setPasscode[7], tempPasscode[7];
volatile int passCount;
volatile int position, state, lockCount;

enum states
{
    initState,
    unlockState,
    lockState
};

void BuzzerOn();
void BuzzerOff();
ISR(PCINT2_vect);

int main(void)
{
    PCICR |= (1 << PCIE2);                          // Enable PIND Control Register interrupts
    PCMSK2 |= (1 << PCINT18);                       // Enable interrupts for PIND 2
    sei();                                          // Enables all interrupts
    
    
    DDRD |= (1 << DDD3);                            // OUTPUT Pi
//    PORTD |= (1 << DDD3); // INPUT Pi
    
    
    
    lcd_init();                 // Initialize the LCD display
    lcd_clear();
    lcd_moveto(0, 0);
    
    //    // Initialize Global Variables
    state = initState;
    position = 2;
    passCount = 0;
    isCorrect = 1;
    buzz = 0;
    lockCount = 0;
    
    
    
    
    while (1)
    {                 // Loop forever
                // State Machine
        if(state == initState)
        {
            initial();
            readPassword();

        }
        else if(state == unlockState)
        {
            PORTD &= ~(1 << PD3);
            unlock();
            readPassword();

        }
        else if(state == lockState)
        {
            PORTD |= 1 << PD3;
            lock();
            readPassword();

        }
        
        //        readPassword();
        //
    }
    return 0;
}

/*
 ISR for rotary encoder
 */
ISR(PCINT2_vect)
{
//    BuzzerOn();
//    buzz = 1;
    lcd_moveto(3,0);
    lcd_writedata('p');
}
               
               
void typeKeys()
{
    char displayChar;
    displayChar = Keypad();
    lcd_moveto(2,position);

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
            displayChar = '*';
            lcd_writedata(displayChar);
            passCount++;
        }
        // loop gets rid of repeat chars
        int k;
        for(k=0; k<120; k++)
        {
            lcd_wait();
        }
        
        if(position < 7)
        {
            position++;
        }
        else
        {
            position = 2;
        }
    }
}

// Reads in 6 digit passcode and changes state accordingly
void readPassword()
{
    lcd_moveto(2,0);
    while(passCount < 6)
    {
        typeKeys();
        if(lockCount == 10)
        {
//            BuzzerOn();
            lockCount = 0;
        }
        else
        {
            if(state == lockState)          // Sets alarm on loop
            {
                lockCount++;
                int k;
                for(k = 0; k < 2; k++)
                {
                    lcd_wait();
                    typeKeys();
                }
            }
        }
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
                    buzz = 0;
                }
                else if(state == lockState)
                {
                    state = unlockState;
                    buzz = 0;
                }
            }
            else if(!isCorrect)
            {
                
                if(state == unlockState)
                {
                    state = unlockState;
                    buzz = 0;
                }
                else if(state == lockState)
                {
                    state = lockState;
                    buzz = 1;
                    // Turn on Buzzer
                    //                    BuzzerOn();
                }
            }
        }
        passCount = 0;          // Reset condition
        isCorrect = 1;
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
    lcd_stringout(">");
    lcd_moveto(2,position);
}

// Displays lock screen
void lock()
{
    lcd_moveto(0,12);
    char lock_screen[] = "LOCKED";
    lcd_init();
    lcd_clear();
    lcd_moveto(0,0);
    lcd_stringout((char *)lock_screen);
    lcd_moveto(2,0);
    lcd_stringout(">");
    lcd_moveto(2,position);

}
// Display unlock screen
void unlock()
{
    lcd_moveto(0,12);
    char unlock_screen[] = "UNLOCKED";
    char enter[] = "Enter pwd to lock:";
    lcd_init();
    lcd_clear();
    lcd_moveto(0,0);
    lcd_stringout((char *)unlock_screen);
    lcd_moveto(1,0);
    lcd_stringout((char *)enter);
    lcd_moveto(2,0);
    lcd_stringout(">");
    lcd_moveto(2,position);
    BuzzerOff();
    
    
}

void BuzzerOn()
{
    DDRD |= 1 << DDD1;          // Set PORTC bit 0 for output
    for(int i = 0; i < TIME; i++)
    {
        PORTD &= ~(1 << PD1);   // Set PD1 to a 0       // Causing shifted display
        _delay_ms(0.12);
        PORTD |= 1 << PD1;      // Set PD1 to a 1
        _delay_ms(0.12);
    }
}
//
void BuzzerOff()
{
    DDRD &= ~(1 << DDD1);
    //    PORTD &= ~(1 << PD1);
}

