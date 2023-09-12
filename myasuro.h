/****************************************************************************/
/*!
  \file  myasuro.h

  \brief Benutzerspezische Definitionen der ASURO Bibliothek


  \par Tasterabfrage:

  Korrekturwert fuer Tasterabfrage in der PollSwitch Funktion.
  Der Wert wird durch Probieren ernmittelt.
  Die korrekte Funktion kann dann mit dem dem Tastertest Programm ermittelt werden.
  Als Wertebereich sollte man 61..65 durchprobieren, bis alle Taster korrekte Werte liefern.
  K1=32, K2=16, K3=8, K4=4, K5=2, K6=1

  \par Odometrie/Encoder:

  Korrekturwerte fuer Odometrie/Encoder in der SIG_ADC Interrupt Funktion.
  Die Werte können mit dem OdometrieTest Programm ermittelt werden.
  Dabei wird eine Messreihe für den linken und rechten Encoder angelegt.
  In einem Tabbelverarbeitungsprogramm (Excel o.ä.) kann die Messreihe eingelesen
  und grafisch dargestellt werden.
  Die Schwellwerte für hell und dunkel können dann

  Hinweis:

  Beim Messen der Werte ist das ADLAR Bit des A/D Wandler auf 1 gesetzt.
  Das bedeutet, der A/D Wert wird rechts aligned in das ADH/ADL Register abgelegt.
  ADH enthält die obersten 8 Bit. ADL die untersten 8 Bit.
  Für die Messung werden vom A/D Wandler Wert nur die 8 obersten Bits (ADH) berücksichtigt.
  Die beiden niederwertigsten Bits (ADL) entfallen.

  \par Go und Turn Funktion

  Korrekturwerte fuer die Go und Turn Funtion.
  Die Werte sind abhaengig von den verwendeten Encoder Scheiben (8 oder 12 Segmente)

  \version  V001 - 09.02.2007 - m.a.r.v.i.n\n
            Initiale Version
  \version  V002 - 18.02.2007 - Sternthaler\n
            Neuer Define\n
            MY_MOTOR_DIFF zum ausgleichen unterschiedlicher Motoren.
  \version  V003 - 20.02.2007 - m.a.r.v.i.n\n
            Kommentare aus my struktur uebernommen
  \version  V004 - 12.01.2008 - Sternthaler
            Kommentar zum Define MY_ASURO_DIFF angepasst.
  \version  V005 - 29.03.2009 - rossir\n
            A/D Wandler Abfrage ueber Interrupt  
*****************************************************************************/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   any later version.                                                    *
***************************************************************************/

#ifndef MYASURO_H
#define MYASURO_H

/* Tastaturabfrage */
/*! Faktor zur Berechnung der gedrueckten Tasten.\n
      Der Originalwert ist \b 61L und koennten im Bereich zwischen ca. 58L und
      65L schwanken. Dieser Wert gleicht Toleranzen der Wiederstaende an den
      Tastern aus.
*/
#define MY_SWITCH_VALUE           62L   /*!< Multiplikator fuer Tasterwerte */

/*! Schwellwert der gedrueckten Tasten.\n
      Erst unterhalb (kleiner) dieses Wertes wird ein Switches-ADC-Wert
      "ernst" ( PollSwitch()>0 ) genommen. Darüber ist es, insbesondere im hier
      angestrebten Interruptbetrieb, eher Rauschen und sollte von PollSwitch() 
      als 0 bewertet werden.
      Problematisch dabei ist, dass laufende Motoren bzw. Motorlast den Messwert, 
      auch bei nicht gedrückten Tasten, nach unten verschieben und PollSwitch() 
      liefert falsche 1 Werte wenn MY_SWITCH_THRESHHOLD nicht sorgfältig gewählt wird.
*/
#define MY_SWITCH_THRESHHOLD	1010   /*!< Schwellwert fuer Tasterwerte */

/* Odometrie / Encoder */
/* Werte für 12 Segmente Encoder */
/*! Faktor zur Berechnung von Ticks um aus den in mm angegebenen Parameter
    umzurechnen.\n
    Der Originalwert ist \b 19363L und ist von der Anzahl der schwarz/weiss
    Teilstuecke auf den Odometriescheiben abhaengig.\n
    Der Originalwert wurde durch stochri ermittelt.
*/
#define MY_GO_ENC_COUNT_VALUE  19363L   /*!< GO Funktion, Divisor fuer Entfernung   */
/*! Faktor zur Berechnung von Ticks um aus den in Grad angegebenen Parameter
    umzurechnen.\n
    Der Originalwert ist \b 177L und ist von der Anzahl der schwarz/weiss
    Teilstuecke auf den Odometriescheiben abhaengig.\n
    Der Originalwert wurde durch stochri ermittelt.
*/
#define MY_TURN_ENC_COUNT_VALUE  177L   /*!< Turn Funktion, Mutiplikator fuer Winkel */

/* Werte zum ausgleichen unterschiedlicher Motoren */
/*! Differenzangabe zwischen den beiden Motoren.\n
    Der angegebene Wert verteilt sich je zur \b Haelfte auf die Vorgaben fuer
    die Motorgeschwindigkeit.\n
    Bei einem positiven Wert, wird der rechte Motor verstaerkt.\n
    Bei einem negativen Wert, wird der linke Motor verstaerkt.
  */
#define MY_MOTOR_DIFF              0    /*!< 1/2 PLUS fuer Rechts, 1/2 MINUS fuer Links */

/* I2C Bus Definitionen */

//#define MY_I2C_SDA     PC3
//#define MY_I2C_SCL     PC2

/* Fuer das LCD Modul aus dem Asuro Buch gilt folgende Pinbelegung */
#define MY_I2C_SDA     PC2
#define MY_I2C_SCL     PC3

/* LCD Definitionen */

/*
#define MY_LCD_LD4			0				// Pin to Data Bus 4
#define MY_LCD_LD5			1				// Pin to Data Bus 5
#define MY_LCD_LD6			2				// Pin to Data Bus 6
#define MY_LCD_LD7			3				// Pin to Data Bus 7

#define MY_LCD_LRS			4				// Pin to RS Pin (Register Select)
#define MY_LCD_LRW			5				// Pin to Read/Write Pin
#define MY_LCD_LEN			7				// Pin to Enable Pin
#define MY_LCD_LBL			6				// Backlight Pin
*/
/* Fuer das LCD Modul aus dem Asuro Buch gilt folgende Pinbelegung */

#define MY_LCD_LD4         4            // Pin to Data Bus 4
#define MY_LCD_LD5         5            // Pin to Data Bus 5
#define MY_LCD_LD6         6            // Pin to Data Bus 6
#define MY_LCD_LD7         7            // Pin to Data Bus 7

#define MY_LCD_LRS         0            // Pin to RS Pin (Register Select)
#define MY_LCD_LRW         1            // Pin to Read/Write Pin
#define MY_LCD_LEN         2            // Pin to Enable Pin
#define MY_LCD_LBL         3            // Backlight Pin

#endif /* MYASURO_H */
