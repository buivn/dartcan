/*
 * LCD17x8ASCII.c
 *
 * Created: 28.11.2012 11:16:23
 *  Author: operator
 */

//_____ I N C L U D E S ________________________________________________________

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "LCD17x8ASCII.h"

//_____ A S C I I A R R A Y S __________________________________________________

// 13 initialization bytes array in program memory for DOGS LCD 102 x 64
const unsigned char DOGS_init [13] PROGMEM = 
{	0x40, 0xa1, 0xc0, 0xa4, 0xa6, 0xa2, 0x2f, 0x27, 0x81, 0x10, 0xfa, 0x90, 0xaf
};

// 8x6 pixel ASCII [0..127] array in program memory
const unsigned char ASCII_array [128][6] PROGMEM =
{ // Zeichen 0 bis 7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
  // Zeichen 8 bis 15
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 
  // Zeichen 16 bis 23
  0x1f, 0x05, 0x39, 0x44, 0x38, 0x00, 0x1f, 0x05, 0x49, 0x7c, 0x40, 0x00, // Small F0 and F1
  0x1f, 0x05, 0x49, 0x64, 0x58, 0x00, 0x1f, 0x05, 0x45, 0x54, 0x28, 0x00, // Small F2 and F3
  0x1f, 0x05, 0x1d, 0x10, 0x78, 0x00, 0x1f, 0x05, 0x5d, 0x54, 0x24, 0x00, // Small F4 and F5
  0x1f, 0x05, 0x39, 0x54, 0x24, 0x00, 0x1f, 0x05, 0x05, 0x74, 0x0c, 0x00, // Small F6 and F7
  // Zeichen 24 bis 31
  0x1f, 0x05, 0x29, 0x54, 0x28, 0x00, 0x1f, 0x05, 0x49, 0x54, 0x38, 0x00, // Small F8 and F9
  0x1c, 0x14, 0x36, 0x1c, 0x08, 0x00, 0x08, 0x1c, 0x36, 0x14, 0x1c, 0x00, // Arrows -> and <-
  0x39, 0x45, 0x55, 0x45, 0x39, 0x00, 0x4e, 0x51, 0x55, 0x51, 0x4e, 0x00, // Rotary encoder sign upper and lower
  0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0x00, // Block sign and inverted block sign
  // Zeichen 32 bis 39
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x00, // Blank and !
  0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00, // " and #
  0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00, 0x23, 0x13, 0x08, 0x64, 0x62, 0x00, // $ and %
  0x36, 0x49, 0x55, 0x22, 0x50, 0x00, 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, // & and '
  // Zeichen 40 bis 47
  0x00, 0x1c, 0x22, 0x41, 0x00, 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00, 0x00, // ( and )
  0x14, 0x08, 0x3e, 0x08, 0x14, 0x00, 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00, // * and +
  0x00, 0x50, 0x30, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, // , and -
  0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, // . and /
  // Zeichen 48 bis 55
  0x3e, 0x51, 0x49, 0x45, 0x3e, 0x00, 0x00, 0x42, 0x7f, 0x40, 0x00, 0x00, // 0 and 1
  0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x21, 0x41, 0x45, 0x4b, 0x31, 0x00, // 2 and 3
  0x18, 0x14, 0x12, 0x7f, 0x10, 0x00, 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, // 4 and 5
  0x3c, 0x4a, 0x49, 0x49, 0x30, 0x00, 0x01, 0x01, 0x79, 0x05, 0x03, 0x00, // 6 and 7
  // Zeichen 56 bis 63
  0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x06, 0x49, 0x49, 0x29, 0x1e, 0x00, // 8 and 9
  0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x56, 0x36, 0x00, 0x00, 0x00, // : and ;
  0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, // < and =
  0x41, 0x22, 0x14, 0x08, 0x00, 0x00, 0x02, 0x01, 0x51, 0x09, 0x06, 0x00, // > and ?
  // Zeichen 64 bis 71
  0x32, 0x49, 0x79, 0x41, 0x3e, 0x00, 0x7e, 0x09, 0x09, 0x09, 0x7e, 0x00, // @ and A
  0x7f, 0x49, 0x49, 0x49, 0x36, 0x00, 0x3e, 0x41, 0x41, 0x41, 0x22, 0x00, // B and C
  0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00, 0x7f, 0x49, 0x49, 0x49, 0x41, 0x00, // D and E
  0x7f, 0x09, 0x09, 0x09, 0x01, 0x00, 0x3e, 0x41, 0x49, 0x49, 0x7a, 0x00, // F and G
  // Zeichen 72 bis 79
  0x7f, 0x08, 0x08, 0x08, 0x7f, 0x00, 0x00, 0x41, 0x7f, 0x41, 0x00, 0x00, // H and I
  0x20, 0x41, 0x41, 0x3f, 0x01, 0x00, 0x7f, 0x08, 0x14, 0x22, 0x41, 0x00, // J and K
  0x7f, 0x40, 0x40, 0x40, 0x40, 0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00, // L and M
  0x7f, 0x04, 0x08, 0x10, 0x7f, 0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00, // N and O
  // Zeichen 80 bis 87
  0x7f, 0x09, 0x09, 0x09, 0x06, 0x00, 0x3e, 0x41, 0x51, 0x21, 0x5e, 0x00, // P and Q
  0x7f, 0x09, 0x19, 0x29, 0x46, 0x00, 0x46, 0x49, 0x49, 0x49, 0x31, 0x00, // R and S
  0x01, 0x01, 0x7f, 0x01, 0x01, 0x00, 0x3f, 0x40, 0x40, 0x40, 0x3f, 0x00, // T and U
  0x1f, 0x20, 0x40, 0x20, 0x1f, 0x00, 0x3f, 0x40, 0x38, 0x40, 0x3f, 0x00, // V and W
  // Zeichen 88 bis 95
  0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x07, 0x08, 0x70, 0x08, 0x07, 0x00, // X and Y
  0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x7f, 0x41, 0x41, 0x00, 0x00, // Z and [
  0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x41, 0x41, 0x7f, 0x00, 0x00, // \ and ]
  0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, // ^ and _
  // Zeichen 96 bis 103
  0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x20, 0x54, 0x54, 0x54, 0x78, 0x00, // ` and a
  0x7f, 0x48, 0x44, 0x44, 0x38, 0x00, 0x38, 0x44, 0x44, 0x44, 0x20, 0x00, // b and c
  0x38, 0x44, 0x44, 0x48, 0x7f, 0x00, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, // d and e
  0x08, 0xfe, 0x09, 0x09, 0x02, 0x00, 0x18, 0xa4, 0xa4, 0xa4, 0x7c, 0x00, // f and g
  // Zeichen 104 bis 111
  0x7f, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x44, 0x7d, 0x40, 0x00, 0x00, // h and i
  0x40, 0x80, 0x84, 0x7d, 0x00, 0x00, 0x7f, 0x10, 0x28, 0x44, 0x00, 0x00, // j and k
  0x00, 0x41, 0x7f, 0x40, 0x00, 0x00, 0x7c, 0x04, 0x18, 0x04, 0x78, 0x00, // l and m
  0x7c, 0x08, 0x04, 0x04, 0x78, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, // n and o
  // Zeichen 112 bis 119
  0xfc, 0x24, 0x24, 0x24, 0x18, 0x00, 0x18, 0x24, 0x24, 0x28, 0xfc, 0x00, // p and q
  0x7c, 0x08, 0x04, 0x04, 0x08, 0x00, 0x48, 0x54, 0x54, 0x54, 0x20, 0x00, // r and s
  0x04, 0x3f, 0x44, 0x40, 0x20, 0x00, 0x3c, 0x40, 0x40, 0x20, 0x7c, 0x00, // t and u
  0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00, 0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00, // v and w
  // Zeichen 120 bis 127
  0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x1c, 0xa0, 0xa0, 0xa0, 0x7c, 0x00, // x and y
  0x44, 0x64, 0x54, 0x4c, 0x44, 0x00, 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, // z and {
  0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x41, 0x36, 0x08, 0x00, 0x00, // | and }
  0x02, 0x01, 0x02, 0x04, 0x02, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00  // ~ and inverted blank
};

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------------------------
/*! \brief Init DOGS-LCD
 *	 
 *   DOGS is a graphic LCD with 64 Lines and 102 rows. For ASCII output
 *   on the display there are 8 lines with 16 characters  
 *  
 */
