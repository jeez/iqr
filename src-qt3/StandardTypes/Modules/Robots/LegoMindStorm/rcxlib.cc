/*
   This programm is based on Kekoa Proudfoots code
   (see http://graphics.Stanford.EDU/~kekoa/rcx/)
*/

#include "rcxlib.h"

//Locals
unsigned char lastMessage;
unsigned char sendbuf[BUFFERSIZE];
unsigned char recvbuf[BUFFERSIZE];
unsigned char *sp = sendbuf;


// check if the response from the rcx is valid:
void rcx_check_result(int returnvalue){ 
    if (returnvalue == -2) {
        fprintf(stderr, "RCX: Bad link.\n");
//        exit(4);
    }
    if (returnvalue== -1) {
        fprintf(stderr, "RCX: Bad response.\n");
//        exit(2);
    }
    if (returnvalue == 0) {
        fprintf(stderr, "RCX: No response.\n");
//        exit(3);
    }
    if (returnvalue > BUFFERSIZE) {
        fprintf(stderr, "RCX: Response too large.\n");
//        exit(5);
    }
    return;
  
}


// initialise the serial port connection for linux
int rcx_init(const char *port)
{
    int fd = -1;
    int result;
    struct termios ios;

    fprintf(stderr, "RCX: Init %s\n", port);
    if ((fd = open(port, O_RDWR)) < 0) {
        perror("open");
//        exit(1);
	return -1;
    }

    if (!isatty(fd)) {
        close(fd);
//        exit(1);
	return -1;
    }

    memset(&ios, 0, sizeof(ios));
    ios.c_cflag = CREAD | CLOCAL | CS8 | PARENB | PARODD;
    cfsetispeed(&ios, B2400);
    cfsetospeed(&ios, B2400);

    if (tcsetattr(fd, TCSANOW, &ios) == -1) {
        perror("tcsetattr");
//        exit(1);
	return -1;
    }

    /* Clear the last Message*/
    sendbuf[0]= 0x90;
    result = rcx_send(fd, sendbuf, 1, recvbuf, BUFFERSIZE);
    if (result == 1) {  
	fprintf(stderr, "RCX: Init OK %x \n", sendbuf[0]); 
    }
    else (rcx_check_result(result));
    return fd;
}


// close serial port connection
void rcx_close(int fd)
{
    close(fd);
}

