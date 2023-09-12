/*
 * rc5.c Infrarot Fernbedienung Funktionen
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your
 * option) any later version.
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 */

/*! 
  /file     rc5.c
  /brief    Funktionen fuer die Dekodierung von RC5-Fernbedienungs-Codes
  /author   Benjamin Benz (bbe@heise.de),\n
            m.a.r.v.i.n

  /version      V001 - 20.12.2005 - Benjamin Benz (bbe@heise.de)\n
                Version fuer den c't-Bot
  /version      V002 - 11.02.2007 - m.a.r.v.i.n\n
                portiert fuer den ASURO
  /version      V003 - 22.01.2008 - m.a.r.v.i.n\n
                Ovr2IntFunc Variable als Zeiger auf die Funktion IsrRC5\n
                Wird von der Timer Overflow Interrupt Funtkion aus angesprungen. 
*/

// Infos ueber RC6: http://www.xs4all.nl/~sbp/knowledge/ir/rc6.htm
// http://www.xs4all.nl/~sbp/knowledge/ir/ir.htm

// ========================================================================
// RC5 Infrarot-Empfaenger
// ========================================================================
#include <avr/io.h>
#include "asuro.h"
#include "rc5.h"


// -----------------------------------------------------------------------------
// Timing
// -----------------------------------------------------------------------------
#define IR_SAMPLES_PER_BIT     8   /*!< 8 Samples per Bit */
#define IR_SAMPLES_PER_BIT_EARLY 6  /*!< Flanke fruehestens nach 7 Samples */
#define IR_SAMPLES_PER_BIT_LATE 10  /*!< Flanke spaetestens nach 9 Samples */
#define IR_SAMPLES_PER_BIT_MIN   3  /*!< Flanke vor 3 Samples -> paket verwerfen */
#define IR_PAUSE_SAMPLES       250  /*!< Startbit ist erst nach 200 Samples ohne */
// Pegelaenderung gueltig -- eigentlich muesste
// man rund 500 Samples abwarten (50 x
// Bitzeit), doch weil der Samplezaehler ein
// Byte ist, beschraenken wir uns hier auf ein
// Minimum von 250 Samples

#define IR_PORT   PORTD     /*!< Port D */
#define IR_DDR    DDRD      /*!< DDR of Port D */
#define IR_PINR   PIND      /*!< Port D input */
#define IR_PIN    PD0       /*!< Pin 0 */


static uint8_t     RC5lastsample = 0;  /*!< zuletzt gelesenes Sample */
static uint8_t     RC5bittimer   = 0;  /*!< zaehlt die Aufrufe von ir_isr() */

static uint16_t    RC5data_tmp = 0;    /*!< RC5-Bitstream */
static uint8_t     RC5bitcount = 0;    /*!< anzahl gelesener bits */

volatile uint16_t  RC5data = 0;        /*!< letztes komplett gelesenes RC5-paket */
volatile uint8_t   enableRC5 = 0;      /*!< schaltet die RC5 Abfrage ein/aus */

/*!
 * Interrupt Serviceroutine
 * wird alle 27.7us aufgerufen
 */
void IsrRC5 (void)
{
  // sample lesen
  uint8_t sample = 1;
  if (enableRC5 && !(count36kHz % 8)) 
  {  
    if ((IR_PINR & (1<<IR_PIN)) != 0)
    {
      sample = 0;
    }

    // bittimer erhoehen (bleibt bei 255 stehen)
    if (RC5bittimer<255)
    {
      RC5bittimer++;
    }

    // flankenerkennung
    if ( RC5lastsample != sample)
    {
      if (RC5bittimer<=IR_SAMPLES_PER_BIT_MIN)
      {
        // flanke kommt zu frueh: paket verwerfen
        RC5bitcount=0;
      }
      else
      {
        // Startbit
        if (RC5bitcount==0)
        {
          if ( (sample==1) && (RC5bittimer>IR_PAUSE_SAMPLES) )
          {
            // Startbit speichern
            RC5data_tmp = 1;
            RC5bitcount++;
          }
          else
          {
            // error
            RC5data_tmp = 0;
          }

          // bittimer-reset
          RC5bittimer = 0;

        // Bits 2..14: nur Flanken innerhalb des Bits beruecksichtigen
        }
        else
        {
          if (RC5bittimer >= IR_SAMPLES_PER_BIT_EARLY)
          {
            if (RC5bittimer<=IR_SAMPLES_PER_BIT_LATE)
            {
              // Bit speichern
              RC5data_tmp = (RC5data_tmp<<1) | sample;
              RC5bitcount++;
            }
            else
            {
              // zu spaet: paket verwerfen
              RC5bitcount = 0;
            }

            // bittimer-reset
            RC5bittimer = 0;
          }
        }
      }

    }
    else
    {
      // keine flanke innerhalb bitzeit?
      if (RC5bittimer > IR_SAMPLES_PER_BIT_LATE)
      {
        // 14 bits gelesen?
        if (RC5bitcount==14)
        {
          RC5data = RC5data_tmp;
        }
        // paket verwerfen
        RC5bitcount = 0;
      }
    }

    // sample im samplepuffer ablegen
    RC5lastsample = sample;
  }

}


/*!
 * IR-Daten lesen
 * @return wert von ir_data, loescht anschliessend ir_data
 */
uint16_t ReadRC5 (void)
{
  uint16_t retvalue = RC5data;
  RC5data = 0;
  return retvalue;
}

/*!
 * Init IR-System
 */
void InitRC5 (void)
{
  IR_DDR  &= ~IR_PIN;   // Pin auf Input
  IR_PORT |= IR_PIN;    // Pullup an
  enableRC5 = 1;
  Ovr2IntFunc = IsrRC5;
}