void DOGS_LCD_init (void)
{ 
	unsigned char  i, temp_byte;
	// Init ports
	DDRB = 0b00110111; // Out: 5-!CS 4-CD 2-MOSI 1-SCK 0-SS (must be output see datasheet)
	SPCR = 0b01010001; // SPI enable, master, SCK high if idle, sample on trailing edge, fck/16
	SPSR = 0; // fclkio/16 = 1 MHz
	
	PORTB = PORTB & 0b11001111; // Set command mode and chip select
	for (i = 0; i < 13; i++)
	{	
		temp_byte = pgm_read_byte(&DOGS_init[i]);
		SPDR = temp_byte;  
		while(!(SPSR & (1<<SPIF))); // Wait for transmission complete
	} // end for
	PORTB = PORTB | 0b00100000; // No chip select
}
//------------------------------------------------------------------------------------------------
/*! \brief Write one ASCII character to DOGS graphic display at position Col and Row
 *	 
 *  \param Row           Value for the ASCII row [0..7]
 *  \param Col           Value for the DOGS col [0..16]
 *  \param ASCIIchar     ASCII and special character to display [0..127]
 */
void Dpc (unsigned char Row, unsigned char Col, unsigned char ASCIIchar)
{ // Local vars
	unsigned char lauf, DOGSrow, DOGScol, DOGScol_l, DOGScol_h, DOGSchar;
	// Activate DOGS LCD
	PORTB = PORTB & 0b11011111; // Set chip select
	for (lauf = 0; lauf < 6; lauf++)
	{ 
		// Send 6 bytes to DOGS LCD
		PORTB = PORTB & 0b11101111; // Set command mode
		// Compute row address
		DOGSrow =  0b10110000 | (Row & 0b00000111); // ROW = [0..7]
		SPDR = DOGSrow;  
		while(!(SPSR & (1<<SPIF)));
		// Compute col address
		if (Col > 16) Col = 16;
		DOGScol = (Col * 6) + lauf; // Col = [0..16]
		// Compute high col address
		DOGScol_h = 0b00010000 | (DOGScol >> 4);
		SPDR = DOGScol_h;  while(!(SPSR & (1<<SPIF)));
		// Compute low col address
		DOGScol_l = 0b00000000 | (DOGScol & 0b00001111);
		SPDR = DOGScol_l;  while(!(SPSR & (1<<SPIF)));
		// Compute data byte
		
		PORTB = PORTB | 0b00010000; // Set data mode
		DOGSchar = pgm_read_byte(&ASCII_array[ASCIIchar & 0b01111111][lauf]);
		SPDR = DOGSchar;  
		while(!(SPSR & (1<<SPIF)));	// wait for transmitting complete
	}	// end for
	PORTB = PORTB | 0b00100000; // No chip select
}
//------------------------------------------------------------------------------------------------
/*! \brief Write a string to DOGS graphic display at position Col and Row
 *	 
 *  \param Row           Value for the ASCII row [0..7]
 *  \param Col           Value for the DOGS col [0..16]
 *  \param * String      Pointer to Null terminated char array
 */
void Dpstr (unsigned char row, unsigned char col, const char* string)
{ 
	unsigned char  i;
	if (*string != 0)
	  for (i = 0; *(string + i) != 0; i++)
	    Dpc ( row, col+i, *(string + i));
}
//------------------------------------------------------------------------------------------------
/*! \brief Clear screen at DOGS graphic display
 *	 
 *   DOGS is a graphic LCD with 64 Lines and 102 rows. For ASCII output
 *   on the display there are 8 lines with 16 characters  
 */
void Csr (void)
{ 
	unsigned char  i;
	for (i = 0; i < 136; i++)		// clear from the second arrow
		Dpc (( i / 17), (i % 17), ' ');
}
//------------------------------------------------------------------------------------------------