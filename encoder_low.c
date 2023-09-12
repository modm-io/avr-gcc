/****************************************************************************/
/*!
  \file     encoder_low.c

  \brief    Low Level Funktionen für Radencoder und Odometrie.

  \see      Defines zum setzen von Port's und Konfigurationen in asuro.h\n
            TRUE, FALSE, LEFT, RIGHT

  \version  V--- -
            KEINE original Version von der ASURO CD vom DLR\n
  \version  V--- - bis zum 07.01.2007 - \n
            Bitte in Datei CHANGELOG nachsehen.\n
            Das Grundgeruest dieser Funktionen wurde von stochri erstellt.\n
  \version  V001 - 13.01.2007 - m.a.r.v.i.n\n
            +++ Alle Funktionen\n
            Zerlegte Sourcen in einzelne Dateien fuer eine echte Library.
  \version  V002 - 27.01.2007 - Sternthaler\n
            +++ Alle Funktionen\n
            Kommentierte Version (KEINE Funktionsaenderung)
  \version  V003 - 21.02.2007 - Sternthaler\n
            +++ EncoderStart()\n
            KEINE Funktionsaenderung\n
            Die Fehlerbeschreibung von Sternthaler ersatzlos gestrichen, da\n
            die Funktionalitaet von stochri durch das Starten des AD-Wandlers\n
            in EncoderInit() im sogenannten 'free running'-Mode gegeben ist.
  \version  V004 - 27.01.2008 - m.a.r.v.i.n\n
            Die Encoder Interrupt Funktion IsrEnc wird in der EncoderInit\n
            Funktion über die Variable AdcIntFunc in den ADC Interrupt\n
            eingebunden und von dort aufgerufen
  \version  V005 - 29.03.2009 - rossir\n
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
  Den Interrupt Betrieb der Odometriesensoren-Messung initialisieren und starten.

  \see
  autoencode, encoder

  \par  Funktionsweise:
  Globale Variable autoencode auf TRUE.\n
  Diese Funktion wird nun verlassen und das aufrufende Hauptprogramm arbeit\n
  weiter. In der Zwischenzeit ist der AD-Wandler beschaeftigt um das Mess-\n
  ergebniss zu ermitteln.\n
  Ist der Wandler fertig, wird der Interrupt zum AD-Wandler aufgerufen und in\n
  der dazu vorhandene Interrupt-Funktion aus asuro.c bearbeitet.\n
  Dort wird nun AUTOMATISCH das Messergebnis ausgewertet, ein erkannter\n
  Hell- Dunkel-Wechsel an der Encoderscheibe erkannt und dadurch der Zaehler\n
  in der globalen Variablen encoder[] weitergezaehlt.\n
  Ausserdem wird dort dann der AD-Wandler fuer die andere Seite gestartet.\n
  Da dies dann immer im Wechsel laeuft, kann das Hauptprogramm, ohne\n
  weiters Zutun von nun ab auf die Zaehlerwerte in encoder[] zugreifen.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  int main (void)
  {
    Init ();

    EncoderInit ();

    MotorDir (FWD, FWD);
    MotorSpeed (150, 150);
    while (1)
    (
      // Dein Programm

      if (encoder [0] > 500)
      {
        EncoderStop ();
        MotorSpeed (0, 0);
      }
    }
    return 0;
  }
  \endcode
*****************************************************************************/
void EncoderInit (void)
{
  /*
    Alle definierten Interrupts im Asuro sperren.
  */
  cli();

  /*
    Odometrie im Interruptbetrieb weiter bearbeiten.
  */
  autoencode = TRUE;
  
  /*
    Die Odometrie Hell-/Dunkel-Zaehler zuruecksetzen/initialisieren.
   */ 
  EncoderSet (0, 0);

  /*
    Alle definierten Interrupts im Asuro wieder zulassen.
  */
  sei();

}

/****************************************************************************/
/*!
  \brief
  Den Interrupt Betrieb der Odometriesensoren-Messung anhalten.

  \see 
  Die globale Variable autoencode hier auf FALSE gesetzt.

  \par  Funktionsweise:
  Durch das setzen der globalen Variablen autoencode auf FALSE wird in\n
  der AD-Wandler-Interruptfunktion der Teil zur Bearbeitung uebersprungen.\n
  Dadurch wird der Wandler nicht mehr neu gestartet und somit stopp die\n
  Automatik.

  \par  Beispiel:
  (siehe unter EncoderInit bzw. in den examples)
*****************************************************************************/
void EncoderStop (void)
{
  autoencode = FALSE;
}



/****************************************************************************/
/*!
  \brief
  Den Interrupt Betrieb der Odometriesensoren-Messung starten.

  \see 
  Die globale Variable autoencode hier auf TRUE gesetzt.

  \par Beispiel:
  (siehe unter EncoderInit bzw. in den examples)
*****************************************************************************/
void EncoderStart (void)
{
  autoencode = TRUE;
}



/****************************************************************************/
/*!
  \brief
  Interruptbetriebene Odometriesensoren Werte vorbelegen.

  \param[in]
  setl Wert fuer links

  \param[in]
  setr Wert fuer rechts

  \par  Hinweis:
  Initialisiert die beiden Werte in der globalen Variable encoder.\n
  Normalerweise werden die Zaehlerwerte mit 0 initialisiert. Fuer einige\n
  Anwendungen kann es sinnvoll sein auch schon bestimmte Werte vorzubelegen.

  \see
  Die globale Variable encoder wird hier initialisiert.

  \par Beispiel:
  (siehe unter den examples)
*****************************************************************************/
void EncoderSet (
  int setl,
  int setr)
{
  encoder [LEFT]  = setl;
  encoder [RIGHT] = setr;
}
