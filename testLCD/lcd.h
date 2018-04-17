//
//  lcd.h
//  
//
//  Created by Joseph Powell on 3/23/18.
//

#ifndef lcd_h
#define lcd_h

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "keypad.h"
/*
 The NIBBLE_HIGH condition determines which PORT bits are used to
 transfer data to data bits 4-7 of the LCD.
 If LCD data bits 4-7 are connected to PORTD, bits 4-7, define NIBBLE_HIGH
 If LCD data bits 4-7 are connected to PORTD, bits 2-3 and PORTB bits 0-1,
 don't define NIBBLE_HIGH.
 */
#define NIBBLE_HIGH                 // Use bits 4-7 for talking to LCD

#define LCD_RS          (1 << PB4) //(1 << PB1) //
#define LCD_RW          (1 << PB3) //(1 << PB0) //
#define LCD_E           (1 << PB7) //(1 << PB2) //
#define LCD_Bits        (LCD_RS|LCD_RW|LCD_E)

#ifdef NIBBLE_HIGH
#define LCD_Data_D     0xf0     // Bits in Port D for LCD data
#define LCD_Status     0x80     // Bit in Port D for LCD busy status
#else
#define LCD_Data_B     0x03     // Bits in Port B for LCD data
#define LCD_Data_D     0x0c     // Bits in Port D for LCD data
#define LCD_Status     (1 << PD7) // Bit in Port D for LCD busy status
#endif


void lcd_init(void);
void lcd_moveto(unsigned char, unsigned char);
void lcd_stringout(char *);
void lcd_writecommand(unsigned char);
void lcd_writedata(unsigned char);
void lcd_writebyte(unsigned char);
void lcd_writenibble(unsigned char);
void lcd_wait(void);
void lcd_stringout_P(char *);
void lcd_clear();

#endif /* lcd_h */
