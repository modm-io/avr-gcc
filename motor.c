/****************************************************************************/
/*!
  \file     motor.c

  \brief    Funktionen zur Steuerung der Motoren.

  Die Motorsteuerung erfolgt grundsaetzlich ueber die auf der Asuro-Platine\n
  aufgebauten H-Bruecken. Dies ist eine Schaltung, ueber die ein Strom in\n
  verschiedene Richtungen durch die Motoren geleitet werden kann.\n
  Zur Geschwindigkeitssteuerung werden die beiden im Prozessor vorhandenen\n
  PWM-Kanaele genutzt, deren Ausgangssignale die Staerke des Stromflusses in\n
  den H-Bruecken beinflusst.\n
  Die Initialisierung der PWM-Funktionalitaet erfolgt in der Funktion Init().

  \see      Defines fuer die Auswahl der ADC-Kanaele in asuro.h\n
            FWD, RWD, BREAK, FREE

  \version  sto1 - 07.01.2006 - stochri\n
            +++ SetMotorPower()\n
            first implementation
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



/***************************************************************************
*
* 	sto1     07.01.2006   stochri	       first implementation
*
* control the motors
*
* input range: -127 full power backturn
*                 0 stop motors
*              +127 full power vorward
*
*       motor direction and speed can be set with one parameter
*
*	last modification:
* 	Ver.     Date         Author           Comments
* 	-------  ----------   --------------   ---------------------------------
* 	sto1     07.01.2006   stochri	       first implementation
* 	-------  ----------   --------------   ---------------------------------
****************************************************************************/
/****************************************************************************/
/*!
  \brief
  Steuert die Motorgeschwindigkeit und Drehrichtung der Motoren.

  \param[in]
  leftpwm   linker Motor (-rückwaerts, + vorwaerts) (Wertebereich -128...127)

  \param[in]
  rightpwm  rechter Motor (-rückwaerts, + vorwaerts)  (Wertebereich -128...127)

  \par  Hinweis:
  Da der Wertebereich dieser Funktion nur von -128 bis +127 reicht, aber die\n
  urspruengliche Funktion MotorSpeed() einen Wertebereich bis +255 hat, werden\n
  die hier uebergebene Parameter als Absolutwert mit 2 multipliziert\n
  weitergegeben.\n

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Setzt die Geschwindigkeit fuer den linken Motor auf 60 (vorwaerts),
  // und für den rechten Motor auf -60 (rueckwaerts)
  // Asuro soll auf der Stelle drehen.
  SetMotorPower (60, -600);
  \endcode
*****************************************************************************/
void SetMotorPower (
  int8_t leftpwm,
  int8_t rightpwm)
{
  unsigned char left, right;

  if (leftpwm < 0)                      // Ein negativer Wert fuehrt ...
  {
    left = RWD;                         // ... zu einer Rueckwaertsfahrt, ...
    leftpwm = -leftpwm;                 // aber immer positiv PWM-Wert
  }
  else
    left = FWD;                         // ... sonst nach vorne, ...
  if (leftpwm == 0)
    left = BREAK;                       // ... oder bei 0 zum Bremsen.

  if (rightpwm < 0)
  {
    right = RWD;
    rightpwm = -rightpwm;
  }
  else
    right = FWD;
  if (rightpwm == 0)
    right = BREAK;

  MotorDir (left, right);                 // Drehrichtung setzen

  /*
    Die Geschwindigkeitsparameter mit 2 multiplizieren, da der Absolutwert
    der Parameter dieser Funktion nur genau die Haelfte von der MotorSpeed()-
    Funktion betraegt.
  */
  MotorSpeed (leftpwm * 2, rightpwm * 2);
}
