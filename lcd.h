/*!
 * \~english 
 * \file lcd.h
 *
 * \brief Library to control LCD over I²c
 *
 * \par LCD Library
 *
 * This Library is designed for HD44870 based LCDs with I2C expander PCF8574p.
 * It requires I²C Master Software from Peter Fleury.
 * PIN-Assignment: P0-P3 -> DB4-DB7, P4 -> RS, P5 -> R/w, P7 -> Enable.
 * For Use with ASURO Robot
 *
 * Inspiration from I²C-LCD library by "Nico Eichelmann, Thomas Eichelmann"
 */
/*------------------------------------------------------------------------------
 *
 * Copyright (c) 2006 Rizqi Ahmad (raid_ox)
 *
 * This software is a free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your option)
 * any later version. You should have received a copy of the GNU
 * General Public License along with asurolib; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301  USA
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************/

#ifndef LCD_H
#define LCD_H

#include "asuro.h"
#include "myasuro.h"

/*------ DEFINITIONS & MACROS -------------------------------*/


//////////////// ! LCD SETTING TO BE ADJUSTED

////// LCD INIT COMMANDS
// !SEE DATASHEET
// EXAMPLE:
//#define LCD_INIT {(LCD_4BIT | LCD_1LINE | LCD_5x7), COMMAND2, COMMAND3}

#define LCD_INIT	{(LCD_4BIT | LCD_2LINE | LCD_5X7), 0}
//#define LCD_INIT {0x29, 0x1C, 0x52, 0x69, 0x74, 0}             // DOGM162 Init, See Datasheet

#define LCD_DEV		0x40			// LCD Device Address
#define LCD_LINES	2			 	  // Total LCD Lines
#define LCD_CHARS	8				  // Maximal LCD characters each line

#define LCD_LINE1	0x00			// This should be 0x00 on all displays
#define LCD_LINE2	0x40			// Change this to the address for line 2 on your display
#define LCD_LINE3	0x10			// Change this to the address for line 3 on your display
#define LCD_LINE4	0x50			// Change this to the address for line 4 on your display

////// PCF8574p PINS

#define LD4         MY_LCD_LD4            // Pin to Data Bus 4
#define LD5         MY_LCD_LD5            // Pin to Data Bus 5
#define LD6         MY_LCD_LD6            // Pin to Data Bus 6
#define LD7         MY_LCD_LD7            // Pin to Data Bus 7

#define LRS         MY_LCD_LRS            // Pin to RS Pin (Register Select)
#define LRW         MY_LCD_LRW            // Pin to Read/Write Pin
#define LEN         MY_LCD_LEN            // Pin to Enable Pin
#define LBL         MY_LCD_LBL            // Backlight Pin

//////////////// ! LCD SETTING END

////// BITS

#define LCD_D0		(1 << LD4)
#define LCD_D1		(1 << LD5)
#define LCD_D2		(1 << LD6)
#define LCD_D3		(1 << LD7)

#define LCD_D4		(1 << LD4)
#define LCD_D5		(1 << LD5)
#define LCD_D6		(1 << LD6)
#define LCD_D7		(1 << LD7)

#define LCD_RS		(1 << LRS)
#define LCD_RW		(1 << LRW)
#define LCD_EN		(1 << LEN)
#define LCD_BL		(1 << LBL)

////// DEFINITIONS

#ifndef CENTER
#define CENTER 			2
#endif

////// COMMANDS

#define LCD_CLEAR		        0x01						            // Clear Screen
#define LCD_HOME			      0x02						            // Cursor Home

#define LCD_ENTRYMODE		    0x04						            // Set entrymode
#define LCD_INCREASE		    (LCD_ENTRYMODE | 0x02)		  // Set cursor move direction -- Increase
#define LCD_DECREASE		    (LCD_ENTRYMODE | 0x00)		  // Set cursor move direction -- Decrease
#define LCD_DISPLAYSHIFTON	(LCD_ENTRYMODE | 0x01)		  // Display is shifted
#define LCD_DISPLAYSHIFTOFF	(LCD_ENTRYMODE | 0x00)		  // Display is not shifted

#define LCD_DISPLAYMODE	    0x08						            // Set displaymode
#define LCD_DISPLAYON		    (LCD_DISPLAYMODE | 0x04)	  // Display on
#define LCD_DISPLAYOFF	    (LCD_DISPLAYMODE | 0x00)	  // Display off
#define LCD_CURSORON		    (LCD_DISPLAYMODE | 0x02)	  // Cursor on
#define LCD_CURSOROFF		    (LCD_DISPLAYMODE | 0x00)	  // Cursor off
#define LCD_BLINKINGON	    (LCD_DISPLAYMODE | 0x01)	  // Blinking on
#define LCD_BLINKINGOFF	    (LCD_DISPLAYMODE | 0x00)	  // Blinking off

