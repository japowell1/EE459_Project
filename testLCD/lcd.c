/*************************************************************
 *       lcd.c - Demonstrate interface to a parallel LCD display
 *
 *       This program will print a message on an LCD display
 *       using the 4-bit wide interface method
 *
 *       PORTB, bit 4 (0x10) - output to RS (Register Select) input of display
 *              bit 3 (0x08) - output to R/W (Read/Write) input of display
 *              bit 2 (0x04) - output to E (Enable) input of display
 *       PORTB, bits 0-1, PORTD, bits 2-7 - Outputs to DB0-DB7 inputs of display.
 *
 *       The second line of the display starts at address 0x40.
 *
 * Revision History
 * Date     Author      Description
 * 11/17/05 A. Weber    First Release for 8-bit interface
 * 01/13/06 A. Weber    Modified for CodeWarrior 5.0
 * 08/25/06 A. Weber    Modified for JL16 processor
 * 05/08/07 A. Weber    Some editing changes for clarification
 * 06/25/07 A. Weber    Updated name of direct page segment
 * 08/17/07 A. Weber    Incorporated changes to demo board
 * 01/15/08 A. Weber    More changes to the demo board
 * 02/12/08 A. Weber    Changed 2nd LCD line address
 * 04/22/08 A. Weber    Added "one" variable to make warning go away
 * 04/15/11 A. Weber    Adapted for ATmega168
 * 01/30/12 A. Weber    Moved the LCD strings into ROM
 * 02/27/12 A. Weber    Corrected port bit assignments above
 * 11/18/13 A. Weber    Renamed for ATmega328P
 * 04/10/15 A. Weber    Extended "E" pulse, renamed strout to strout_P
 * 05/06/17 A. Weber    Change to use new LCD routines
 *************************************************************/

#include "lcd.h"


//int main(void) {
//
//    lcd_init();                 // Initialize the LCD display
//    lcd_moveto(0, 0);
//    lcd_stringout_P((char *)str1);      // Print string on line 1
//
//    lcd_moveto(1, 0);
//    lcd_stringout_P((char *)str2);      // Print string on line 2
//
//    lcd_clear();
//
//
//    char displayChar;
//    while (1) {                 // Loop forever
//
//        displayChar = Keypad();
//
//        if(displayChar > 34 && displayChar < 58)
//        {
//            lcd_moveto(0,0);
//            lcd_writedata(displayChar);
//        }
//    }
//
//    return 0;   /* never reached */
//}

/*
 lcd_stringout_P - Print the contents of the character string "s" starting at LCD
 RAM location "x" where the string is stored in ROM.  The string must be
 terminated by a zero byte.
 */
void lcd_stringout_P(char *s)
{
    unsigned char ch;
    
    /* Use the "pgm_read_byte()" routine to read the date from ROM */
    while ((ch = pgm_read_byte(s++)) != '\0') {
        lcd_writedata(ch);             // Output the next character
    }
}

/*
 lcd_init - Do various things to force a initialization of the LCD
 display by instructions, and then set up the display parameters and
 turn the display on.
 */
void lcd_init(void)
{
#ifdef NIBBLE_HIGH
    DDRD |= LCD_Data_D;         // Set PORTD bits 4-7 for output
#else
    DDRB |= LCD_Data_B;         // Set PORTB bits 0-1 for output
    DDRD |= LCD_Data_D;         // Set PORTD bits 2-3 for output
#endif
    DDRB |= LCD_Bits;           // Set PORTB bits 2, 3 and 4 for output
    
    PORTB &= ~LCD_RS;           // Clear RS for command write
    
    
    
    _delay_ms(15);              // Delay at least 15ms
    
    lcd_writenibble(0x30);      // Send 00110000, set for 8-bit interface
    _delay_ms(5);               // Delay at least 4msec
    
    lcd_writenibble(0x30);      // Send 00110000, set for 8-bit interface
    _delay_us(120);             // Delay at least 100usec
    
    lcd_writenibble(0x30);      // Send 00110000, set for 8-bit interface
    _delay_ms(2);
    
    lcd_writenibble(0x20);      // Send 00100000, set for 4-bit interface
    _delay_ms(2);
    
    lcd_writecommand(0x28);     // Function Set: 4-bit interface, 2 lines
    
    lcd_writecommand(0x0f);     // Display and cursor on
}

