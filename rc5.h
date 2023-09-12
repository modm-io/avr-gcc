/*
 * rc5.h Infrarot Fernbedienung Definitionen 
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
  /file     rc5.h
  /brief    Definitionen fuer die Dekodierung von RC5-Fernbedienungs-Codes
  /author   Benjamin Benz (bbe@heise.de), m.a.r.v.i.n

  /version      V001 - 20.12.2005 - Benjamin Benz (bbe@heise.de)
                Version fuer den c't-Bot
                V002 - 11.02.2007 - m.a.r.v.i.n
                portiert fuer den ASURO
*/
#ifndef RC5_H
#define RC5_H

#include <inttypes.h>

#define RC5_TOGGLE  0x0800    /*!< Das RC5-Toggle-Bit */
#define RC5_ADDRESS 0x07C0    /*!< Der Adressbereich */
#define RC5_COMMAND 0x103F    /*!< Der Kommandobereich */

#define RC5_MASK (RC5_COMMAND)

extern volatile uint16_t  RC5data;     /*!< letztes komplett gelesenes RC5-Paket */
extern volatile uint8_t   enableRC5;   /*!< schaltet die RC5 Abfrage ein/aus */

/*!
 * \brief Initialize the RC5 Receiver
 */
void InitRC5(void);

/*!
 * \brief Read last received RC5 Data
 * \return last received RC5 Data
 */
uint16_t ReadRC5(void);

/*!
 * \brief RC5 interrupt service routine\n
 * called by timer overflow isr ever 27,7 micro seconds
 */
void IsrRC5(void);
#endif  /* RC5_H */
