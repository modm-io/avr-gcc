/****************************************************************************/
/*!
  \file print.c

  \brief
  Print Ausgabe Funktionen

  Die Ausgabe der Zeichen erfolgt ueber die IR LED an der seriellen Schnittstelle.\n
  Ueber den IR Transceiver koennen die Zeichen empfangen und in einem Terminalprogramm\n
  vom PC dargestellt werden.\n
  Dazu muss bei den Einstellungen des Terminalprogramms die COM Schnittstelle\n 
  ausgewaehlt werden, an der der IR Transceiver haengt.\n 
  Die weiteren Schnittstellenparameter sind:\n 
  2400 Baud, 8 Databits, no parity, 1 Stopbit, Hardware Handshake none\n
  
  \par Hinweis:
  Wer den USB IR Transceiver mit einem Terminalprogramm benutzen will,\n
  muss dazu die aktuellen USB Treiber von der FTDI Homepage verwenden.\n
  Diese CDM Treiber enthalten sowohl die VCP Treiber,\n
  die das Terminalprogramm verwendet, als auch die D2XX Treiber,\n
  die das Flasher Tool verwendet.\n
  Zu finden sind die Treiber unter http://www.ftdichip.com/FTDrivers.htm .

  \version  V--- - 10.11.2003 - Jan Grewe - DLR\n
            Original Version von der ASURO CD\n
  \version  V--- - bis zum 07.01.2007 - \n
            Bitte in Datei CHANGELOG nachsehen.\n
  \version  V001 - 13.01.2007 - m.a.r.v.i.n\n
            +++ Alle Funktionen\n
            Zerlegte Sourcen in einzelne Dateien fuer eine echte Library.
  \version  V002 - 09.02.2007 - m.a.r.v.i.n\n
            +++ PrintLong ()  NEU\n
            Ausgabe eines Long Wertes als String ueber die serielle
            Schnittstelle.\n
            +++ Alle Funktionen\n
            Kommentierte Version\n
  \version  V003 - 14.02.2007 - Sternthaler\n
            +++ Alle Funktionen\n
            Kommentare einheitlicher zu den anderen Sourcen.\n
  \version  V004 - 25.07.2007 - Sternthaler (Gemeldet von helmut_w)\n
            +++ UartPutc ()\n
            Abschalten des Senders nach der Datenuebertragung zum sparen
            von Energie.
  \version  V005 - 18.08.2007 - marvin\n
            +++ SerPrint ()\n
            signed char als Parameter wg. Compilerwarnungen
  \version  V006 - 27.01.2008 - marvin\n
            neue Funktion SerPrint_p\n
            zur Ausgabe von Strings aus dem Programmspeicher
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
  Ausgabe eines Integer Wertes als String ueber die serielle Schnittstelle.

  \param[in]
  wert Auszugebender Integer Wert (16Bit)

  \author   Robotrixer, marvin

  \version  beta - 31.03.2005 - Robotrixer\n
            first implementation
  \version  2.60 - 28.09.2005 - m.a.r.v.i.n\n
            strlen verwenden, anstelle fester Laenge
  \version  2.61 - 20.11.2006 - m.a.r.v.i.n\n
            Initialisierung text String kann zu Fehler\n
            beim Flashen mit RS232/IR Adapter fuehren\n
            (Bug report von francesco)
  \version  2.70b- 07.01.2007 - m.a.r.v.i.n\n
            SerPrint Funktion anstelle SerWrite verwenden
  \version  2.70rc2- 09.02.2007 - m.a.r.v.i.n\n
            Text Laenge auf 7 erhoeht, fuer Ausgabe negativer Werte
            (Bug Report von HermannSW)

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Beispiel fuer zeilenweise Ausgabe der Encoder Werte
  EncoderInit();
  While(1)
  {
    PrintInt (encoder [0]);
    SerPrint ("   ");
    PrintInt (encoder [1]);
    SerPrint ("\n\r");
    MSleep (500); //0,5sek. warten
  }
  \endcode
*****************************************************************************/
void PrintInt (
  int wert)
{
  char text [7];                        // "-12345"

  itoa (wert, text, 10);
  SerPrint (text);
}



