/****************************************************************************/
/*!
  \file     asuro.c

  \brief    Init- und Interrupt-Funktionen der Asuro Library.\n
            Timer2 wird mit 36 kHz betrieben, im Gegensatz zur ausgelieferten\n
            Version mit 72 kHz.

  \par      Interrupt-Doku nur ueber die Datei zu sehen:
            Bitte ueber den oben ausgegebenen Link \b 'gehe \b zum \b Quellcode \n
            \b dieser \b Datei' direkt in der Datei nachsehen.\n
            DoxyGen ist nicht zur Erkennung von Interrupt-Funktionen 'bereit'.\n
            Behandelt werden folgende Interrupts:\n
            - TIMER2_OVF_vect : Timer 2 (fest mit 36 kHz belegt)\n
            - ADC_vect         : Analog-Digital-Wandler

  \par      Wichtiger Hinweis:
            Die Init()-Funktion muss von jedem Programm beim Start\n
            aufgerufen werden.\n

  \see      Defines zum setzen von Port's und Konfigurationen in asuro.h\n
            IRTX, LEFT_DIR, PWM, GREEN_LED, RIGHT_DIR, FRONT_LED,\n
            ODOMETRIE_LED, RED_LED, ON, OFF, GREEN, FWD, TRUE, FALSE

  \version  V--- - 10.11.2003 - Jan Grewe - DLR\n
            Original Version von der ASURO CD\n
  \version  V--- - 20.11.2006 - m.a.r.v.i.n\n
            +++ SIGNAL (ADC_vect)\n
            static Variable toggle mit FALSE initialisiert.\n
            (Bug report von Rolf_Ebert)
  \version  V--- - bis zum 07.01.2007 - \n
            Bitte in Datei CHANGELOG nachsehen.\n
  \version  V001 - 13.01.2007 - m.a.r.v.i.n\n
            +++ Alle Funktionen\n
            Zerlegte Sourcen in einzelne Dateien fuer eine echte Library.
  \version  V002 - 27.01.2007 - Sternthaler\n
            +++ Alle Funktionen\n
            Kommentierte Version (KEINE Funktionsaenderung)
  \version  V003 - 20.02.2007 - m.a.r.v.i.n\n
            Defines fuer Dometrie High/Low Werte aus myasuro.h verwenden
            StopSwitch ersetzt. Deshalb wurde immer die komplette switches.o 
            mitgelinkt
  \version  V004 - 15.11.2007 - m.a.r.v.i.n\n
            RIGHT_DIR und LEFT_DIR waren in der Init Funktion vertauscht
  \version  V005 - 29.01.2008 - m.a.r.v.i.n\n           
            Initialisierung fuer ATmega168\n
            UART Baudrate einstellbar durch Define\n
            Interrupt User Funktionen f�r Timer und A/D Wandler      
  \version  V006 - 29.03.2009 - rossir\n
            A/D Wandler Abfrage ueber Interrupt  
  \version  V007 - 2013-11-25 - matze
			Some SIGNALS became deprecated, so I replaced them:
			SIG_OVERFLOW2			->	TIMER2_OVF_vect
			SIG_OUTPUT_COMPARE2A	->	TIMER2_COMPA_vect
			SIG_OUTPUT_COMPARE2		->	TIMER2_COMP_vect
			SIG_ADC					->	ADC_vect	
          
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

#define BAUD_RATE   2400
#define F_CPU 16000000UL

/***************************************************************************
  \brief 
  Speicher f�r die ADC Werte aus IsrStandard().
  Hierauf greifen die nichtinterrupt-Funktionen PollSwitch(), LineData(..) etc. zu.
  */ 
volatile unsigned int	adcValue[6];
  
