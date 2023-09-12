/****************************************************************************/
/*!
  \file     time.c

  \brief    Zeit- und Delay Funktionen.

            Erzeugt Zeiteinheiten anhand des laufenden Timer 2-Interrupts.\n
            Dieser ist auf 36 kHz eingestellt um die Hardware der serielle\n
            Schnittstelle zu bedienen (siehe in asuro.c unter
            SIGNAL (SIG_OVERFLOW2)).\n
            Die globalen Variablen \b count36kHz und \b timebase werden dort
            bearbeitet.\n
            In den hier vorhandenen Funktionen werden diese Variablen genutzt\n
            um Zeiten und Verzoegerungen zu erzeugen.

  \see      Defines fuer unseren Asuro immer in asuro.h\n
            Hier werden aktuell keine Defines benutzt.

  \version  V--- - 10.11.2003 - Jan Grewe - DLR\n
            Original Version von der ASURO CD\n
  \version  V--- - bis zum 07.01.2007 - \n
            Bitte in Datei CHANGELOG nachsehen.\n
  \version  V001 - 13.01.2007 - m.a.r.v.i.n\n
            +++ Alle Funktionen\n
            Zerlegte Sourcen in einzelne Dateien fuer eine echte Library.
  \version  V002 - 22.01.2007 - Sternthaler\n
            +++ Alle Funktionen\n
            Kommentierte Version (KEINE Funktionsaenderung)
  \version  V003 - 26.03.2010 - Sternthaler\n
            +++ Msleep()\n
            Parameter von "int" auf "unsigned int" umgestellt. (Tip von Valen)
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
  Gibt die aktuelle Zeit in ms zurueck.

  Da der Asuro keine Atomuhr hat, ist es die vergangene Zeit seitdem er
  eingeschaltet wurde.\n
  Genauer: nachdem der Interrupt Timer2 aktiviert wurde.

  \return
  Einschaltzeit in Millisekunden (Bereich: unsigned long 0..286331153)\n
  Das sind ca. 79.5 Stunden. Fuer die, die ihren Asuro also ohne Solarzellen\n
  betreiben, reicht diese Zeitangabe bevor der Accu leer ist.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Alle 500 ms die Front-LED umschalten.
  unsigned long zeit;
  unsigned char on_off = TRUE;

  zeit = Gettime ();
  while (1)
  {
    if (Gettime () > zeit + 500)
    {
      zeit = Gettime ();
      FrontLED (on_off);
      on_off ^= 1;
    }
  }
  \endcode
*****************************************************************************/
unsigned long Gettime (void)
{
  return ((timebase * 256) + count36kHz) / 36;
}



/****************************************************************************/
/*!
  \brief
  Wartefunktion.

  Die maximale Wartezeit betraegt 7ms. Fuer laengere Wartezeiten siehe Msleep().\n
  Diese Funktion nutzt den Timer 2-Interrupt um ein 'zeitgefuehl' zu erhalten.\n
  Der Interrupt wird mit 36 kHz, durch die Init()-Funktion initialisiert,\n
  aufgerufen und zaehlt dort die globale Variablen \b count36kHz weiter.\n
  Diese Funktion nutzt diesen Zaehler und berechnet daraus mit dem uebergeben\n
  Parameter den Zeitpunkt wann die Pausenzeit erreicht ist, Danach bricht sie\n
  ab, und im Hauptprogramm ist eben eine Wartezeit eingelegt worden.

  \param[in]
  time36kHz Wartezeit x/36kHz (sec)

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // 1 Millisekunde warten
  Sleep (36);
  \endcode
*****************************************************************************/
void Sleep (
  unsigned char time36kHz)
{
  unsigned char ziel = (time36kHz + count36kHz) & 0x00FF;

  while (count36kHz != ziel)
    ;
}



/****************************************************************************/
/*!
  \brief
  Wartefunktion in ms.

  Diese Funktion nutzt die Sleep()-Funktion um mit dem uebergeben Parameter\n
  Pausen in ms-Einheiten zu erhalten.

  \param[in]
  dauer Wartezeit in Millisekunden.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // 1.5 Sekunde warten
  Msleep (1500);
  \endcode
*****************************************************************************/
void Msleep (
  unsigned int ms)
{
  unsigned int z;
  for (z = 0; z < ms; z++)
    Sleep (36);
}
