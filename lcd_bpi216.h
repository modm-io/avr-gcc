/*!
 * \~english 
 * \file lcd_bpi216.h
 * \author Matthias Stark
 * \version 2012-09-28
 *
 * \brief Library to control LCD over a serial interface.
 *
 * This Library is designed for BPI-216 LCD, that has a serial interface.
 * The display has 2 lines whith 16 characters in each line.
 * This library is used to send control commands to the display. To write Text to the display you need to use use the commands for the serial interface.
 */

#include "asuro.h"

/*!
 * \~english 
 * \brief Clear LCD
 */
void LCD_clear(void);

/*!
 * \~english 
 * \brief Sets the cursor to the position where you want to start writting
 * \param line The line you want to use. Counting starts at 1.
 * \param pos The position in the line. Counting starts at 1.
 */
void LCD_moveTo(unsigned char line, unsigned char pos);