/*
 lcd_moveto - Move the cursor to the row and column given by the arguments.
 Row is 0 or 1, column is 0 - 15.
 */
void lcd_moveto(unsigned char row, unsigned char col)
{
    unsigned char pos;
    pos = row * 0x40 + col;
    
    if(row == 2)
    {
        pos = 0x14 + col;
    }
    else if(row == 3)
    {
        pos = 0x54 + col;
    }
    lcd_writecommand(0x80 | pos);

    if(row == 2){
        pos = 0x14 + col;
    }
    else if(row == 3){
        pos = 0x54 + col;
    }

    lcd_writecommand(0x80 | pos);
    
}

/*
 lcd_stringout - Print the contents of the character string "str"
 at the current cursor position.
 */
void lcd_stringout(char *str)
{
    char ch;
    while ((ch = *str++) != '\0')
        lcd_writedata(ch);
}

/*
 lcd_writecommand - Output a byte to the LCD command register.
 */
void lcd_writecommand(unsigned char x)
{
    PORTB &= ~LCD_RS;       // Clear RS for command write
    lcd_writebyte(x);
    lcd_wait();
}

/*
 lcd_writedata - Output a byte to the LCD data register
 */
void lcd_writedata(unsigned char x)
{
    PORTB |= LCD_RS;    // Set RS for data write
    lcd_writebyte(x);
    lcd_wait();
}

/*
 lcd_writebyte - Output a byte to the LCD
 */
void lcd_writebyte(unsigned char x)
{
    lcd_writenibble(x);
    lcd_writenibble(x << 4);
}

/*
 lcd_writenibble - Output a 4-bit nibble to the LCD
 */
void lcd_writenibble(unsigned char x)
{
#ifdef NIBBLE_HIGH
    PORTD &= ~LCD_Data_D;
    PORTD |= (x & LCD_Data_D);  // Put high 4 bits of data in PORTD
#else
    PORTB &= ~LCD_Data_B;
    PORTB |= (x & LCD_Data_B);  // Put low 2 bits of data in PORTB
    PORTD &= ~LCD_Data_D;
    PORTD |= (x & LCD_Data_D);  // Put high 2 bits of data in PORTD
#endif
    PORTB &= ~(LCD_RW|LCD_E);   // Set R/W=0, E=0
    PORTB |= LCD_E;             // Set E to 1
    PORTB |= LCD_E;             // Extend E pulse > 230ns
    PORTB &= ~LCD_E;            // Set E to 0
}

/*
 lcd_wait - Wait for the BUSY flag to reset
 */
void lcd_wait()
{
#ifdef USE_BUSY_FLAG
    unsigned char bf;
    
#ifdef NIBBLE_HIGH
    PORTD &= ~LCD_Data_D;       // Set for no pull ups
    DDRD &= ~LCD_Data_D;        // Set for input
#else
    PORTB &= ~LCD_Data_B;       // Set for no pull ups
    PORTD &= ~LCD_Data_D;
    DDRB &= ~LCD_Data_B;        // Set for input
    DDRD &= ~LCD_Data_D;
#endif
    
    PORTB &= ~(LCD_E|LCD_RS);   // Set E=0, R/W=1, RS=0
    PORTB |= LCD_RW;
    
    do {
        PORTB |= LCD_E;         // Set E=1
        _delay_us(1);           // Wait for signal to appear
        bf = PIND & LCD_Status; // Read status register high bits
        PORTB &= ~LCD_E;        // Set E=0
        PORTB |= LCD_E;         // Need to clock E a second time to fake
        PORTB &= ~LCD_E;        //   getting the status register low bits
    } while (bf != 0);          // If Busy (PORTD, bit 7 = 1), loop
    
#ifdef NIBBLE_HIGH
    DDRD |= LCD_Data_D;         // Set PORTD bits for output
#else
    DDRB |= LCD_Data_B;         // Set PORTB, PORTD bits for output
    DDRD |= LCD_Data_D;
#endif
#else
    _delay_ms(2);        // Delay for 2ms
#endif
}

/*
 lcd_clear clears entire screen
 */
void lcd_clear()
{
    lcd_moveto(0,0);
    lcd_writecommand(0x01);
}