/****************************************************************************/
/*!
  \brief
  Ausgabe eines Long Wertes als String ueber die serielle Schnittstelle.

  \param[in]
  wert Auszugebender Long Wert (32Bit)

  \author HermannSW, marvin

  \version  2.70rc2 - 09.02.2007    m.a.r.v.i.n\n
            first implementation
            (Idee von HermannSW)

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Beispiel fuer Ausgabe der Batterie Werte
  long data;
  While(1)
  {
    data = Batterie ();
    PrintLong (data);
    SerPrint ("\n\r");
    MSleep (500); //0,5sek. warten
  }
  \endcode
*****************************************************************************/
void PrintLong (
  long wert)
{
  char text [12];                       // '-'1234567891'\0'

  ltoa (wert, text, 10);
  SerPrint (text);
}



/****************************************************************************/
/*!
  \brief
  Sendet einen einzelnen Character über die serielle Schnittstelle

  \param[in]
  zeichen auszugebendes Zeichen

  \author   stochri

  \version  sto1 - 07.01.2006   stochri\n
            first implementation
  \version  V004 - 25.07.2007 - Sternthaler (Gemeldet von helmut_w)\n
            Abschalten des Senders nach der Datenuebertragung zum sparen
            von Energie.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // 'wunderbaren' Text als einzelne Zeichen senden.
  UartPutc ('H');
  UartPutc ('e');
  UartPutc ('l');
  UartPutc ('l');
  UartPutc ('o');
  \endcode
*****************************************************************************/
void UartPutc (
  unsigned char zeichen)
{
  UCSRB  = 0x08;                        // enable transmitter
  UCSRA |= 0x40;                        // clear transmitter flag
  while (!(UCSRA & 0x20))               // wait for empty transmit buffer
    ;
  UDR = zeichen;
  while (!(UCSRA & 0x40))               // Wait for transmit complete flag (TXC)
    ;
  UCSRB  = 0x00;                        // disable transmitter / powersave
}



/****************************************************************************/
/*!
  \brief
  Sendet einen null-terminierten String ueber die serielle Schnittstelle.
  Im Gegensatz zur SerWrite Funktion wird bei dieser Funktion kein Parameter
  mit der Anzahl der zu sendenden Zeichne benötigt. Es wird einfach bis zum
  Stringende (0-Character) gesendet.

  Zum Senden von Rohdaten (keine ASCII Zeichen) sollte sattdessen die Funktion
  SerWrite verwendet werden.

  \param[in]
  data null-terminierter String

  \see SerWrite

  \author   stochri

  \version  sto1 - 07.01.2007 - stochri\n
            first implementation
  \version  V005 - 18.08.2007 - marvin\n
            signed char als Parameter wg. Compilerwarnungen

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Beispiel fuer SerPrint
  SerPrint ("Hallo ASURO!\r\n");
  \endcode
*****************************************************************************/
void SerPrint (
  char *data)
{
  unsigned char i = 0;

  while (data [i] != 0x00)
    UartPutc (data [i++]);
}

/****************************************************************************/
/*!
  \brief
  Sendet einen null-terminierten String aus dem FLASH Speicher ueber die
  serielle Schnittstelle.
  
  \param[in]
  data null-terminierter String

  \author   marvin

  \version  V001 - 07.02.2008 - marvin\n
            first implementation
  \par Erlaeuterung:
  Alle String Variablen belegen normalerweise neben dem Speicherplatz im\n
  Programmspeicher (FLASH) zusätzlich denselben Speicherplatz im RAM.\n
  Den Speicherplatz im RAM laesst sich vermeiden, indem man den Bezeichner\n
  PROGMEM bei der Deklaration des Strings angibt.\n
  Zum Auslesen einzelner Zeichen des Strings aus dem Programmspeicher\n
  wird die Funktion pgm_read_byte aus der Standard Library verwendet.


  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Beispiel fuer SerPrint_p
  const char Text_im_FLASH[] PROGMEM = "FLASH String";
  SerPrint_p (Text_im_FLASH);
  \endcode
*****************************************************************************/
void SerPrint_p (
  const char *data)
{
  char c;

  while ((c = pgm_read_byte(data)) != 0)
  {  
    UartPutc (c);
    data++;
  }
}
