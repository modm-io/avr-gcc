/****************************************************************************/
/*!
  \file     globals.c

  \brief    Globale Variablen der Asuro Library.

  \par      Hinweis:
  Diese Variablen sind immer in allen Stellen im Programm nutzbar.

  \version  V--- - 10.11.2003 - Jan Grewe - DLR\n
            Original Version von der ASURO CD\n
  \version  V--- - bis zum 07.01.2007 - \n
            Bitte in Datei CHANGELOG nachsehen.\n
  \version  V001 - 13.01.2007 - m.a.r.v.i.n\n
            Zerlegte Sourcen in einzelne Dateien fuer eine echte Library.
  \version  V002 - 22.01.2007 - Sternthaler\n
            Kommentierte Version (KEINE Funktionsaenderung)
  \version  V003 - 11.07.2007 - Sternthaler\n
            Variable encoder als volatile definiert, da sie im Interrupt
            SIGNAL (SIG_ADC) benutzt wird.
  \version  V004 - 15.11.2007 - m.a.r.v.i.n\n
            Variable switched als volatile definiert, da sie im Interrupt
            SIGNAL (SIG_INTERRUPT1) benutzt wird.
  \version  V005 - 27.01.2008 - m.a.r.v.i.n\n
            Zeiger auf User Funktionen, die von Interrupt Funktionen\n
            aufgerufen werden
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
  Flag, dass der Interrupt SIG_INTERRUPT1 durch eine gedrueckte Taste\n
  ausgeloesst wurde. 0 = keine Taste, 1 = Taste gedrueckt.\n
  Kann im eigenen Programm immer \b abgefragt werden.

  \see
  Interruptfunktion SIGNAL (SIG_INTERRUPT1) in asuro.c\n
  StartSwitch(), StopSwitch(), PollSwitch() in switches.c
*****************************************************************************/
volatile int switched;



/****************************************************************************/
/*!
  \brief
  Odometriesensor Zaehler bei Interrupt Betrieb.\n
  encoder[0] = links; encoder[1] = rechts.

  \see
  Interruptfunktion SIGNAL (SIG_ADC) in asuro.c\n
  EncoderInit(), EncoderStop(), EncoderStart(), EncoderSet(), Go(),\n
  Turn() in encoder.c
*****************************************************************************/
volatile int encoder [2];



/****************************************************************************/
/*!
  \brief
  Counter fuer 36kHz.

  \see
  Interruptfunktion SIGNAL (SIG_OVERFLOW2) in asuro.c\n
  Gettime(), Sleep() in time.c
*****************************************************************************/
volatile unsigned char count36kHz;



/****************************************************************************/
/*!
  \brief
  Sytemzeit in ms.

  \see
  Interruptfunktion SIGNAL (SIG_OVERFLOW2) in asuro.c\n
  Gettime() in time.c
*****************************************************************************/
volatile unsigned long timebase;



/****************************************************************************/
/*!
  \brief
  Steuert, ob die Odometrie-Sensor Abfrage im Interrupt Betrieb laufen soll.

  \see
  Interruptfunktion SIGNAL (SIG_ADC) in asuro.c\n
  EncoderInit(), EncoderStart(), EncoderStop() in encoder_low.c
*****************************************************************************/
volatile int autoencode;

/****************************************************************************/
/*!
  \brief
  Zeiger auf User Funktion die von der Interupt Funktion aus aufgerufen wird.

  \see
  Interruptfunktion SIGNAL (SIG_OVERFLOW2) in asuro.c\n
  InitRC5(), in rc5.c
*****************************************************************************/
volatile voidFuncPtr Ovr2IntFunc; 

/****************************************************************************/
/*!
  \brief
  Zeiger auf User Funktion die von der Interupt Funktion aus aufgerufen wird.

  \see
  Interruptfunktion SIGNAL (SIG_ADC) in asuro.c\n
  EncoderInit() in encoder_low.c
*****************************************************************************/
volatile voidFuncPtr AdcIntFunc; 

