/****************************************************************************/
/*!
  \file printf.c

  \brief
  Print Floating Point Ausgabe Funktionen
  \version  V001 - 16.11.2007 - m.a.r.v.i.n\n
            Original Version von RN-UserIn M1.R

*/

#include "asuro.h"

/****************************************************************************/
/*!
  \brief
  Ausgabe eines Float Wertes als String ueber die serielle Schnittstelle.

  \param[in]
  wert Auszugebender Float Wert (32Bit)

  \param[in]
  vorkomma Anzahl der Vorkomma Stellen

  \param[in]
  nachkomma Anzahl der Nachkomma Stellen

  \author M1.R

  \version  V001 - 16.11.2007    m.a.r.v.i.n\n
            first implementation
            (Idee von M1.R)

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
   SerPrint ("\r\n");
   PrintFloat (123.4567, 3, 4);
   SerPrint ("\r\n");

   SerPrint ("\r\n");
   PrintFloat (-123.4567, 5, 2);
   SerPrint ("\r\n");

   SerPrint ("\r\n");
   PrintFloat (-1.234567, 1, 6);
   SerPrint ("\r\n");

   SerPrint ("\r\n");
   PrintFloat (1.234567, 1, 3);
   SerPrint ("\r\n");
  \endcode
*****************************************************************************/
void PrintFloat (float wert, char vorkomma, char nachkomma)
// vorkomma + nachkomma max 7
{
  char text [10];                                       

  dtostrf(wert, vorkomma, nachkomma, text);
  SerPrint (text);
}

