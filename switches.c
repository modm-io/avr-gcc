/****************************************************************************/
/*!
  \file     switches.c

  \brief    Funktionen zum lesen der Taster und um den 'Interrupt-Betrieb' ein-\n
            und auszuschalten. Im 'Interrupt-Betrieb' wird die globale Variable\n
            switched gesetzt beim betaetigen irgendeines Tasters.

  \see      Define fuer die Auswahl des ADC-Kanals in asuro.h\n
            SWITCH

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
  \version  V003 - 18.02.2007 - Sternthaler\n
            +++ StartSwitch()
            Korrektur im Code-Beispiel
  \version  V004 - 20.02.2007 - m.a.r.v.i.n\n
            Korrekturfaktur aus myasuro.h verwenden
  \version  V005 - 20.01.2008 - m.a.r.v.i.n\n
            ReadADC Funktion zum Auslesen des A/D Wandlers verwenden\n
            Anpassungen an ATmega168
  \version  V006 - 21.04.2008 - m.a.r.v.i.n\n
            Bug Report von RN-User thowil\n
            - Bugfix in PollSwitch Funktion fuehrt zu staendig neuen Interrupts\n
              bei Verwendung mit StartSwitch\n
            - PollSwitch kehrt jetzt sofort ohne A/D Wandlung zurueck\n
              wenn keine Taste gedrueckt wurde
  \version  V007 - 29.03.2009 - rossir\n
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
#include "myasuro.h"



/****************************************************************************/
/*!
  \brief
  Tastsensor Abfrage im 'Polling-Betrieb'.

  \return
  Tastenwert bitorientiert, K1 = Bit5, K2 = Bit4, K3 = Bit3, K4 = Bit2,
  K5 = Bit1, K6 = Bit0

  \par  Hinweis:
  In dieser Funktion ist ein Sleep() Aufruf vorhanden. Der wird benoetigt\n
  damit IsrStandard() genügend Zeit bekommt so, dass der zweite PollSwitch aufruf
  auch garantiert einen neuen Wert zur Verfügung hat.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  uint8_t t1, t2;
  unsigned char text [16];

  while (1)
  {
    t1 = PollSwitch ();
    t2 = PollSwitch ();
    // 2x PollSwitch aufrufen und beide Rueckgabewerte auf Gleichheit ueberpruefen
    if (t1 && t2 && t1 == t2)           // irgendeine Taste gedrueckt 
    {
      itoa (t1, text, 10);              // Tastenwert senden 
      SerPrint (text);
      SerPrint ("\r\n");                // Zeilenvorschub 
    }
    Msleep (500);                       // 0,5 sek warten
  }
  \endcode
*****************************************************************************/
unsigned char PollSwitch (void)
{
  unsigned int i;
  
  if (!switched) return 0;        // Kehrt sofort zurück, falls kein Taster gedrueckt ist 
  
  i = ReadADC(SWITCH, 15);  // sleep 15 ms -> Rückwärtskompatibel?! und wg. timing da draussen und drinnen (IsrStandard())

  if (i>=MY_SWITCH_THRESHHOLD) return 0;	// Eher rauschen -> raus hier!
  
  // Es wurde "ganz sicher" eine Taste gedrückt.
  /*
    Die Original Umrechenfunktion von Jan Grewe - DLR wurde ersetzt durch
    eine Rechnung ohne FLOAT-Berechnungen.
  return  ((unsigned char) ((( 1024.0/(float)i - 1.0)) * 61.0 + 0.5));

    Wert 61L evtl. anpasssen, falls fuer K1 falsche Werte zurueckgegebn werden.
  */
  return ((10240000L / (long)i - 10000L) * MY_SWITCH_VALUE + 5000L) / 10000;
}



/****************************************************************************/
/*!
  \brief
  'Interrupt-Betrieb' zur Tastsensor Abfrage einschalten.

  \par  Hinweis:
  Ueber die globale Variable switched kann nach diesem Funktionsaufruf im\n
  Hauptprogramm geprueft werden, ob eine Taste gedrueckt wurde und kann dann\n
  bei Bedarf die Funktion PollSwitch() aufrufen.\n

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  StartSwitch ();
  while (!switched)         // wartet auf Tastendruck
    ;
  // an dieser Stelle kann mit Pollswitch geprüft werden
  // welche Taste gedrückt wurde, wenn nötig.
  switched = FALSE;         // Vorbereitung für neuen Aufruf von StartSwitch()
  \endcode
*****************************************************************************/
void StartSwitch (void)
{
	// ADC Wert von Switchen wird immer abgefragt
	// siehe ISRStandard() dort auch das handling von switched=TRUE
	cli ();
	switched = FALSE;
	sei (); 
}




/****************************************************************************/
/*!
  \brief
  Die globale Variable switched wird nicht mehr automatisch bei einem\n
  Tastendruck gesetzt.

  \par  Beispiel:
  (Nicht vorhanden)
*****************************************************************************/
void StopSwitch (void)
{
	// ADC Wert von Switchen wird immer abgefragt
}
