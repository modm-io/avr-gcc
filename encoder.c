/****************************************************************************/
/*!
  \file     encoder.c

  \brief    Funktionen zur Vorgabe von Fahrstrecken und Drehungen.

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
  \version  V003 - 20.02.2007 - m.a.r.v.i.n\n
            +++ GO()\n
            +++ Turn()\n
            Korrekturfakturen aus myasuro.h verwenden
  \version  V004 - 07.06.2007 - Sternthaler\n
            +++ Go()      Entfaellt\n
            +++ Turn()    Entfaellt\n
            +++ GoTurn()  NEU\n
            Die Funktion GoTurn() ersetzt die Funktionen Go() und Turn()
            ersatzlos. Um allerdings bestehende Programme nicht umbauen zu
            muessen, gibt es Macros, die dann GoTurn() aufrufen in asuro.h
  \version  V005 - 27.06.2007 - Sternthaler\n
            +++ GoTurn()\n
            Im Beispiel zur Funktion die Variablendefinition in der
            for()-Schleife fuer i entfernt, da sie nicht immer uebersetzbar
            ist.
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


/***************************************************************************
* void GoTurn(int distance, int degree, int speed)
*
*   Go's a distance in mm  OR
*   Turn's given angle.
* Attention: EncoderInit() has to be called first.
*
* the driven distance depends a little bit from the floor friction
* limitations: maximum distance +-32m
* possible problems: in full sunlight the encoder sensors may be disturbed
*
*   input
* distance: postiv->go forward; negativ->go backward; ZERO->use degree for TURN
* degree:   postiv->turn right; negativ->turn left
* speed:    sets motorspeed
*
* last modification:
*   Ver.     Date         Author           Comments
*   -------  ----------   --------------   ---------------------------------
*   sto1     29.07.2005   stochri          motorfunction
*   And1     31.07.2005   Andun            added speed and Odometrie
*   And2     07.08.2005   Andun            Added Odometrie function
*   sto2     31.10.2006   stochri          distance in mm
*   sto2     31.10.2006   stochri          added comments, corrected enc_count initialisation
*   stth     07.06.2007   Sternthaler      combine Go() and Turn() into this
*   -------  ----------   --------------   ---------------------------------
*
***************************************************************************/
/****************************************************************************/
/*!
  \brief
  Faehrt eine bestimmte Strecke mit einer bestimmten Geschwindigkeit.
  (Autor: stochri)\n
  ODER\n
  Dreht um einen bestimmten Winkel mit einer bestimmten Geschwindigkeit.
  (Autor: stochri)\n
  Benutzt die Odometrie Sensoren im Interrupt Betrieb.\n
  Vor dem ersten Aufruf muss deshalb EncoderInit() aufgerufen werden.

  \param[in]
  distance Distanz in mm (- rueckwaerts, + = vorwaerts)\n
           Bei 0 wird degree fuer einen Turn benutzt.
  \param[in]
  degree Winkel (- rechts, + links)
  \param[in]
  speed Geschwindigkeit (Wertebereich 0...255)

  \return
  nichts

  \see
  MACRO Go() und MACRO Turn() koennen weiterhin aufgerufen werden um bestehenden
  Programmcode nicht umschreiben zu muessen.

  \see
  In der globale Variable encoder, werden die Hell-/Dunkelwechsel der\n
  Encoderscheiben im Interruptbetrieb gezaehlt.

  \par  Hinweis:
  Die Berechnung der zu fahrenden Ticks beruht auf der Annahme, dass die\n
  Anzahl der schwarzen Teilstuecke und die Raddurchmesser wie bei stochri sind.\n
  (Sternthaler) Vermutung, dass der Hersteller unterschiedlich grosse Raeder\n
  ausgeliefert hat, da die Berechnung in dieser Form bei Sternthaler nicht\n
  funktioniert.

  \par Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Laesst den Asuro ein Quadrat von 200 mm fahren,
  // bei einer Geschwindigkeit von 150.
  EncoderInit ();
  for (i = 0; i < 4; i++)
  {
    GoTurn (200,  0, 150);  // entspricht Go (200, 150)
    GoTurn (  0, 90, 150);  // entspricht Turn (90, 150)
  }
  \endcode
*****************************************************************************/
void GoTurn (
  int distance,
  int degree,
  int speed)
{
  unsigned  long  enc_count;
            int   tot_count = 0;
            int   diff = 0;
            int   l_speed = speed, r_speed = speed;

  /* stop the motors until the direction is set */
  MotorSpeed (0, 0);

  /* if distance is NOT zero, then take this value to go ... */
  if (distance != 0)
  {
    /* calculate tics from mm */
    enc_count  = abs (distance) * 10000L;
    enc_count /= MY_GO_ENC_COUNT_VALUE;

    if (distance < 0)
      MotorDir (RWD, RWD);
    else
      MotorDir (FWD, FWD);
  }
  /* ... else take the value degree for a turn */
  else
  {
    /*  calculate tics from degree */
    enc_count  = abs (degree) * MY_TURN_ENC_COUNT_VALUE;
    enc_count /= 360L;

    if (degree < 0)
      MotorDir (RWD, FWD);
    else
      MotorDir (FWD, RWD);
  }

  /* reset encoder */
  EncoderSet (0, 0);

  /* now start the machine */
  MotorSpeed (l_speed, r_speed);

  while (tot_count < enc_count)
  {
    tot_count += encoder [LEFT];
    diff = encoder [LEFT] - encoder [RIGHT];

    if (diff > 0)
    { /* Left faster than right */
      if ((l_speed > speed) || (r_speed > 244))
        l_speed -= 10;
      else
        r_speed += 10;
    }

    if (diff < 0)
    { /* Right faster than left */
      if ((r_speed > speed) || (l_speed > 244))
        r_speed -= 10;
      else
        l_speed += 10;
    }
    /* reset encoder */
    EncoderSet (0, 0);

    MotorSpeed (l_speed, r_speed);
    Msleep (1);
  }
  MotorDir (BREAK, BREAK);
  Msleep (200);
}
