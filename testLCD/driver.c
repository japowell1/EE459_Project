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

#define TIME 1000

void typeKeys();
void initial();
void lock();
void unlock();
void readPassword();
volatile char isCorrect;
volatile char buzz, toggle;

volatile char setPasscode[7], tempPasscode[7];
volatile int passCount;
volatile int position, state, lockCount, offset, pos, posLock, posReset;

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

    PCICR |= (1 << PCIE0);                          // Enable PIND Control Register interrupts
    PCMSK0 |= (1 << PCINT2);                       // Enable interrupts for PIND 2
    
    sei();                                          // Enables all interrupts
    
    
    DDRD |= (1 << DDD3);                            // OUTPUT Pi
    PORTB |= (1 << PB2);        // INPUT Pi
    
    
    
    lcd_init();                 // Initialize the LCD display
    lcd_clear();
    lcd_moveto(0, 0);
    
    //    // Initialize Global Variables
    state = initState;
    position = 1;
    toggle = 0;
    passCount = 0;
    isCorrect = 1;
    buzz = 0;
    lockCount = 0;
    offset = 0;
    pos = 0;
    posLock = 0;
    posReset = 0;
    
    
    
    while (1)
    {                 // Loop forever
                // State Machine
        if(state == initState)
        {
            offset = 0;
            initial();
            readPassword();

        }
        else if(state == unlockState)
        {
            offset = 10;
            PORTD &= ~(1 << PD3);
            unlock();
            readPassword();
        }
        else if(state == lockState)
        {
            offset = 0;
            PORTD |= 1 << PD3;
            lock();
            readPassword();

        }
        //        readPassword();
    }
    return 0;
}

/*
ISR for toggle
 */
ISR(PCINT0_vect)
{
    unsigned char btnPress = (PINB & (1 << PB2));
    if (state == unlockState)
    {
        char clear[] = "        ";  // 8 spaces
        char arrow[] = ">";

        if (!btnPress && !toggle)
        {
            _delay_ms(200);         // Debounce button
            position = 1;
            lcd_moveto(1, 9);
            lcd_stringout((char *)clear);
            lcd_moveto(2, 10);
            lcd_stringout((char *)arrow);
            toggle = 1;
        }
        else if (!btnPress && toggle)
        {
            _delay_ms(200);
            position = 1;
            lcd_moveto(2, 9);
            lcd_stringout((char *)clear);
            lcd_moveto(1, 10);
            lcd_stringout((char *)arrow);
            toggle = 0;
            _delay_ms(100);
        }
        
        passCount = 0;              // Reset passCount
    }
}

/*
 ISR for buzzer
 */
ISR(PCINT2_vect)
{
//    BuzzerOn();
    buzz = 1;
    lcd_moveto(3,0);
    lcd_writedata('p');
}
               
               
void typeKeys()
{
    char displayChar;
    displayChar = Keypad();
    pos = position + offset;
    
    if (toggle == 0 && (state==unlockState))
        lcd_moveto(1,pos);
    else
        lcd_moveto(2,pos);

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
            position = 1;
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
        if(lockCount == 10 && (buzz == 1))
        {
            BuzzerOn();
            lockCount = 0;
        }
        else
        {
            if(state == lockState && (buzz == 1))          // Sets alarm on loop
            {
                lockCount++;
                int k;
                for(k = 0; k < 10; k++)
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
                    if(toggle == 0)
                    {
                        state = lockState;
                    }
                    else if(toggle == 1)
                    {
                        state = initState;
                    }
                    
                }
                else if(state == lockState)
                {
                    state = unlockState;
                }
                buzz = 0;
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
//                    buzz = 1;
                    // Turn on Buzzer
                    //                    BuzzerOn();
                }
            }
        }
        passCount = 0;          // Reset condition
        position = 1;
        isCorrect = 1;
    }
}

// Initial setup display
void initial()
{
    const unsigned char init_screen[] = "*** SET PASSWORD ***";
    lcd_init();
    lcd_clear();
    lcd_moveto(0,0);
    lcd_stringout((char *)init_screen);
    lcd_moveto(2,0);
    lcd_stringout(">");
    lcd_moveto(2,position);
    toggle = 0;
}

// Displays lock screen
void lock()
{
    char lock_screen[] = "****** LOCKED ******";
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
    char unlock_screen[] = "***** UNLOCKED *****";
    char enter[] =          "To lock : ";
    char enterToReset[] =   "To reset: ";
    char arrow[] = ">";
    lcd_init();
    lcd_clear();
    lcd_moveto(0,0);
    lcd_stringout((char *)unlock_screen);
    
    lcd_moveto(1,0);
    lcd_stringout((char *)enter);
    if(!toggle)
    {
        lcd_stringout((char*)arrow);
    }

    lcd_moveto(2,0);
    lcd_stringout((char *)enterToReset);
    if(toggle)
    {
        lcd_stringout((char*)arrow);
    }
    
    
    BuzzerOff();
}

void BuzzerOn()
{
    DDRD |= 1 << DDD1;          // Set PORTC bit 0 for output
    for(int i = 0; i < TIME; i++)
    {
        PORTD &= ~(1 << PD1);   // Set PD1 to a 0
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

