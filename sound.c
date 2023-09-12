/****************************************************************************/
/*!
  \file     sound.c

  \brief    Soundausgabe Funktionen

  Mit den Motoren des ASUROs lassen sich auch Töne erzeugen.\n
  Das Prinzip dahinter ist folgendes:\n
  Die Frequenz des Tons wird durch umschalten der Motor Drehrichtung bestimmt.\n
  Die Lautstärke wird über die Geschwindigkeit der Motoren bestimmt.\n
  Theoretisch liessen sich damit auch Stereo Signale erzeugen, da es ja zwei\n
  Motoren gibt, die sich auch getrennt ansteuern lassen. Dazu müsste die\n
  Sound Funktion allerdings umgeschrieben werden.

  \see      Define fuer die Steuerung der Motoren in asuro.h\n
            FWD, RWD

  \version  sto2 - 01.09.2006 - stochri\n
            first version
  \version  V001 - 09.02.2007 - m.a.r.v.i.n\n
            +++ Alle Funktionen\n
            Kommentierte Version (KEINE Funktionsaenderung)
  \version  V002 - 18.02.2007 - Sternthaler\n
            +++ Alle Funktionen\n
            Einheitliche Formatierung zu den anderen Sourcen.
  \version  V003 - 26.06.2007 - stochri\n
            Bugfix Fehler in der Soundlaenge (max. 250ms)  
            
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
  Soundausgabe ueber die Motoren.

  \param[in]
  freq          Frequenz in Hz
  \param[in]
  duration_msec Laenge in Millisekunden
  \param[in]
  amplitude     Amplitude

  \return
  nichts

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Laesst den Asuro einen Ton von 1kHz für eine 1/2 Sekunde
  // mit max. Amplitude (255) spielen.
  Sound (1000, 500, 255);
  \endcode
*****************************************************************************/
void Sound (
  uint16_t freq,
  uint16_t duration_msec,
  uint8_t  amplitude)
{
  uint16_t wait_tics;
  uint32_t n,k,period_usec,dauer_usec;

  period_usec = 1000000L / freq;
  dauer_usec = 1000 * (uint32_t) duration_msec;
  k = dauer_usec / period_usec;

  //IR Interuptfreq=36KHz
  //Wavefreq=18KHz

  wait_tics = 18000 / freq;

  MotorSpeed (amplitude, amplitude);

  for (n = 0; n < k; n++)
  {
    MotorDir (FWD, FWD);
    Sleep (wait_tics);
    MotorDir (RWD, RWD);
    Sleep (wait_tics);
  }
  MotorSpeed (0, 0);
}

#define BEEP sound (1000, 100, 255)