#define LCD_SHIFTMODE		    0x10						            // Set shiftmode
#define LCD_DISPLAYSHIFT	  (LCD_SHIFTMODE | 0x08)		  // Display shift
#define LCD_CURSORMOVE	    (LCD_SHIFTMODE | 0x00)		  // Cursor move
#define LCD_RIGHT			      (LCD_SHIFTMODE | 0x04)		  // Right shift
#define LCD_LEFT			      (LCD_SHIFTMODE | 0x00)		  // Left shift

#define LCD_CONFIGURATION	  0x20					          	  // Set function
#define LCD_8BIT			      (LCD_CONFIGURATION | 0x10)  // 8 bits interface
#define LCD_4BIT			      (LCD_CONFIGURATION | 0x00)	// 4 bits interface
#define LCD_2LINE			      (LCD_CONFIGURATION | 0x08)	// 2 line display
#define LCD_1LINE			      (LCD_CONFIGURATION | 0x00)	// 1 line display
#define LCD_5X10			      (LCD_CONFIGURATION | 0x04)	// 5 X 10 dots
#define LCD_5X7				      (LCD_CONFIGURATION | 0x00)	// 5 X 7 dots

#define LCD_CGRAM			      0x40						            // DD RAM Address Set
#define LCD_DDRAM			      0x80						            // DD RAM Address Set

// Makros für die drei Taster 
#define LCD_KEY_YELLOW   (1<<PD6)
#define LCD_KEY_RED      (1<<PD2)
#define LCD_KEY_BLUE     (1<<PB3)

/*------- GLOBAL VARIABLES -----------------*/

unsigned char portLCD; // LCD PORT REGISTER
unsigned char lineLCD;
unsigned char cursorLCD;

/*------ HEADERS & DESCRIPTIONS ------------*/
/*!
 * \~english 
 * \brief Init LCD Hardware
 */
void InitLCD(void);

/*!
 * \~english 
 * \brief Init LCD Keys
 */
void InitLCDKeys(void);

/*!
 * \~english 
 * \brief Switch LCD Backlight on/off
 * \param state On or off
 */
void BacklightLCD(unsigned char state);

/*!
 * \~english 
 * \brief Set LCD IO Port
 * \param setCommand ON or OFF
 * \param bits Bits to set
 */

void SetIOLCD(unsigned char setCommand, unsigned char bits);

/*!
 * \~english 
 * \brief Read LCD IO Port
 * \return IO Port
 */
unsigned char GetIOLCD(void);

/*!
 * \~english 
 * \brief Set Cursor
 * \param cursor cursor position
 * \param line line number
 */
void SetCursorLCD(unsigned char cursor, unsigned char line);

/*!
 * \~english 
 * \brief Clears the Display
 */
void ClearLCD(void);

/*!
 * \~english 
 * \brief Write Data LCD, for use with CommandLCD or WriteLCD
 * \param data LCD Data
 */
void SetDataLCD(unsigned char data);

/*!
 * \~english 
 * \brief Set LCD Command, implemented as macro
 * \param command LCD command
 */
void CommandLCD(unsigned char command);

/*!
 * \~english 
 * \brief Write Data
 * \param data LCD Data
 */
void WriteLCD(unsigned char data);

/*!
 * \~english 
 * \brief Print String to LCD
 * \param string String pointer
 * \param wrap String Wrapped or not
 */
void PrintLCD(char *string, unsigned char wrap);

/*!
 * \~english 
 * \brief Print String at cursor position
 * \param cursor Cursor position
 * \param line line number
 * \param string String pointer
 */
void PrintSetLCD(unsigned char cursor, unsigned char line, char *string);

/*!
 * \~english 
 * \brief Print Integer Value
 * \param value Integer value
 */
void PrintIntLCD(int value);

/*!
 * \~english 
 * \brief Print a String at aligned position (left, center right)
 * \param line Line number
 * \param alignment left, center or right aligned
 * \param string string pointer
 */
void PrintAlignLCD(unsigned char line, unsigned char alignment, char *string);

/*!
 * \~english 
 * \brief Print String from Program Space to LCD
 * \param progmem_s String pointer
 */
void PrintLCD_p(const char *progmem_s);
#define PrintLCD_P(_str) PrintLCD_p(PSTR(_str))

/*!
 * \~english 
 * \brief Set special character from Program Space to LCD user character
 * \param AsciiCode ASCII code
 * \param progmem_s String pointer
 */
void SetCharLCD_p(unsigned char AsciiCode, const char *progmem_s);
#define SetCharLCD_P(_asc, _str) SetCharLCD_p(_asc, PSTR(_str))

/*!
 * \~english 
 * \brief poll LCD module keys
 * \return keys pressed bit coded
 */
int PollSwitchLCD (void);

/*----------- FUNCTIONS -------------------*/

#endif /* LCD_H */


