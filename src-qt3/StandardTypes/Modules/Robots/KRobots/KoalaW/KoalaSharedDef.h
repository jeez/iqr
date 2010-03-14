/* ************************************************************************** */
/* ** Definitions **/
/* ************************************************************************** */
#define NEWLINE_CHAR		0x0A

/* ************************************************************************** */
/* Wireless data format:													  */
/*  byte  0 : Wireless Sender ID											  */
/*  byte  1 : Wireless Recipient ID											  */
/*  byte  2 : Sender Modul ID (or 0) or RECEIVED_DATA_NOTIFY				  */
/*  byte  3 : Recipient Modul ID (or 0)										  */
/*  byte  4 : Command as listed below										  */
/*  bytes 5..n: command parameter											  */
/* ************************************************************************** */
/* ** Wireless IDs **/
/* ************************************************************************** */
#define COMMAND_BROADCAST_ID    0				// Command to all Koalas, e.g. STOP
#define BASE_STATION_ID			1			    // Control PC
#define THIS_KOALA_ID			NEW_KOALA_ID	// my ID number

#define NEW_KOALA_ID			2
#define OLD_KOALA_ID			3

#define RECEIVED_DATA_NOTIFY	255				// in byte 2

/* ************************************************************************** */
/* TWI data format:															  */
/*  byte  0 : Sender Modul ID	 											  */
/*  byte  1 : Recipient Modul ID											  */
/*  byte  2 : Command as listed below										  */
/*  bytes 3..n: command parameter											  */
/* ************************************************************************** */
/* ** TWI IDs **/
/* ************************************************************************** */
#define TWI_BROADCAST_ID		      0x00
#define TWI_MASTER_ID			 	  0x01
#define TWI_KOALA_ID			 	  0x02
#define TWI_PANTILT_ID			 	  0x03
#define TWI_GYRO_ID				 	  0x04

#define TWI_WHISKERBOARDS_ID		  0x10
#define TWI_WHISKERBOARD_LEFT_ID      0x11
#define TWI_WHISKERBOARD_RIGHT_ID     0x12

/* ************************************************************************** */
/* ** TWI Protocol **/
/* ************************************************************************** */
// TWI_MASTER
#define CMD_MASTER_SEND_KOALA_DATA		0x01    // + SenderID + Data for Master
#define CMD_MASTER_SEND_PANTILT_DATA	0x01    // + SenderID + Data for Master

// TWI_KOALA:
#define CMD_LED_LEDRED				0x21    // + SenderID +LedNumber (0..7)
#define CMD_LED_LEDGREEN			0x22    // + SenderID +LedNumber (0..7)
#define CMD_LED_LEDOFF				0x23    // + SenderID +LedNumber (0..7)
#define CMD_LED_BEEPER				0x24    // + SenderID +BeeperFlag (0..1)
#define CMD_LED_BEEPER_TIMER    	0x25    // + SenderID +BeeperTime in 100ms

#define CMD_KOALA_SEND				0x31    // + SenderID +Data for Koala

// TWI_PANTILT:
#define CMD_PANTILT_SET				0x41    // + SenderID +ServoNumber + Desired Position (-128 .. 127) in degree
#define CMD_PANTILT_CAMERACOMM		0x51    // + SenderID +Data to Camera

// TWI_GYRO:
#define CMD_GYRO_RESET		    	0x61    // + SenderID +Nothing
#define CMD_GYRO_REQUEST_DATA		0x62    // will return the current position and direction

// TWI_WHISKER
#define TWI_WHISKER_COIL_RESET		0xA1	// reset all whisker coils
#define TWI_SCAN_ALL_WHISKERS		0xA2	// Scan Whiskers once

