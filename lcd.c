/*!
  \file     lcd.c
   
  \brief    Funktionen zur Ansteurung eines LCD Moduls ueber die I2C Schnittstelle.
            Diese Bibliothek unterstützt HD44870 kompatible LCD Module mit einem
            I2C Port Expander Chip PCF8574P\n
            Die Pinbelegung des I2C Portexpanders ist folgende:\n 
            P0-P3  - BD4-DB7\n
            P4     - RS\n
            P4     - R/W\n
            P6     - Backlight (optional)\n
            P7     - EN\n
            Es wird zudem die I2C Emulations Bibliothek benötigt.\n

  
  \author   Rizqi Ahmad (raid_ox)
  
  \version  V001 - 17-02-2007 - Rizqi Ahmad (raid_ox)
            Erste Version
  \version  V002 - 08.04.2007 - m.a.r.v.i.n\n  
            +++ Alle Funktionen\n
            Kommentierte Version (KEINE Funktionsaenderung)
  \version  V003 - 21.01.2008 - m.a.r.v.i.n\n 
            Anpassung an ATmega168 
            neue Funktion PrintLCD_p zur Ausgabe von Strings aus dem Programmspeicher\n
            neue Funktion SetCharLCD zum Setzen von Sonderzeichen\n
            neue Funktion PollSwitchLCD zur Abfrage der Tasten des Arexx LCD Moduls

 */

/*************************************************************************************************************************************************** 
 * Library to control LCD over I²c
 *
 * LCD Library
 *
 * This Library is designed for HD44870 based LCDs with I2C expander PCF8574p. It requires I²C Master Software from Peter Fleury.
 * PIN-Assignment: P0-P3 -> DB4-DB7, P4 -> RS, P5 -> R/w, P7 -> Enable.
 * For Use with ASURO Robot
 *
 * Inspiration from I²C-LCD library by "Nico Eichelmann, Thomas Eichelmann"
 *
 * --------------------------------------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (c) 2006 Rizqi Ahmad (raid_ox)
 *
 * This software is a free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your option) any later version. You should have received a copy of the GNU
 * General Public License along with asurolib; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 **************************************************************************************************************************************************/

#include "asuro.h"
#include "lcd.h"
#include "i2c.h"

/*!
  \brief
  LCD Initialisierung
*/
void InitLCD(void)
{
  unsigned char init[] = LCD_INIT;
  unsigned char i = 0;

  SetIOLCD(OFF, LCD_EN);              // Start LCD Control, EN=0
  Msleep(1);                          // Wait LCD Ready

  // Initialize LCD
  CommandLCD( LCD_8BIT | (LCD_8BIT >> 4) );
  CommandLCD( LCD_8BIT | (LCD_4BIT >> 4) );

  while (init[i] != 0x00)
  {
    CommandLCD(init[i]);
    i++;
  }

  CommandLCD( LCD_DISPLAYON );        // Display on/off Control (Entry Display,Cursor off,Cursor not Blink)
  CommandLCD( LCD_INCREASE );         // Entry Mode Set (I/D=1 Increment,S=0 Cursor Shift)
  CommandLCD( LCD_CLEAR );            // Clear Display
  CommandLCD( LCD_HOME );             // Home Cursor
  Msleep(1);                          // Wait Initial Complete
}

/*!
  \brief
  LCD Backlight an oder ausschalten

  \param
  state Backlight an oder aus

*/
void BacklightLCD(unsigned char state)
{
  SetIOLCD(state, LCD_BL);
}

