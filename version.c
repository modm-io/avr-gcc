/*!
 * \file version.c
 * \brief Versions Kommentare und Versions String
 *
 * $Revision: 2.70 $
 * $Date: 07. Januar 2007 $
 * $Author: Jan Grewe, Robotrixer, Waste, Stochri, Andun, m.a.r.v.i.n $
 *
 */
/***************************************************************************
*
*it is not allowed to remove the nicknames of the contributers to this software
* from the function header
*/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   any later version.                                                    *
***************************************************************************/

/*******************************************************************************
*
* File Name:   asuro.c
* Project  :   ASURO
*
* Description: This file contains ASURO main features
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00	   14.08.2003   Jan Grewe        build
* 2.00     14.10.2003   Jan Grewe        LEFT_VEL, RIGHT_VEL -> MotorSpeed(unsigned char left_speed, unsigned char right_speed);
*                                        LeftRwd(),LeftFwd(),RightRwd(),
*                                        RigthFwd() -> MotorDir(unsigned char left_dir, unsigned char right_dir);
*                                        GREEN_ON,GREEN_OFF,RED_ON,RED_OFF -> StatusLED(unsigned char color);
*                                        LED_RED_ON, LED_RED_OFF -> FrontLED(unsigned char color);
*                                        Blink(unsigned char left, unsigned char right) -> BackLED(unsigned char left, unsigned char right);
*                                        Alles in Funktionen gefasst => leichter verständlich ?!?!
* 2.10     17.10.2003   Jan Grewe        new Timer funktion void Sleep(unsigned char time36kHz)
*
* Copyright (c) 2003 DLR Robotics & Mechatronics
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library "Robotrixer Buxtehude"
*
* Description: This file contains additional functions:
*
* signal (SIG_ADC)                 interrupt/signal routine for encoder-counter
* signal (SIG_INTERRUPT1)          signal for switches
* EncoderInit()                    initializing encoder-counter
* EncoderStart()                   start autoencoding
* EncoderStop()                    stop autoencoding
* EncoderSet(int,int)              set encodervalue
* Msleep(int delay)                wait for delay in milliseconds
* Gettime()                        get systemtime in milliseconds
* PrintInt(int)
*
* modifications in Sleep, SIG_OUTPUT_COMPARE2, PollSwitch, LineData
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* beta1	   31.03.2005   Robotrixer	 asuro library
* -------  ----------   --------------   ------------------------------
* the encoder source is based on RechteckDemo.c ver 2.0 by Jan Grewe 22.10.2003
* Copyright (c) 2003 DLR Robotics & Mechatronics

*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library modified for IR collision detector
*
* Description: modifications made in following functions:
*
* SIGNAL (SIG_OUTPUT_COMPARE2)	->	SIGNAL (SIG_OVERFLOW2)
* Gettime()				counts now 36kHz
* Init()				timer2 modified for adjustable duty cycle
* Batterie()				bug fixed
* Sleep()				counts now 36kHz
* Msleep()				counts now 36kHz
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* beta2	   11.06.2005   Waste   	 asuro library
* -------  ----------   --------------   ------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: This file contains additional functions:
*
* motor control functions 29.7.2005 stochri
* void Go(int distance)
* void Turn(int degree)
*
* unsigned char Wheelspeed[2]		measured Wheelspeed by interupt
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* sto1     29.07.2005   stochri		 asuro library with motor control functions
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: modifications made in following functions:
*
* void Go(int distance, int speed)
* void Turn(int degree, int speed)
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* And1     31.07.2005   Andun		 Added Speed and Odometrie
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: modifications made in following functions:
*
* void PrintInt(int wert)
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* 2.60     28.09.2005   m.a.r.v.i.n      doxygen comments
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: modifications made in following functions:
*
* SIGNAL (SIG_ADC)
* void PrintInt(int wert)
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* 2.61     20.11.2006   m.a.r.v.i.n      SIGNAL (SIG_ADC): static Variable toggle initialisiert
*                                        auf False (Bug report von Rolf_Ebert)
*                                        PrintInt: Initialisierung text String kann zu Fehler
*                                        beim Flashen mit RS232/IR Adapter fuehren
*                                        (Bug report von francesco)
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: new functions has been added:
*
* void UartPutc(unsigned char zeichen)
* void SerPrint(unsigned char *data)
* void SetMotorPower(int8_t left_speed, int8_t right_speed )
* void sound(uint16_t freq, uint16_t duration_msec, uint8_t amplitude)
*
* Description: modifications made in following functions:
*
* void Go(int distance, int power)
* void Turn(int degree, int speed)
* void PrintInt(int wert)
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* 2.70     07.01.2007   stochri          new functions:
*                                        uart_putc: send single character
*                                        SerPrint: send 0-terminated string
*                                        SetMotorPower: set Motor speed and direction
*                                        sound: Sound Ausgabe ueber die Motoren
*                                        Go: distance in mm
*                                        Turn: comments
*                       m.a.r.v.i.n      PrintInt: SerWrite ersetzt durch SerPrint
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/

#include "asuro.h"

const char version[5] = "2.80";

