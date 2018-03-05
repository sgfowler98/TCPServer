# TCPServer
TCP Server and Client for Networks &amp; Network Programming class
sgfowle - Sam Fowler

This program is a TCP server and client. The client creates a socket, connects,
and then sends data line-by-line from an input file, then closes the connection.
The server creates a socket, binds the address, listens, then when it accepts a
client it receives the data, reverses the strings, and prints the reversed strings
to stdout, then it closes the connection.

Files:
tcpserver.c
This file provides the implementation of the TCP server

tcpclient.c
This file provides the implementation of the TCP client

Makefile
This file provides compilation rules for the program

Other information:
When the total file length is greater than the server's buffer, ntohl(len) was returning
huge numbers which would make the program exit on the first line length check even though
the length was well below the max length.
When trying to investigate that, sometimes my program was appending 'time' to my first
line of output, and then later it was adding vdso_get_ to my output. I couldn't find
any relevant information about this problem and my only guess is something to do with
the OS.
