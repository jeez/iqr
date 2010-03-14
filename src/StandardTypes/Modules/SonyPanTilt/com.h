/*----------------------------------------------------------------------------
Neuroinformatik Software Basis NISWB -- Header File
File: 			com.h
Verfasser:		Rolf Nestler
Beschreibung:		Ansteuerung einer seriellen Schnittstelle - class Com
Verwendete Header:	
Included in:		com.cc

$Id: com.h,v 1.2 2002/03/27 10:45:32 expodev Exp $

PP 22.06.99 standalone version
-----------------------------------------------------------------------------*/ 

#ifndef __COM_H
#define __COM_H


//#include	<cstrings.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<termios.h>

#define	COM_STOPBITS_1 	1
#define	COM_STOPBITS_2 	2
#define COM_PARITY_NONE	0
#define COM_PARITY_EVEN	1
#define COM_PARITY_ODD	2
#define	COM_DATABITS_7	7
#define	COM_DATABITS_8	8

/** Klasse zum Ansteuern von seriellen Schnittstellen unter Linux, SunOS und Solaris. \\
Headerfile: com.h \\
Library: libnicom.so (-lnicom) */

class	Com
//: public CommonBase
//-----------------------------------
{	protected:
  
		int	com_handle;
      		int	state;
      		int	time;
		int     timeout;
      		int 	init(int comport, int baud, int bits, int parity, 
					int stop,
      					int hw_protocol);
      		int 	init(char *comport, int baud, int bits, int parity, 
					int stop,
      					int hw_protocol);					
		int   setTimeout(int _timeout);

	public:

/** Constructor. 
Es wird eine Standard-Kommunikation mit 9600 Baud, keiner Parit�t, einem Stopbit und 8 Datenbits aufgebaut.
@param comport bezeichnet die Nummer der zu bedienenden Schnittstelle, beginnend mit 1. Bei Linux ist 1 /dev/ttyS0, 2 /dev/ttyS1, 3..10 /dev/ttyE0../dev/ttyE7. bei SunOS und Solaris ist 1 /dev/ttya und 2 /dev/ttyb. 
@param hw_protocol = 1: es werden die Leitungen RTS und CTS genutzt. */

   		Com(int comport, int hw_protocol = 1); 

/** Constructor. Alle Parameter der Schnittstelle k�nnen frei gew�hlt werden.
@param comport bezeichnet die Nummer der zu bedienenden Schnittstelle, beginnend mit 1. Bei Linux ist 1 /dev/ttyS0, 2 /dev/ttyS1, 3..10 /dev/ttyE0../dev/ttyE7. bei SunOS und Solaris ist 1 /dev/ttya und 2 /dev/ttyb. 
@param baud gibt die Baudrate an. Zul�ssig sind die Konstanten B300 bis B38400 in den g�ngigen Schritten (s. termios.h).
@param bits gibt die Anzahl der Datenbits an. M�gliche Werte: COM\_DATABITS\_7, COM\_DATABITS\_8.
@param parity git die Art der Parit�tspr�fung an. M�gliche Werte: COM\_PARITY\_NONE, COM\_PARITY\_EVEN, COM\_PARITY\_ODD.
@param stop gibt die Anzahl der Stopbits an. M�gliche Werte: COM\_STOPBITS\_1, COM\_STOPBITS\_2
@param hw\_protocol = 1: es werden die Leitungen RTS und CTS genutzt. */

      		Com(int comport, int baud, int bits, int parity, int stop,
      					int hw_protocol);
      		Com(char *comport, int baud, int bits, int parity, int stop,
      					int hw_protocol);
/** Destructor. Die Schnittstelle wird geschlossen. */

		virtual	~Com();

/** Senden der Daten eines Buffers. 
@param data enth�lt die zu sendenden Daten.
@param size bezeichnet die Anzahl der zu sendenden Bytes.
@return Die Funktion liefert momentan immer TRUE */

      		int write(char *data, int size);

/** Empfangen von Daten. 
@param data ist der Buffer, der die empfangenen Daten aufnehmen soll.
@param size bezeichnet die Anzahl der zu empfangenen Bytes. 
@return Die Funktion kehrt im Normalfall erst dann zur�ck, wenn alle angeforderten Bytes (size) empfangen wurden. Momentan liefert sie immer TRUE */ 

      		int read(char *data, int size);

/** Senden eines Strings. 
@param s ist der String, dessen Inhalt gesendet wird. Er sollte am Ende ein CR enthalten. 
@return Momentan liefert die Funktion immer TRUE */

//      		int write(const String &s);

/** Empfangen eines Strings.
@param s ist der String, der die empfangenen Daten aufnehmen soll. Gelesen wird solange, bis im Datenstrom das Zeichen CR auftaucht. 
@return Die Funktion kehrt erst zur�ck, wenn das Zeichen CR gelesen wurde. Sie liefert momentan immer TRUE. */

//      		int read(String &s);

/** Einstellen der Baudrate f�r Sender und Empf�nger. 
@param new_rate stellt die neue Baudrate ein. Erlaubt sind die Konstanten B300 bis B38400 in den g�ngigen Schritten (s. termios.h) */

      		virtual void setBaudrate(int new_rate);

/** Einstellen eines Time Outs f�r die Empfangs-Funktionen - NICHT IMPLEMENTIERT. */

      		int timeoutTime(int new_time)
      		{	time = new_time;	
      			return 1;
      		};

/** Abfragen des Zustandes der Schnittstelle . 
@return Die Funktion liefert TRUE, wenn die Schnittstelle erfolgreich er�ffnet wurde, ansonsten FALSE. */

		int	getState()
		{	return state;	};
		
		void clearComBuffer();
};




#endif

/*************** END OF FILE **********************************************/


