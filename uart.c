/****************************************************************************/
/*!
  \file     uart.c

  \brief    Funktionen zur seriellen Kommunikation.

            Senden und empfangen von Daten ueber die seriellen Schnittstelle.\n
            Diese Funktionen benoetigen einen laufenden Timer 2-Interrupt, da\n
            die Hardware vom Asuro und die 'Gegenstelle' getaktete Signale\n
            benötigen um sie als gueltig zu erkennen. Dieser Takt muss bei der\n
            verwendeten Hardware 36 kHz betragen. Er wird in der Init()-Funktion\n
            eingestellt und zusaetzlich in den Timer-Funktionen in time.c
            benutzt.

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
  \version  V003 - 25.07.2007 - Sternthaler (Gemeldet von helmut_w)\n
            +++ SerWrite ()\n
            + Abschalten des Senders nach der Datenuebertragung zum sparen
              von Energie.\n
            + Erklaerungsversuch fuer die Warteschleife am Ende der Funktion.
  \version  V004 - 31.07.2007 - Sternthaler\n
            +++ SerWrite ()\n
            + Erklaerungsversuch fuer die Warteschleife mit Bezug zum Forum
              unter http://www.roboternetz.de/
  \version  V005 - 14.08.2007 - m.a.r.v.i.n\n
            Magic Numbers ersetzt durch IO Register Defines
  \version  V006 - 27.03.2008 - m.a.r.v.i.n\n
            Warteschlefe am Ende SerWrite Funktion entfernt.
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
  Senden von Daten ueber die serielle Schnittstelle.
  
  Die zu senden Daten werden nicht als 0-terminierter C-String erwartet, sondern\n
  es werden grundsaetzlich so viele Zeichen gesendet wie im 2.ten Parameter\n
  angegeben werden. Deshalb sollte die Funktion eher zum Senden von Rohdaten
  verwendet werden (keine ASCII Zeichen)
  Zum Senden von Strings sollte stattdessen die Funktion SerPrint verwendet werden.
  Bei der aktuellen WinAVR Version (2007055) gibt es eine Warnung, falls ein 
  String als 1. Parameter übergeben wird

  \version  V003 - 25.07.2007 - Sternthaler (Gemeldet von helmut_w)\n
            + Abschalten des Senders nach der Datenuebertragung zum sparen
              von Energie.\n
            + Erklaerungsversuch fuer die Warteschleife am Ende der Funktion.
  \version  V004 - 31.07.2007 - Sternthaler\n
            + Erklaerungsversuch fuer die Warteschleife mit Bezug zum Forum
              unter http://www.roboternetz.de/

  \param[in]
  *data Zu sendende Daten
  \param[in]
  length Die Anzahl der zu sendenden Zeichen.

  \see
  Die Initialisierung vom Timer 2-Interrupt erfolgt in der Funktion Init().

  \see SerPrint

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Senden von Rohdaten ueber die Schnittstelle.
  unsigned char buf[4] = {0x55, 0xaa, 0xab, 0xcd);
  SerWrite (buf, 4);
  \endcode
*****************************************************************************/
void SerWrite (
  unsigned char *data,
  unsigned char length)
{
  unsigned char i = 0;

  UCSRB = (1<<TXEN);                    // Sender einschalten
  while (length > 0)                    // so lange noch Daten da sind
  {
    if (UCSRA & (1<<UDRE))
    {                                   // warten, bis der Sendebuffer leer ist
      UDR = data[i++];
      length --;
    }
  }
  while (!(UCSRA & (1<<TXC)))           // abwarten, bis das letzte Zeichen 
    ;                                   // uebertragen wurde.        

  UCSRA |= (1<<TXC);                    // transmission completed Flag setzen

  UCSRB &= ~(1<<TXEN);                  // Sender ausschalten / Powersave
}



/****************************************************************************/
/*!
  \brief
  Lesen von Daten ueber die serielle Schnittstelle
  
  Die empfangenen Daten werden in der als Pointer uebergeben Variable data\n
  dem Aufrufer zur verfuegung gestellt.\n
  Der Aufrufer bestimmt ueber den Parameter Timeout, ob diese Funktion im\n
  'blocking'- oder im 'nonblocking'-Mode laufen soll. Im 'blocking'-Mode\n
  bleibt diese Funktion auf alle Faelle so lange aktiv, bis die, uber den\n
  Parameter length, geforderte Anzahl Zeichen empfamgen wurde.

  \param[out]
  data Zeiger auf die einzulesenden Daten
  \param[in]
  length Anzahl der zu lesenden Zeichen
  \param[in]
  timeout 0 = blockierender Mode\n
          Wird hier ein Wert groesser 0 uebergeben, wird nur eine gewisse Zeit\n
          lang versucht ein weiteres Zeichen ueber die Schnittstelle zu empfangen.\n
          Kommt in dieser Zeit kein weiteres Zeichen, wird im zurueckgegeben\n
          Parameter date das erste Zeichen auf 'T' gesetzt und die Funktion\n
          kehrt zum Aufrufer zurueck.\n
          Ansonsten wird die Funktion auf alle Faelle verlassen, wenn die als\n
          Parameter length geforderte Anzahl Zeichen empfangen werden konnten.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Empfangen von 5 Zeichen. Aber spaetestens nach 20 Versuchen wieder
  // im Programm weiter machen.
  char emfangene_daten [10];

  SerRead (emfangene_daten, 5, 20);
  if (emfangene_daten [0] == 'T')
    SerWrite ("Mist, timeout", 13);
  else
    SerWrite ("5 Zeichen empfangen", 19);
  \endcode
*****************************************************************************/
void SerRead (
  unsigned char *data,
  unsigned char length,
  unsigned int timeout)
{
  unsigned char i = 0;
  unsigned int  time = 0;

  UCSRB = (1<<RXEN);                         // Empfaenger einschalten

  if (timeout != 0)
  {
    /*
      Die Funktion wird auf alle Faelle, auch ohne empfangene Daten, wieder
      verlassen. --> nonblocking mode
    */
    while (i < length && time++ < timeout)
    {
      if (UCSRA & (1<<RXC))
      {
        data [i++] = UDR;
        time = 0;
      }
    }
    if (time > timeout)
      data [0] = 'T';
  }
  else
  {
    /*
      Die Funktion wird auf alle Faelle erst nach dem Empfang von der
      vorgegebenen Anzahl Zeichen verlassen. blocking mode
    */
    while (i < length)
    {
      if (UCSRA & (1<<RXC))
        data [i++] = UDR;
    }
  }
}
