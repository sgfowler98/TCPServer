#
# Author: sgfowle, Sam Fowler
# Filename: Makefile
# Description: A file containing compilation rules for the TCP server and client.
#

LIBS= -lsocket -lnsl

CFLAG= -DSUN

.c.o:	gcc -c -Wall ${CFLAG} $<

all: tcpserver tcpclient

tcpserver: tcpserver.o
	gcc -o tcpserver tcpserver.o

tcpclient: tcpclient.o
	gcc -o tcpclient tcpclient.o

clean:
	rm *.o tcpserver tcpclient


