/****************************************************************************/
/*!
  \file     adc_low.c

  \brief    Low Level Funktion zum Auslesen der ADC-Wandler.

  \version  V001 - 20.01.2008 - m.a.r.v.i.n\n
  \version  V002 - 29.03.2009 - rossir\n
            A/D Wandler Abfrage ueber Interrupt  
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

/****************************************************************************/
/*!
  \brief
  Liest die A/D Multiplexer Interupt Werte des gewuenschten A/D Kanals\n

  \param  mux Nummer des A/D Kanal Multiplexer
  \param  sleep optionale Wartezeit

  \return
  10 Bit A/D Wert (Bereich 0..1023)
*****************************************************************************/

unsigned int ReadADC(unsigned int mux, unsigned int sleep)
{  
	unsigned int adc;
	
  if (sleep)
    Sleep(sleep);

	cli();
	adc=adcValue[mux];
	sei();
	
	return adc>>6;
}

