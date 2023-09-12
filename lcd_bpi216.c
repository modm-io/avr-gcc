/*!
  \file     lcd_bpi216.c
   
  \brief    Funktionen zur Ansteurung eines LCD Moduls ueber die serielle Schnittstelle.
            Diese Bibliothek unterstützt BPI-216 LCD Module mit 2 Zeilen und jeweils 16 Zeichen \n
            Die Bibliothek dient dabei ausschließlich dazu Steuerbefehle an das Display abzusetzten.
			Die eigentliche Textausgabe erfolgt mit den Funktionen für die serielle Schnittstelle.
 
  \author   Matthias Stark
  
  \version  V001 - 2012-09-28 - Matthias Stark
            Erste Version
   */

#include "lcd_bpi216.h"

// LCD interface commands
void LCD_clear(void)
{
	unsigned char buf[1]={254};
	SerWrite(buf,1);
	buf[0]=1;
	SerWrite(buf,1);
}


void LCD_moveTo(unsigned char line, unsigned char pos)
{
	unsigned char buf[1]={254};
	SerWrite(buf,1);
	if(line==1)
	{
		buf[0]=128-1+pos;
		SerWrite(buf,1);
	}
	else if(line==2)
	{
		buf[0]=192-1+pos;
		SerWrite(buf,1);
	}
	else
		;
}