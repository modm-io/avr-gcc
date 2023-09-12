/****************************************************************************/
/*!
  \file     leds.c

  \brief    Funktionen zur Steuerung der LED's.

  Die Port-Pin's zu den LED's werden so manipuliert, dass sie die daran\n
  angeschlossenen LED's mit Strom versorgen oder eben nicht. Dazu muessen die\n
  entsprechenden Ports als Ausgang konfiguriert sein. Dies erfolgt entweder\n
  schon in der Init()-Funktion, oder wird im Falle der BackLED()-Funktion\n
  dort vorgenommen.

  \see      Defines fuer die Auswahl der ADC-Kanaele in asuro.h\n
            ON, OFF, GREEN, YELLOW, RED

  \version  V--- - 10.11.2003 - Jan Grewe - DLR\n
            Original Version von der ASURO CD\n
  \version  V--- - bis zum 07.01.2007 - \n
            Bitte in Datei CHANGELOG nachsehen.\n
  \version  V001 - 13.01.2007 - m.a.r.v.i.n\n
            +++ Alle Funktionen\n
            Zerlegte Sourcen in einzelne Dateien fuer eine echte Library.
  \version  V002 - 05.02.2007 - Sternthaler\n
            +++ Alle Funktionen\n
            Kommentierte Version (KEINE Funktionsaenderung)
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
  Steuert die (lustige) mehrfarbige Status-LED.

  \param[in]
  color Zu setzende Farbe. [ OFF | GREEN | RED | YELLOW ]
        Bei einem nicht definierten Wert von 'color' aendert sich nichts an
        der LED.

  \return
  nichts

  \par  Hinweis:
  Diese Funktion ist als 'inline'-Funktion definiert.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Schaltet die Status-LED abwechselnd auf gruen und rot. 
  while (1)
  { 
    StatusLED (GREEN);
    Msleep (500);
    StatusLED (RED);
    MSleep (500);
  }
  \endcode
*****************************************************************************/
void StatusLED (
  unsigned char color)
{
  if (color == OFF)
  {
    GREEN_LED_OFF;
    RED_LED_OFF;
  }
  if (color == GREEN)
  {
    GREEN_LED_ON;
    RED_LED_OFF;
  }
  if (color == YELLOW)
  {
    GREEN_LED_ON;
    RED_LED_ON;
  }
  if (color == RED)
  {
    GREEN_LED_OFF;
    RED_LED_ON;
  }
}



/****************************************************************************/
/*!
  \brief
  Steuert die vorne, nach unten zeigende, Front-LED.

  \param[in]
  status Schaltet die LED an bzw. aus. [ ON | OFF ]

  \return
  nichts

  \par  Hinweis:
  Diese Funktion ist als 'inline'-Funktion definiert.

  \par  Achtung:
  Der uebergeben Parameter wird nicht geprueft, und kann evl. zu unerwarteten\n
  Reaktionen fuehren, da der Port D anhand dieses Wertes manipuliert wird.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // schalte die Front-LED an.
  FrontLED (ON);
  \endcode
*****************************************************************************/
void FrontLED (
  unsigned char status)
{
  PORTD = (PORTD &~(1 << PD6)) | (status << PD6);
}



/****************************************************************************/
/*!
  \brief
  Steuert die beiden hinteren Back-LED's\n
  Wenn diese Funktion aufgerufen wird, funktioniert die Odometriemessung\n
  \b nicht mehr, da die gleichen Port-Pins (Port C:Pin 0 und 1) des Prozessors\n
  hierfuer verwendet werden.

  \param[in]
  left  Schaltet die linke LED an bzw. aus. [ ON | OFF ]
  \param[in]
  right Schaltet die rechte LED an bzw. aus. [ ON | OFF ]

  \return
  nichts

  \par  Hinweis:
  Obwohl die uebergebenen Parameter nicht geprueft werden, treten hier keine\n
  unerwarteten Reaktionen am Port C auf.\n

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Linke LED aus; Rechte LED an
  BackLED (OFF, ON);
  \endcode
*****************************************************************************/
void BackLED (
  unsigned char left,
  unsigned char right)
{
  if (left || right)
  {
    PORTD &= ~(1 << PD7);               // Rad-LED's OFF
    DDRC |= (1 << PC0) | (1 << PC1);    // Port als Output => KEINE Odometrie
    PORTC |= (1 << PC0) | (1 << PC1);
  }
  if (!left)
    PORTC &= ~(1 << PC1);
  if (!right)
    PORTC &= ~(1 << PC0);
}
