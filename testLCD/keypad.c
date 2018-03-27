//
//  keypad.c
//  
//
//  Created by Joseph Powell on 3/23/18.
//

#include "keypad.h"

/*
 Read Keypad
 */
char Keypad()
{
    
    // Set columns as output
    DDRC |= (1 << DDC0); // Column 0
    DDRC |= (1 << DDC1); // Column 1
    DDRC |= (1 << DDC2); // Column 2
    
    // Pull up resistors for inputs
    PORTC |= (1 << DDC3); // Row 3 (*,0,#)
    PORTC |= (1 << DDC4); // Row 2 (7,8,9)
    PORTC |= (1 << DDC5); // Row 1 (4,5,6)
    PORTD |= (1 << PD0);    // Row 0 (1,2,3)
    
    // Define keypad
    char keys[4][3] = {
        {'*','0','#'},
        {'7','8','9'},
        {'4','5','6'},
        {'1','2','3'}
    };
    
    uint8_t r,c;
    for(c = 0; c < 3; c++)      // Loop through columns
    {
        PORTC |= 0X07;          // Set all bits hi
        
        PORTC &= ~(1 << c);
        
        for(r = 3; r < 7; r++)  // Loop through rows
        {
            if(r == 6)
            {
                if(!(PIND & (0x01 << PD0)))  // Read PIND0
                {
                    return keys[0][c];
                }
            }
            else
            {
                // Checks row 3 first
                if(!(PINC & (0x01 << r)))    // Read PINC3-5
                {
                    return keys[6-r][c];
                }
            }
        }
        //        _delay_ms(200);
    }
    return '<';
}