/***************************************************************************
  \brief 
  Interrupt-Funktion fuer den AD-Wandler. Kann auch, ueber autoencode gesteuert, 
  die Odometrie-Zaehler in encoder hochzaehlen.
  Bemerkung: 
  Alle ADC Werte kommen durch die ADC hardware pipeline 2 Interrupts sp�ter an. 

  \param 
  keine 

  \return 
  nichts 

  \see 
  Die globale Variable autoencode wird hier ausgewertet. Ist sie nicht FALSE,\n 
  dann wird der AD-Wandler-Wert zum Zaehlen der Odometriewerte in der globalen\n 
  Variablen encoder benutzt.\n 
  Es wird auch der AD-Wandler-Kanal auf die 'andere' Seite der Odometrie\n 
  umgeschaltete und der AD-Wandler neu gestartet.\n 
  Somit wird erreicht, dass zumindest die Odometriemessung automatisch erfolgt. 
	Anmerkungen zu encoder:
	1) 
	In avg wird ein gleitender Mittelwert mitgef�hrt: a(n+1) = (a(n)+sensor)/4
	(Ich shifte um 2 und habe damit einen Teiler von 4.)

	2) 
	Ich wollte der Interruptroutine Platz und Zeit ersparen und habe deshalb 
	auf eine Initialisierung von avg verzichtet. Durch den kleinen Teiler 4 
	passt sich avg sehr rapide den ADC-Werten an. Und verliert in der Realit�t 
	auch "nie" den ersten Tick. (So muss ASURO auch meist erst mal anfahren.) 

	3) 
	Allerdings - und hier habe ich noch nicht probiert - k�nnte es mit ein Teiler von 8 
	zum ersten mal gelingen, bei Tageslicht, auch ohne eingeschaltetem Encoder-LED, 
	Ticks zu sammeln (allerdings verrauscht). Und das spart immerhin Energie - 
	falls es wirklich darauf ankommt. 

  * ADC_vect: handles all ADC and odometry ticking
  
  \par  Beispiel: 
int main(void) { 
   int lSoll=60, rSoll=30, lPwm=lSoll, rPwm=rSoll, delta=50; 
   Init();    
   EncoderInit(); 
   EncoderStart();    
    
   while(1) { 
      int lIst=encoder[LEFT],   rIst=encoder[RIGHT]; 
      EncoderSet(0, 0);    
       
      lPwm+=(delta*lSoll - 1000*lIst)/300;       
      rPwm+=(delta*rSoll - 1000*rIst)/300; 
       
      SetMotorPower(lPwm, rPwm); 
      Msleep(delta);       
   } 
} 
*****************************************************************************/ 
void IsrStandard(void) {
	static int sign[2]={1,1}, avg[2], adc_cnt = 0;
	static unsigned char mux[]={
		_BV(ADLAR) | _BV(REFS0) | IR_RIGHT,	// AVCC reference with external capacitor
		_BV(ADLAR) | _BV(REFS0) | IR_LEFT,	// AVCC reference with external capacitor
		_BV(ADLAR) | _BV(REFS0) | SWITCH,
		_BV(ADLAR) | _BV(REFS0) | BATTERIE | _BV(REFS1),	// internal 2.56V reference with external capacitor
		_BV(ADLAR) | _BV(REFS0) | WHEEL_RIGHT,	// AVCC reference with external capacitor
		_BV(ADLAR) | _BV(REFS0) | WHEEL_LEFT,	// AVCC reference with external capacitor
	};
	// Vielleicht hat Rakke ja Recht: 
	// ADCL muss zuerst gelesen werden! Sonst k�nnen sich zwei Wandlungen �berschneiden.
   	unsigned int sensor = ADCL | (ADCH << 8);	
	adcValue[adc_cnt]=sensor;
	
	if(autoencode)		// Aus Kompatibilit�tsgr�nden wird autoencode weiter benutzt
	{
		if(adc_cnt<2) { // WHEEL_RIGHT || WHEEL_LEFT
	    	int s=(sensor >> 8); 
		    // In avg wird ein gleitender Mittelwert mitgef�hrt: a(n+1) = 0.75*a(n)+0.25*s
		    avg[adc_cnt] += (s-avg[adc_cnt])>>2; 
		
			// Schneidet die aktuelle Sensorkurve den gleitenden Mittelwert? Konkret:
		    // Weicht der aktuelle Sensorwert um mehr als +/- 2 vom gleitenden Mittelwert ab? 
		    if (sign[adc_cnt]*(s-avg[adc_cnt]) > 2) 
		    { 
		       // Dann z�hle einen Tick weiter. 
		       // Und n�chster Tick erst wieder bei -/+ 2 Abweichung vom gleitenden Mittelwert. 
		       encoder[adc_cnt^RIGHT]++; 
		       sign[adc_cnt] = -sign [adc_cnt]; 
		    } 
		}
	}
	
	if(adc_cnt==SWITCH) {
		switched=switched || sensor<(MY_SWITCH_THRESHHOLD<<6);	// Es wurde (ganz sicher) eine Taste gedr�ckt
	}
	
	ADMUX = mux[adc_cnt];
	adc_cnt=(adc_cnt+1) % sizeof(mux);
}

