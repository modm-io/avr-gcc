/****************************************************************************/
/*!
  \file     i2c.c

  \brief    Funktionen zum Emulieren der I2C Bus.\n
            Dabei werden die Pins C2 und C3 genutzt als SCL und
            SDA. Sie sind aber in myasuro.h veraenderbar. Die
            Verwendung von I2C Bus benoetigt PullUps jeweils 4.7k Ohm.

  \see      Defines fuer die I2C in asuro.h\n

  \version  V001 - 17.02.2006 - Rizqi Ahmad (raid_ox)\n
            Original Version
  \version  V002 - 18.02.2006 - Sternthaler\n
            Keine Codeaenderung.\n
            Austausch der TAB's gegen BLANK's.
*****************************************************************************/
/*****************************************************************************
*                                                                            *
*   This program is free software; you can redistribute it and/or modify     *
*   it under the terms of the GNU General Public License as published by     *
*   the Free Software Foundation; either version 2 of the License, or        *
*   any later version.                                                       *
*                                                                            *
*****************************************************************************/
#include "asuro.h"
#include "i2c.h"



/****************************************************************************/
/*!
  \brief
  I2C Bus Initialisieren.

  \see SDA_HI, SCLHI

  \par  Funktionsweise:
  Schaltet PC2 und PC3 als Ausgang. und Setze die Pins als High

*****************************************************************************/
void InitI2C (void)
{
  SDA_DDR |= (1 << SDA);        
  SCL_DDR |= (1 << SCL);        
  SDA_HI;             
  SCL_HI;             
}



/****************************************************************************/
/*!
  \brief
  Daten ueber I2C schreiben

  \param
  byte daten

  \return
  antwort

  \see SDA_HI, SDA_LO, SCL_HI, SCL_TOGGLE, HDEL

*****************************************************************************/
unsigned char WriteI2C (unsigned char byte)
{
  unsigned char i;
  
  for (i=8; i>0; i--)
  {
    if ( byte & (1<<(i-1)) )
      SDA_HI;
    else
      SDA_LO;         
    SCL_TOGGLE;         
  }
  SDA_HI;             

  SDA_DDR &= ~(1 << SDA);
  HDEL;
  SCL_HI;
  byte = SDA_PIN & (1 << SDA);

  HDEL;
  SCL_LO;
  SDA_DDR |= (1 << SDA);
  HDEL;

  return (byte == 0);
}



/****************************************************************************/
/*!
  \brief
  Daten aus I2C Bus lesen

  \param
  ack NAK oder ACK

  \return
  erhaltene Daten

  \see SDA_HI, SDA_LO, SCL_HI, SCL_TOGGLE, HDEL

*****************************************************************************/
unsigned char ReadI2C(unsigned char ack)
{
  unsigned char i, byte = 0;
  
  SDA_HI;
  SDA_DDR &= ~(1 << SDA);
  
  for (i=0; i<8; i++)
  {
  HDEL;
    SCL_HI;
    byte <<= 1;
    byte |= (SDA_PIN & (1 << SDA)) >> SDA;
    HDEL;
    SCL_LO;
  }

  SDA_DDR |= (1 << SDA);

  if (ack)
    SDA_LO;                             // ack
  else
    SDA_HI;                             // nak

  SCL_TOGGLE;
  SDA_HI;

  return byte;
}



/****************************************************************************/
/*!
  \brief
  Datenaustausch starten

  \param
  device Addresse der I2C Gerät

  \return
  antwort

*****************************************************************************/
unsigned char StartI2C (unsigned char device)
{
  I2C_START;
  return WriteI2C (device);
}



/****************************************************************************/
/*!
  \brief
  Datenaustausch Stoppen


*****************************************************************************/
void StopI2C (void)
{
  SDA_LO;
  I2C_STOP;
}
