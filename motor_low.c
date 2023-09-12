/****************************************************************************/
/*!
  \file     motor_low.c

  \brief    Low Level Funktionen zur Steuerung der Motoren.

  Die Motorsteuerung erfolgt grundsaetzlich ueber die auf der Asuro-Platine\n
  aufgebauten H-Bruecken. Dies ist eine Schaltung, ueber die ein Strom in\n
  verschiedene Richtungen durch die Motoren geleitet werden kann.\n
  Zur Geschwindigkeitssteuerung werden die beiden im Prozessor vorhandenen\n
  PWM-Kanaele genutzt, deren Ausgangssignale die Staerke des Stromflusses in\n
  den H-Bruecken beinflusst.\n
  Die Initialisierung der PWM-Funktionalitaet erfolgt in der Funktion Init().

  \see      Defines fuer die Auswahl der ADC-Kanaele in asuro.h\n
            FWD, RWD, BREAK, FREE

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
  \version  V003 - 18.02.2007 - m.a.r.v.i.n\n
            Datei gesplitted in motor_low.c und motor.c 
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
  Steuert die Geschwindigkeit der Motoren.

  \param[in]
  left_speed  Geschwindigkeit linker Motor (Bereich 0..255)
  \param[in]
  right_speed Geschwindigkeit rechter Motor (Bereich 0..255)

  \return
  nichts

  \see
  Die Initialisierung der PWM-Funktionalitaet erfolgt in der Funktion Init().

  \par  Hinweis:
  Diese Funktion ist als 'inline'-Funktion definiert.
  
  \par  Arbeitsweise:
  Ueber die Parameter werden die beiden Kanaele der PWM-Funktionalitaet im\n
  Prozessor angesteuert. Diese Art der Geschwindigkeitsreglung beruht darauf,\n
  dass ein digitaler Output-Pin in schneller Folge an- und ausgeschaltet wird.\n
  Mit dem Parameter wird nun gesteuert wie \b lange der Strom im \b Verhaeltniss \n
  zur Zykluszeit \b angeschaltet ist.\n
  Wird z.B. ein Wert von 150 fuer einen Parameter uebergeben, dann wird fuer\n
  150 / 255-tel der Zykluszeit der Port auf 1 geschaltet und somit ist die\n
  Motorleistung entsprechend reduziert.\n
  Daraus ergibt sich auch dass der Asuro \b noch \b nicht bei einem Wert von\n
  20 fahren wird, da diese Leistung nicht ausreicht ihn 'anzuschieben'.\n
  (PWM = Pulsweitenmodulation)\n

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Setzt die Geschwindigkeit fuer den linken Motor
  // auf 150 und stoppt den rechten Motor.
  MotorSpeed (150, 0);
  \endcode
*****************************************************************************/
void MotorSpeed (
  unsigned char left_speed,
  unsigned char right_speed)
{
  OCR1A = left_speed;
  OCR1B = right_speed;
}



/****************************************************************************/
/*!
  \brief
  Steuert die Drehrichtung der Motoren.

  \param[in]
  left_dir  Richtung des linken Motors [ FWD | RWD | BREAK | FREE ]
  \param[in]
  right_dir Richtung des rechten Motors [ FWD | RWD | BREAK | FREE ]

  \return
  nichts

  \par  Hinweis:
  Diese Funktion ist als 'inline'-Funktion definiert.
  
  \par  Arbeitsweise:
  Ueber die Parameter werden die Port-Pin's zu den H-Bruecken beider Motoren so\n
  gesetzt, dass die jeweils 4 beteiligten Transitoren einer Bruecke den Strom\n
  durch die Motoren entweder 
  - FWD vorwaerts durchleiten
  - RWD rueckwaerts durchleiten
  - BREAK den Motor kurzschliessen (Bremswirkung)
  - FREE oder von der Stromversorgung trennen (Motor laeuft aus)

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Setze die Richtung fuer den rechten Motor auf Rueckwaerts
  // und blockiert den linken Motor.
  MotorDir (BREAK, RWD);
  \endcode
*****************************************************************************/
void MotorDir (
  unsigned char left_dir,
  unsigned char right_dir)
{
  PORTD = (PORTD &~ ((1 << PD4) | (1 << PD5))) | left_dir;
  PORTB = (PORTB &~ ((1 << PB4) | (1 << PB5))) | right_dir;
}