/****************************************************************************/
/*!
  \brief
  Initialisiert die Hardware: Ports, A/D Wandler, Serielle Schnittstelle, PWM\n
  Die Init Funktion muss von jeden Programm beim Start aufgerufen werden 

  \see
  Die Funktionen Sleep() und Msleep() in time.c werden mit dem hier\n
  eingestellten 36 kHz-Takt betrieben.\n

  \par  Funktionsweise der Zeitfunktionen:
  Msleep() ruft Sleep() auf. In Sleep() wird die globale Variable count36kHz\n
  zur Zeitverzoegerung benutzt. Diese Variable wird jedesmal im Interrupt\n
  TIMER2_OVF_vect um 1 hochgezaehlt.\n
  Der Interrupt selber wird durch den hier eingestellten Timer ausgeloesst.\n
  Somit ist dieser Timer fuer die Zeitverzoegerung zustaendig.

  \see
  Die globale Variable autoencode fuer die automatische Bearbeitung der\n
  Odometrie-ADC-Wandler wird hier auf FALSE gesetzt.

  \par  Hinweis zur 36 kHz-Frequenz vom Timer 2
  Genau diese Frequenz wird von dem Empfaengerbaustein benoetigt und kann\n
  deshalb nicht geaendert werden.\n
  In der urspruenglichen, vom Hersteller ausgelieferten LIB, war diese\n
  Frequenz allerdings auf 72 kHz eingestellt. Durch eine geschickte\n
  Umkonfigurierung durch waste konnte diese aber halbiert werden.\n
  Sinnvoll ist dies, da der durch diesen Timer2 auch ausgeloesste Timer-\n
  Interrupt dann nur noch die Haelfte an Rechenzeit in Anspruch nimmt.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // Die Init()-Funktion MUSS IMMER zu Anfang aufgerufen werden.
  int main (void)
  {
    int wert;

    Init ();

    while (1)
    (
        // Dein Programm
    }
    return 0;
  }
  \endcode
*****************************************************************************/
void Init (
  void)
{
  /*
    Timer2, zum Betrieb mit der seriellen Schnittstelle, fuer die
    IR-Kommunikation auf 36 kHz eingestellt.
  */
#if defined(__AVR_ATmega168__)
  // fast PWM, set OC2A on compare match, clear OC2A at bottom, clk/1
  TCCR2A = _BV(WGM20) | _BV(WGM21) | _BV(COM2A0) | _BV(COM2A1);
  TCCR2B = _BV(CS20);
  // interrupt on timer overflow
  TIMSK2 |= _BV(TOIE2); 
#else
  // fast PWM, set OC2A on compare match, clear OC2A at bottom, clk/1
  TCCR2 = _BV(WGM20) | _BV(WGM21) | _BV(COM20) | _BV(COM21) | _BV(CS20);
  // interrupt on timer overflow
  TIMSK |= _BV(TOIE2); 
#endif
  // 36kHz carrier/timer
  OCR2  = 0x91;

  /*
    Die serielle Schnittstelle wurde waerend der Boot-Phase schon
    programmiert und gestartet. Hier werden die Parameter auf 2400 1N8 gesetzt.
  */
#if defined(__AVR_ATmega168__)
   UBRR0L = (uint8_t)(F_CPU/(BAUD_RATE*16L)-1);
   UBRR0H = (F_CPU/(BAUD_RATE*16L)-1) >> 8;
   UCSR0B = (1<<RXEN0) | (1<<TXEN0);
   UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
#else
  UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8; 	// set baud rate
  UBRRL = (((F_CPU/BAUD_RATE)/16)-1);
  UCSRB = (1<<RXEN)|(1<<TXEN);  // enable Rx & Tx
  UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // config USART; 8N1
#endif

  /*
    Datenrichtung der I/O-Ports festlegen. Dies ist durch die Beschaltung der
    Asuro-Hardware nicht aenderbar.
    Port B: Seriell Senden; Richtungsvorgabe Motor links; Takt fuer die
            Geschwindigkeit beider Motoren; Grueneanteil-Status-LED
    Port D: Richtungsvorgabe Motor rechts; Vordere LED;
            Odometrie-LED (Radsensor); Rotanteil-Status-LED
  */
  DDRB = IRTX | RIGHT_DIR | PWM | GREEN_LED;
  DDRD = LEFT_DIR | FRONT_LED | ODOMETRIE_LED | RED_LED | SWITCHES;  // Port-Bits als Output


  /*
    PWM-Kanaele OC1A und OC1B auf 8-Bit einstellen.
    Sie werden fuer die Geschwindigkeitsvorgaben der Motoren benutzt.
  */
  TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
  TCCR1B = _BV(CS11);                 // tmr1-Timer mit MCU-Takt/8 betreiben.

  /*
    Sonstige Vorbereitungen.
    - Alle LED's ausschalten
    - Motoren stoppen und schon mal auf Vorwaerts einstellen.
    - Globale Variable autoencoder ausschalten.
  */
  FrontLED (OFF);
  BackLED (ON, ON);
  BackLED (OFF, OFF);
  StatusLED (GREEN);
  SWITCH_ON; 

  MotorDir (FWD, FWD);
  MotorSpeed (0, 0);

  autoencode = FALSE;

  Ovr2IntFunc = 0;
  AdcIntFunc = IsrStandard;
  
  /*
   * Vorbereitung f�r WHEEL-Interrupts (Encoder)
    Port C als Input => dadurch gehen die Back-LED aus ...
  */
  DDRC &= ~ (_BV(PC0) | _BV(PC1));

  /*
    ... aber nun koennen die LED's am Rad eingeschaltet werden, und die
        Sensoren koennen gemessen werden.
  */
  ODOMETRIE_LED_ON;

  /*
    Linken Odometrie-Sensor auswaehlen. (AVCC ref. with external capacitor)
  */
  ADMUX = _BV(ADLAR) | _BV(REFS0) | WHEEL_LEFT;

  /*
    AD-Wandler einschalten, Parameter einstellen und Starten. (clk/128)
    Startet den ADC im 'free running'-Mode (ADFR). Das heisst, der Wandler
    nach einer Messung automatisch wieder neu startet.
  */
  ADCSRA = _BV(ADEN) | _BV(ADFR) | _BV(ADIE) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
  
  /*
    Funktion zum ALLGEMEINEN ZULASSEN von Interrupts.
  */
  sei ();
}