// the communication is dependend on this function:
int rcx_send(int fd,  unsigned char *sbuf, int slen, unsigned char *rbuf, int rlen)
{
    unsigned char tbuf[BUFFERSIZE];
    unsigned char vbuf[BUFFERSIZE];
    unsigned char *sp = sbuf;
    struct timeval tv;
    fd_set fds;
    int tlen = 0, vlen, vpos, rpos;
    int sum = 0, retry, returnval, count;
//    int i;

    tbuf[tlen++] = 0x55;
    tbuf[tlen++] = 0xff;
    tbuf[tlen++] = 0x00;
    while (slen--) {
        tbuf[tlen++] = *sp;
        tbuf[tlen++] = (~*sp) & 0xff;
        sum += *sp++;
    }
    tbuf[tlen++] = sum;
    tbuf[tlen++] = ~sum;

    for (retry = 0; retry < RETRIES; retry++) {
        if (write(fd, tbuf, tlen) != tlen) {
            perror("write");
            exit(1);
        }


        //printf ("\nPC: ");
	//for (i=0; i<tlen; i++) {
	//    printf ("%x ",tbuf[i]);
	//}
        vlen = 0;
        while (1) {
            FD_ZERO(&fds);
            FD_SET(fd, &fds);
            tv.tv_sec = 0;
            //tv.tv_usec =   300000;
            tv.tv_usec = 40001;
            if (select(FD_SETSIZE, &fds, NULL, NULL, &tv) == -1) {
                perror("select");
                exit(1);
            }
            if (!FD_ISSET(fd, &fds))
                break;
            if ((count = read(fd, &vbuf[vlen], BUFFERSIZE - vlen)) == -1) {
                perror("read");
                exit(1);
            }
            vlen += count;
	    //printf("%d\n", count);
        }




        //printf ("\nRX: ");
	//for (i=0; i<vlen; i++) {
	//    printf ("%x ",vbuf[i]);
	//}
        //printf ("\n");
	
        /* Check echo */

        returnval = -2;
        if (vlen < tlen)
            continue; /* retry */
        for (vpos = 0; vpos < tlen; vpos++)
            if (tbuf[vpos] != vbuf[vpos])
                break;
        if (vpos < tlen)
            continue; /* retry */

	/* return from message op-codes with no reply */
	//if ((tbuf[3] == 0xf7) && (vpos == vlen)) {returnval=1; break;}

        /* Check reply */

        returnval = 0;
        if (vpos == vlen)
            continue;

        returnval = -1;
        //if (vlen - vpos < 5)
	// break; /* could continue instead */

        if (vbuf[vpos++] != 0x55)
            continue; /* could continue instead */
        if (vbuf[vpos++] != 0xff)
            continue; /* could continue instead */
        if (vbuf[vpos++] != 0x00)
            continue; /* could continue instead */

        for (sum = 0, rpos = 0; vpos < vlen - 2; vpos += 2, rpos++) {
            if (vbuf[vpos] != ((~vbuf[vpos+1]) & 0xff))
                break;
            sum += vbuf[vpos];
            if (rpos < rlen)
                rbuf[rpos] = vbuf[vpos];
        }
        //if (vpos != vlen - 2)
	// break; /* could continue instead */
        if (vbuf[vpos] != ((~vbuf[vpos+1]) & 0xff))
            break; /* could continue instead */
        if ((sum & 0xff) != vbuf[vpos])
            break; /* could continue instead */

        return rpos;
    }

    return returnval;
}




//Toggle the 0x08 bit when the same message is send again
unsigned char rcx_toggle(unsigned char command){
      return (command ^ 0x08);
}




// THE FOLLOWING FUNCTIONS ARE SELF-EXPLAINING
void rcx_power_off(int fd){
    unsigned char command[1];
    unsigned char reply[BUFFERSIZE];
    command[0]= OP_POWER_OFF;
    rcx_send(fd, command, 1, reply, BUFFERSIZE);
}


void rcx_send_message(int fd, int message){
    unsigned char command[2];
    unsigned char reply[BUFFERSIZE];
    int result;
    command[0]= OP_SEND_MESSAGE;
    if (lastMessage == command[0]) {  
	//fprintf(stderr, "Last command == command %d ", command[0]); 
    command[0]=rcx_toggle(command[0]);
    };
    command[1]= message & 0xff;
    result = rcx_send(fd, command, 2, reply, BUFFERSIZE);
    //fprintf(stderr, "Result: %i ", result);   
}



void rcx_play_sound(int fd, int x){
    unsigned char command[2];
    unsigned char reply[BUFFERSIZE];
    int result; 

    command[0]= OP_PLAY_SOUND;
    if (lastMessage == command[0]) {  
	//fprintf(stderr, "Last command == command %d ", command[0]); 
    command[0]=rcx_toggle(command[0]);
    };
    command[1]= x & 0xff;
    result = rcx_send(fd, command, 2, reply, BUFFERSIZE);
    if (result == 1) {  
	//fprintf(stderr, "O.k: %x \n", reply[0]); 
    }
    lastMessage = command[0];
}

void rcx_play_tone(int fd, short frequency, int duration){
    unsigned char command[4];
    unsigned char reply[BUFFERSIZE];
    int result;
    command[0]= OP_PLAY_TONE;
    if (lastMessage == command[0]) {  
	//fprintf(stderr, "Last command == command %d ", command[0]); 
    command[0]=rcx_toggle(command[0]);
    };
   
    command[1]= (frequency & 0xff00) >> 8;
    command[2]= (frequency & 0xff);
    command[3]= duration;
    result= rcx_send(fd, command, 4, reply, BUFFERSIZE);
    if (result == 1) {  
	//fprintf(stderr, "O.k: %x \n", reply[0]); 
    }
    lastMessage = command[0];
}

