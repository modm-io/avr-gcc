/*!
  \file i2c.h
  \brief Definitionen und Funktionen zum I2C Bus.
  
  \par I2C Einfuehrung
  I2C ist ein 2 Draht Bus-System das von Philips entwickelt wurde.
  Dazu wird lediglich eine Datenleitung und eine Taktleitung benötigt 
  (und natuerlich eine gemeinsame Masseleitung). 
  Es gibt eine Reihe von ICs mit I2C Bus Schnittstelle 
  (Digital Port Erweiterungen, A/D Wandler, EEPROMs etc.)
  Damit ergibt sich eine Vielzahl von Moeglichkeiten um die begrenzten
  IO Ressourcen des ASUROs zu erweitern. 
  Bis zu 127 Teilnehmer sind an einem I2C Bus anschliessbar. Dazu werden 
  lediglich 2 Port Pis des ASUROs benötigt.
  Bie meissten ATmel AVR Prozessoren unterstuetzen die I2C Schnittstelle
  per Hardware, TWI (Two Wire Interface) genannt.  
  
  \par I2C am ASURO
  Die Hardware TWI (I2C) Ports sind beim ASURO leider durch den Batterie Abfrage 
  A/D Port und dem Tastsensor A/D Port belegt. Um die I2C Schnittstelle ohne 
  Umbau des ASUROs zu realisieren, wird der I2C BUs mit Software emuliert. 
  Dabei werden füer den I2C Bus die Ports der ASURO Erweiterungsplatine verwendet. 
  Dies sind ADC2 fuer die I2C Taktleitung (SCL) und ADC3 fuer die I2C Datenleitung (SDA). 
  Leider geht dadurch die Funktion des Liniensensors verloren. 
  Da die beiden Fototransistoren ebenfalls die Ports ADC2 und ADC3 belegen. 
  Ausloeten braucht man die Fototransistoren zum Glueck nicht.

  \par I2C Anwendungen
  Ueber den I2C Bus lassen sich z.B. folgende Erweiterungen realisieren.
  \li Eine Digitalport Erweiterung, z.B. 8 IO Ports mit dem PCF8574 
  \li Eine Analogport Erweiterung, z.B. 4 A/D Ports und 1 D/A Port mit dem PCF8594 
  \li Ein LCD Modul mithilfe der Digitalporterweiterung
  \li Ein weiterer AVR Prozessor als I2C Slave, als universelle Porterweiterung. 

  \author raid_ox
  

  \version  V001 - 17.02.2007 - raid_ox\n
            Erste Implementierung
  \version  V002 - 10.02.2007 - m.a.r.v.i.n\n
            Absplittung von asuro.h in eigene Header-Datei,\n
            Doxygen Kommentare (KEINE Funktionsänderung)
 */
/****************************************************************************
*
* File Name:   i2c.h
* Project  :   asuro library 
*
* Description: I2C Functions included:
*
* void InitI2C(void);
* unsigned char WriteI2C(unsigned char byte);
* unsigned char ReadI2C(unsigned char nak);
* unsigned char StartI2C(unsigned char device);
* void StopI2C(void);
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* 2.70rc2  17.02.2007   raid_ox          new functions:
*                                        InitI2C: Initialisation
*                                        WriteI2C: Write Data over I2C
*                                        ...
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/

#ifndef I2C_H
#define I2C_H

#include "myasuro.h"

/* I2C Bus Kommandos und Statusmeldungen */
#define READ    1
#define WRITE   0
#define ACK     1
#define NAK     0

/* I2C Bus Definitionen */

#define SDA     MY_I2C_SDA
#define SCL     MY_I2C_SCL

#define SDA_DDR   DDRC
#define SCL_DDR   DDRC
#define SDA_PIN   PINC
#define SCL_PIN   PINC
#define SDA_PORT  PORTC
#define SCL_PORT  PORTC

#define NOP     asm volatile("nop")         /*<! No Operation */
#define QDEL    NOP; NOP; NOP; NOP; NOP     /*<! 1/4 Delay */
#define HDEL    QDEL; QDEL                  /*<! 1/2 Delay */

#define SDA_HI    SDA_PORT |= (1 << SDA)
#define SDA_LO    SDA_PORT &= ~(1 << SDA)

#define SCL_HI    SCL_PORT |= (1 << SCL)
#define SCL_LO    SCL_PORT &= ~(1 << SCL)

#define SCL_TOGGLE  HDEL; SCL_HI; HDEL; SCL_LO
#define I2C_START SDA_LO; QDEL; SCL_LO
#define I2C_STOP  HDEL; SCL_HI; QDEL; SDA_HI; HDEL

/* I2C Bus Funktionsprototypen */

/*!
 * \~english 
 * \brief Init I2C Hardware
 */
void InitI2C(void);

/*!
 * \~english 
 * \brief Write a Character to I2C Bus
 * \param byte Character to send
 * \return status of I2C 
 */
unsigned char WriteI2C(unsigned char byte);

/*!
 * \~english 
 * \brief Read a Character from I2C Bus
 * \param nak After receive send ack or nak
 * \return received character 
 */
unsigned char ReadI2C(unsigned char nak);

/*!
 * \~english 
 * \brief Start I2C transmission for a specific I2C device
 * \param device device address
 * \return I2C Status
 */
unsigned char StartI2C(unsigned char device);

/*!
 * \~english 
 * \brief Stop I2C transmission. Leave I2C bus in idle mode
 */
void StopI2C(void);

#endif /* I2C_H */