/****************************************************************************/
/*
  \brief
  Interrupt-Funktion fuer Timer-2-Ueberlauf.

  \see
  count36kHz, timebase

  \par
  Der zum Timer gehoerende Zaehler TCNT2 wird so justiert, dass damit die\n
  gewuenschten 36 kHz erreicht werden.\n
  Fuer die Zeitfunktionen werden die globalen Variablen count36kHz und\n
  timebase hochgezaehlt.

  \par 
  Die Variable Ovr2IntFunc kann als Zeiger auf eine User Funktion benutzt werden\n
  und wird dann, falls ungleich 0, von der Interrupt Funktion aus angesprungen.

  \par  Beispiel:
  (Nicht vorhanden)
*****************************************************************************/
SIGNAL (TIMER2_OVF_vect)
{
  TCNT2 += 0x25;
  count36kHz ++;
  if (!count36kHz)
    timebase ++;
  if (Ovr2IntFunc)
    Ovr2IntFunc();
}


/**
 * being used insted TIMER2_OVF_vect during ultrasonic polling
 */
#if defined(__AVR_ATmega168__)
SIGNAL(TIMER2_COMPA_vect)
#else
SIGNAL(TIMER2_COMP_vect) 
#endif
{
	count36kHz++;
  if (!count36kHz)
    timebase ++;
}


/****************************************************************************/
/*
  \brief
  Interrupt-Funktion fuer den AD-Wandler. Kann ueber autoencode gesteuert\n
  die Odometrie-Zaehler in encoder hochzaehlen.

  \see
  Die globale Variable autoencode wird hier ausgewertet. Ist sie nicht FALSE,\n
  dann wird der AD-Wandler-Wert zum Zaehlen der Odometriewerte in der globalen\n
  Variablen encoder benutzt.\n
  Es wird auch der AD-Wandler-Kanal auf die 'andere' Seite der Odometrie\n
  umgeschaltete und der AD-Wandler neu gestartet.\n
  Somit wird erreicht, dass zumindest die Odometriemessung automatisch erfolgt.

  \par 
  Die Variable AdcIntFunc kann als Zeiger auf eine User Funktion benutzt werden\n
  und wird dann, falls ungleich 0, von der Interrupt Funktion aus angesprungen.

  \par  Beispiel:
  (Nicht vorhanden)
*****************************************************************************/
SIGNAL (ADC_vect)
{
    AdcIntFunc();
}