void rcx_set_motors_direction(int fd, int demand){
    unsigned char command[2];
    unsigned char reply[BUFFERSIZE];
    int result;

    command[0]= OP_SET_MOTORS_DIRECTION;
    if (lastMessage == command[0]) {  
	//fprintf(stderr, "Last command == command %d ", command[0]); 
    command[0]=rcx_toggle(command[0]);
    };
   
    command[1]= demand;
    result = rcx_send(fd, command, 2, reply, BUFFERSIZE);
    if (result == 1) {  
	//fprintf(stderr, "O.k: %x \n", reply[0]); 
    }
    lastMessage = command[0];
}

void rcx_set_motors_state(int fd, int demand){
    unsigned char command[2];
    unsigned char reply[BUFFERSIZE];
    int result;
    command[0]= OP_SET_MOTORS_STATE;
    if (lastMessage == command[0]) {  
	//fprintf(stderr, "Last command == command %d ", command[0]); 
    command[0]=rcx_toggle(command[0]);
    };
   
    command[1]= demand;
    result= rcx_send(fd, command, 2, reply, BUFFERSIZE);    
    if (result == 1) {  
	//fprintf(stderr, "O.k: %x \n", reply[0]); 
    }
    lastMessage = command[0];
}

void rcx_set_motors_power(int fd, int motors, int source, int argument){
    unsigned char command[4];
    unsigned char reply[BUFFERSIZE];
    int result;
 
    command[0]= OP_SET_MOTORS_POWER;
    if (lastMessage == command[0]) {  
	//fprintf(stderr, "Last command == command %d ", command[0]); 
    command[0]=rcx_toggle(command[0]);
    };
   
    command[1]= motors;
    command[2]= source;
    command[3]= argument;    
    
    result=rcx_send(fd, command, 4, reply, BUFFERSIZE);
    lastMessage = command[0];
    if (result == 1) {  
	//fprintf(stderr, "O.k: %x \n", reply[0]); 
    }
    else {rcx_check_result(result);}
}

short rcx_get_value(int fd, int source, int argument){
    unsigned char command[4];
    unsigned char reply[BUFFERSIZE];
    int result;
    short value=0;
 
    command[0]= OP_GETVALUE;
    if (lastMessage == command[0]) {  
	//fprintf(stderr, "Last command == command %d ", command[0]); 
    command[0]=rcx_toggle(command[0]);
    };
   
    command[1]= source;
    command[2]= argument;    

    result = rcx_send(fd, command, 3, reply, BUFFERSIZE);
    if (result == 3) {  
	//fprintf(stderr, "Got values: %x %x \n", reply[1], reply[2]); 
    }
    else {rcx_check_result(result);}
    lastMessage = command[0];
    
    value = ((reply[2] << 8) + reply[1]);
    //fprintf(stderr, "Value: %i", value);  
    return value;
}

void rcx_set_sensor_type(int fd, int sensor, int type){
    unsigned char command[3];
    unsigned char reply[BUFFERSIZE];
    int result = 0;
 
    command[0]= OP_SET_SENSOR_TYPE;
    if (lastMessage == command[0]) {  
	//fprintf(stderr, "Last command == command %d ", command[0]); 
    command[0]=rcx_toggle(command[0]);
    };
   
    command[1]= sensor;
    command[2]= type;    

    result = rcx_send(fd, command, 3, reply, BUFFERSIZE);
    if (result == 1) {  
	//fprintf(stderr, "Got values: %x %x \n", reply[1], reply[2]); 
    }
    else {rcx_check_result(result);}
    lastMessage = command[0];
   
}

