/*
   This programm is based on Kekoa Proudfoots code
   (see http://graphics.Stanford.EDU/~kekoa/rcx/)
*/

#ifndef RCXLIB_H
#define RCXLIB_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>


// Settings
#define OP_ALIVE 0x10
#define OP_CLEAR_MESSAGE 0x90
#define OP_POWER_OFF 0x60

// Effectors
#define OP_PLAY_SOUND 0x51
#define OP_PLAY_TONE 0x23
#define OP_SET_MOTORS_DIRECTION 0xe1
#define OP_SET_MOTORS_STATE 0x21
#define OP_SET_MOTORS_POWER 0x13

// Motor parameters
#define PAR_SET_MOTORS_ON 0x80
#define PAR_SET_MOTORS_OFF 0x40
#define PAR_SET_MOTORS_FLOAT 0x00
#define PAR_SET_MOTORS_FLIP 0x40
#define PAR_SET_MOTORS_FWD 0x80
#define PAR_SET_MOTORS_REV 0x00

// Sensor type
#define OP_SET_SENSOR_TYPE 0x32

// Communication
#define OP_GETVALUE      0x12
#define OP_SEND_MESSAGE  0xf7
#define BUFFERSIZE       4096
#define RETRIES          5

// General RCX constants
#define MAX_MOTOR        3
#define MAX_SENSOR       3
#define MAX_MOTORPOWER   7

// Sensor modes
#define MODE_SET_ID          9
#define MODE_RAW_ID          12
#define MODE_BOOLEAN_ID      13
#define MODE_SET_STRING      "SET"
#define MODE_RAW_STRING      "RAW"
#define MODE_BOOLEAN_STRING  "BOOLEAN"

// Sensor types and strings
#define SENSOR_RAW_ID        0
#define SENSOR_TOUCH_ID      1
#define SENSOR_TEMP_ID       2
#define SENSOR_LIGHT_ID      3
#define SENSOR_ROT_ID        4
#define SENSOR_RAW_STRING    "RAW"
#define SENSOR_TOUCH_STRING  "TOUCH"
#define SENSOR_TEMP_STRING   "TEMP"
#define SENSOR_LIGHT_STRING  "LIGHT"
#define SENSOR_ROT_STRING    "ROT"


void rcx_check_result(int returnvalue); 
int rcx_init(const char *dev);
void rcx_close(int fd);
int rcx_send(int fd,  unsigned char *sbuf, int slen, unsigned char *rbuf, int rlen);
unsigned char rcx_toggle(unsigned char command);
void rcx_power_off(int fd);
void rcx_send_message(int fd, int message);
void rcx_play_sound(int fd, int x);
void rcx_play_tone(int fd, short frequency, int duration);
void rcx_set_motors_direction(int fd, int demand);
void rcx_set_motors_state(int fd, int demand);
void rcx_set_motors_power(int fd, int motors, int source, int argument);
short rcx_get_value(int fd, int source, int argument);
void rcx_set_sensor_type(int fd, int sensor, int type); 

#endif