/*!
  \brief
  LCD Daten schreiben

  \param  data auszugebende Date
*/
void SetDataLCD(unsigned char data)
{
  unsigned char dataPins;             // Pin Compatibility

  // Set First Nibble Data to DataPins on PCF8574
  dataPins &= 0x00;
  dataPins |= ((data & 0x80) >> 7) << LD7;
  dataPins |= ((data & 0x40) >> 6) << LD6;
  dataPins |= ((data & 0x20) >> 5) << LD5;
  dataPins |= ((data & 0x10) >> 4) << LD4;

  SetIOLCD(OFF, LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);     // Clear old LCD Data (Bit[7..4])
  SetIOLCD(ON, dataPins);             // Strobe High Nibble Command
  SetIOLCD(ON, LCD_EN);               // Enable ON
  Msleep(1);
  SetIOLCD(OFF, LCD_EN);              // Enable OFF

  // Set Second Nibble Data to DataPins on PCF8574
  dataPins &= 0x00;
  dataPins |= ((data & 0x08) >> 3) << LD7;
  dataPins |= ((data & 0x04) >> 2) << LD6;
  dataPins |= ((data & 0x02) >> 1) << LD5;
  dataPins |= ((data & 0x01) >> 0) << LD4;

  SetIOLCD(OFF, LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);     // Clear old LCD Data (Bit[7..4])
  SetIOLCD(ON, dataPins);             // Strobe Low Nibble Command
  SetIOLCD(ON, LCD_EN);               // Enable ON
  Msleep(1);
  SetIOLCD(OFF, LCD_EN);              // Enable OFF

  Msleep(1);                          // Wait LCD Busy
}

/*!
  \brief
  LCD IO Ports schreiben

  \param  setCommand an - oder auschalten
  \param  bits  zu setzende bits
*/
void SetIOLCD(unsigned char setCommand, unsigned char bits)
{
  if (setCommand == ON)
    portLCD |= bits;
  else
    portLCD &= ~bits;
  StartI2C(LCD_DEV);
  WriteI2C(portLCD);
  StopI2C();
}

/*!
  \brief
  LCD IO Ports lesen

  \return  LCD IO Port
*/
unsigned char GetIOLCD(void)
{
  unsigned char data = 0x00;
  StartI2C(LCD_DEV+1);
  data = ReadI2C(0);
  StopI2C();
  return data;
}

/*!
  \brief
  LCD Cursor setzen

  \param  cursor Cursor Position
  \param  line Zeilen Nummer
*/
void SetCursorLCD(unsigned char cursor, unsigned char line)
{
  cursorLCD   = cursor;
  lineLCD   = line;

  if (line == 0)
    line = LCD_LINE1;
#if LCD_LINES>=2
  else if (line == 1)
    line = LCD_LINE2;
#endif
#if LCD_LINES>=3
  else if (line == 2)
    line = LCD_LINE3;
#endif
#if LCD_LINES>=4
  else if (line == 3)
    line = LCD_LINE4;
#endif
  else
    line = LCD_LINE1;

  CommandLCD(LCD_DDRAM | (line+cursor));
}

/*!
  \brief
  LCD Kommando ausgeben

  \param  command auszugebendes Kommando
*/
void CommandLCD(unsigned char command)
{
  if (command == LCD_HOME)
    lineLCD = cursorLCD = 0x00;
  SetIOLCD(OFF, LCD_RS);
  SetDataLCD(command);
}

/*!
  \brief
  LCD loeschen und Cursor auf 1 Postion der ersten Zeile

*/
void ClearLCD(void)
{
  CommandLCD(LCD_CLEAR);
  CommandLCD(LCD_HOME);
}

/*!
  \brief
  LCD Daten schreiben

  \param  data auszugebende Date

*/
void WriteLCD(unsigned char data)
{
  SetIOLCD(ON, LCD_RS);
  SetDataLCD(data);
  cursorLCD++;
}

/*!
  \brief
  LCD String ausgeben optional mit Zeilenumbruch

  \param  string auszugebender String
  \param  wrap Zeilenumbruch ja oder nein
*/
void PrintLCD(char *string, unsigned char wrap)
{
  unsigned char i = 0;
  while (string[i] != 0x00)
  {
    if (cursorLCD >= LCD_CHARS)
    {
      if (wrap)
        SetCursorLCD(0, lineLCD+1);
      else
        break;
    }
    WriteLCD(string[i]);
    i++;
  }
}

