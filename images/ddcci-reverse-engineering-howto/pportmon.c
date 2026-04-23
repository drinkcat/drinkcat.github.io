/*	Parallel port I2C monitor
(c) Nicolas Boichat 2004
   
For more informations see http://www.boichat.ch/nicolas/ddcci/

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <stdio.h>
#include <unistd.h> /* needed for ioperm() */
#include <asm/io.h> /* for outb() and inb() */
#include <sys/time.h>

#define DATA 0x378
#define STATUS DATA+1
#define CONTROL DATA+2

#define SIZE 4*1048576

int main(void) {
   struct timeval lasttv;
   struct timeval newtv;
	unsigned char buf[SIZE+1];
	int i;

   if (ioperm(DATA,3,1)) {
      printf("Sorry, you were not able to gain access to the ports\n");
      printf("You must be root to run this program\n");
      exit(1);
   }

   gettimeofday(&lasttv, NULL);

	for (i = 0; i < SIZE;) {
		buf[i++] = inb(STATUS);
	}

   gettimeofday(&newtv, NULL);
   
   double us = (double)(newtv.tv_usec - lasttv.tv_usec) + (double)((newtv.tv_sec - lasttv.tv_sec)*1000000);
   double usb = us/(SIZE);

	buf[SIZE] = 0x00;
   
   printf("Ns : %f, usec per tick : %f\n", us, usb);

	int old = -1;

	int c = 0;

	int sda = -1;
	int scl = -1;
	int nsda;
	int nscl;

	int bitindex = 0;
	int byteindex = 0;
	char byte[15];
	byte[8] = '\0';
	byte[9] = '\0';
	byte[10] = '\0';
	byte[11] = '\0';
	byte[12] = '\0';
	byte[13] = '\0';
	byte[14] = '\0';

	int j = 0;

	unsigned char bytes[65536];

	int data = 0;

	for (i = 0; i <= SIZE; i++) {
		if (old != buf[i]) {
		   if (old != -1) {
				nsda = (old & 0x10) >> 4;
				nscl = (old & 0x20) >> 5;
				//printf("%f (%d): %#x D%d C%d\n", usb*c, c, old, nsda, nscl);
				if ((scl == 1) && (nscl == 1)) { /* SCL high, SDA changing => start or stop */
					if ((sda == 1) && (nsda == 0)) {
						printf("-->Start\n");
						byteindex = 0;
						bitindex = 0;
						data = 0;
					}
					else if ((sda == 0) && (nsda == 1)) {
						printf("-->Stop (%d, %d)\n", byteindex, bitindex);
					}
				}
				else if ((scl == 0) && (nscl == 1)) { /* SCL getting high => bit or ack received */
					if (bitindex == 8) { //ACK
						printf("-->Byte : %s %#x (%f %d)\n", byte, data, usb*c, c);
						if (byteindex == 0) {
							printf("==>Address : %#x (%s)\n", data >> 1, ((data & 0x01)==0) ? "W": "R");
						}
						bytes[j] = (unsigned char)data;
						bitindex = 0;
						data = 0;
						byteindex++;
						j++;
						c = 0;
						if (nsda != 0) {
							printf("-->!ACK\n");
						}
					}
					else {
						//printf("-->Bit #%d : %d (%f %d)\n", bitindex, nsda, usb*c, c);
						byte[bitindex] = '0' + nsda;
						data = (data << 1) | nsda;
						bitindex++;
					}
				} 
				sda = nsda;
				scl = nscl;
			}
			old = buf[i];
		}
		c++;
	}

	/*if ((bytes[0] == 0x6e) && (bytes[1] == 0x51) && (bytes[2] == 0x84) && (bytes[3] == 0x03) && (bytes[5] == 0x00)) {
		printf("Value to change: %#x\nNew value: %#x %#x\n", bytes[4], bytes[6], bytes[7]);
	}*/

   //printf("%f (%d): %#x D%d C%d\n", usb*c, c, buf[i-1], (buf[i-1] & 0x10) >> 4, (buf[i-1] & 0x20) >> 5);
   
   return 0;
}

/* Slow method */
void slow() {
	int old = -1;
	int new = -1;

	int c = 0;

	while(1) {
		new = inb(STATUS);
		if (old != new) {
		   printf("%d : %#x\n", c, new);
			old = new;
			c = 0;
		}
		c++;
	}
}
