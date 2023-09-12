/****************************************************************************/
/*!
  \file     my_motor_low.c

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

  \version  V001 - 05.02.2007 - Sternthaler\n
            +++ MotorSpeed()\n
            Der Wert vom Define MY_MOTOR_DIFF wird zur Haelfte auf die beiden
            Motoren verteilt. Es werden die Grenzen von 0 und 255
            beruecksichtigt. So lange es geht, wird die angegebene Differenz
            aus MY_MOTOR_DIFF erhalten.
            V002 - 27.03.2008 - m.a.r.v.i.n\n
            Funktion in eigenen Datei abgesplittet und in MyMotorSpeed umbenannt.
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
  Steuert die Geschwindigkeit der Motoren.

  \param[in]
  left_speed  Geschwindigkeit linker Motor (Bereich 0..255)
  \param[in]
  right_speed Geschwindigkeit rechter Motor (Bereich 0..255)

  \return
  nichts

  \see
  Die Initialisierung der PWM-Funktionalitaet erfolgt in der Funktion Init().

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
  Der Define MY_MOTOR_DIFF aus myasuro.h wird jeweils zur Haelfte auf die\n
  beiden Motoren verteilt. Es werden die Grenzen von 0 und 255 beruecksichtigt.\n
  So lange es geht, wird die angegebene Differenz aus MY_MOTOR_DIFF erhalten.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Setzt die Geschwindigkeit fuer den linken Motor
  // auf 150 und stoppt den rechten Motor.
  MotorSpeed (150, 0);
  \endcode
*****************************************************************************/
void MyMotorSpeed (
  unsigned char left_speed,
  unsigned char right_speed)
{
           int  l_speed;
           int  r_speed;

  if (MY_MOTOR_DIFF != 0)
  {
    /*
      Das Ganze macht nur Sinn, wenn mindestens ein Motor fahren soll.
    */
    if (left_speed  > 0  ||
        right_speed > 0)
    {
      /*
        Bei positivem MY_MOTOR_DIFF wird der rechte Motor verstaerkt.
        Bei negativem MY_MOTOR_DIFF wird der linke  Motor verstaerkt.
      */
      l_speed = left_speed  - MY_MOTOR_DIFF / 2;
      r_speed = right_speed + MY_MOTOR_DIFF / 2;

      /*
        Wertebereich von 0 bis 255 pruefen.
        Unterscheiden nach positiver, bzw. negativer Vorgabe.
      */
      if (MY_MOTOR_DIFF > 0)
      {
        /*
          Bei positivem MY_MOTOR_DIFF kann nur die rechte Seite > 255 werden.
        */
        if (r_speed > 255)
        {
          /*
            Um die Differenz von MY_MOTOR_DIFF weiterhin zu erhalten, wird nun
            die linke Seite noch weiter reduziert, und die rechte Seite auf
            das zulaessige Maximum gesetzt.
          */
          l_speed -= (r_speed - 255);
          r_speed  = 255;
        }
        if (l_speed < 0)
        {
          /*
            Die linke Seite ist nun unter dem zulaessige Minimum von 0.
            Auch dies muss korrigiert werden.
          */
          l_speed = 0;
        }
      }
      else
      {
        /*
          Bei negativem MY_MOTOR_DIFF kann nur die linke Seite > 255 werden.
        */
        if (l_speed > 255)
        {
          /*
            Um die Differenz von MY_MOTOR_DIFF weiterhin zu erhalten, wird nun
            die rechte Seite noch weiter reduziert, und die linke Seite auf
            das zulaessige Maximum gesetzt.
          */
          r_speed -= (l_speed - 255);
          l_speed  = 255;
        }
        if (r_speed < 0)
        {
          /*
            Die rechte Seite ist nun unter dem zulaessige Minimum von 0.
            Auch dies muss korrigiert werden.
          */
          r_speed = 0;
        }
      }

      left_speed  = l_speed;
      right_speed = r_speed;
    }
  }

  MotorSpeed(left_speed, right_speed);
}