/*!
  \brief
  LCD String ausgeben an bestimmter Position

  \param  cursor Cursor Position
  \param  line Zeilen Nummer
  \param  string auszugebender String
*/
void PrintSetLCD(unsigned char cursor, unsigned char line, char *string)
{
  SetCursorLCD(cursor, line);
  PrintLCD(string, OFF);
}

/*!
  \brief
  LCD Integer Wert ausgeben

  \param  value auszugebender Integer Wert
*/
void PrintIntLCD(int value)
{
  char text[6];
  itoa(value,text,10);
  PrintLCD(text, OFF);
}

/*!
  \brief
  LCD Stringausgabe mit Ausrichtung (links, mitte, rechts)

  \param  alignment Ausrichtung (links, mitte, rechts)
  \param  line Zeilennummer
  \param  string auszugebender String

*/
void PrintAlignLCD(unsigned char alignment, unsigned char line, char *string)
{
  unsigned char i = 0;
  while (string[i] != 0x00)
    i++;
  if (alignment == RIGHT)
    PrintSetLCD(LCD_CHARS-i, line, string);
  else if (alignment == CENTER)
    PrintSetLCD((LCD_CHARS-i)/2, line, string);
  else
    PrintSetLCD(0, line, string);
}


/*!
  \brief
  LCD Stringausgabe aus dem Program Memory (Flash)

  \param  progmem_s auszugebender String

*/

void PrintLCD_p(const char *progmem_s)
{
  register char c;

  while ( (c = pgm_read_byte(progmem_s++)) ) 
  {
    WriteLCD(c);
  }
}


/*!
  \brief
  Sonderzeichen an das Display senden

  \param  
  progmem_s auszugebender String

  \param  
  AsciiCode Ascii Code des zu erzeugenden Sonderzeichens

*/

void SetCharLCD_p(unsigned char AsciiCode, const char *progmem_s)
{
	unsigned char i;
	
	CommandLCD( 0x40|(AsciiCode<<3));
	for(i=0;i<=7;i++)
	{
		WriteLCD(pgm_read_byte(progmem_s++));
	}
}

/*!
  \brief
  Abfrage der Taserdes Arexx LCD Moduls

  \return
  gibt den Wert aller gedrueckten Tasten zurueck\n
  bit kodiert
*/

int PollSwitchLCD (void)
{
  int key = 0;

  //taster pins auf input schalten
  //  OC2 PIN deaktivieren, aber 36kHz Timer weiterlaufen lassen (z.B. für Sleep(void) )
#if defined(__AVR_ATmega168__)
  TCCR2A = _BV(WGM21);
  TCCR2B = _BV(CS20);
#else
  TCCR2 = (1 << WGM21) | (1 << CS20); 
#endif
  DDRD &= ~((1<<PD2)|(1<<PD6));      // roten und gelben Taster als Eingang definieren
  DDRB &= ~(1<<PB3);                 // blauen Taster als Eingang definieren

  //taster abfragen

  key  |= (PIND&LCD_KEY_YELLOW);
  key  |= (PIND&LCD_KEY_RED);
  key  |= (PINB&LCD_KEY_BLUE);

  //taster pins auf output schalten
#if defined(__AVR_ATmega168__)
  TCCR2A = _BV(WGM20) | _BV(WGM21) | _BV(COM2A0) | _BV(COM2A1);
  TCCR2B = _BV(CS20);
#else
  TCCR2 = (1 << WGM20) | (1 << WGM21) | (1 << COM20) | (1 << COM21) | (1 << CS20);
#endif

  DDRD |= ((1<<PD2)|(1<<PD6));     // roten und gelben Taster als ausgang definieren
  DDRB |= (1<<PB3);                // blauen Taster als ausgang definieren


  //tasterwert zurückgeben

  return key;
}

