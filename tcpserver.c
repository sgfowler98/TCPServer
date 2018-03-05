/*
** Author: sgfowle, Sam Fowler
** Filename: tcpserver.c
** Description: this file provides the implementation of a TCP server that establishes
**		a socket, binds it, then listens for a connection. Upon connecting, it
**		receives ASCII data from the client and prints it in reverse to stdout.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	int ld, sd, addrlen, n, i, k, strlength;
	struct sockaddr_in skaddr, from;
	char buffer[10000];
	char temp;

	//create socket and check for errors
	if ((ld = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error encountered while creating socket");
		exit(1);
	}

	//establish address
	skaddr.sin_family = AF_INET;
	skaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (atoi(argv[1]) < 0 || atoi(argv[1]) > 65535) { //if valid port
		perror("Port number must be between 0 and 65535");
		exit(1);
	}
	skaddr.sin_port = htons(atoi(argv[1]));

	//bind address
	if (bind(ld, (struct sockaddr *) &skaddr, sizeof(skaddr)) < 0) {
		perror("Error encountered while binding");
		exit(1);
	}

	//put the socket into passive mode to await connection
	if (listen(ld, 5) < 0) {
		perror("Error encountered while listening");
		exit(1);
	}

	//process incoming connection
	printf("Ready for a connection...\n");
	addrlen = sizeof(skaddr);
	if ((sd = accept(ld, (struct sockaddr *) &from, &addrlen)) < 0) {
		perror("Error encountered while accepting");
		exit(1);
	}

	//read file line by line, reverse the char array, print, and reset buffer
	n = 1;
	while ((n = read(sd, (char *) &strlength, sizeof(strlength))) > 0) {
		if (n < 0) {
			perror("Error encountered while reading linesize");
			exit(1);
		}
		strlength = htonl(strlength);
		if (strlength > 10000) { //check for buffer overflow
			errno = 1; //set errno to prevent confusing perror() success print
			perror("Error: all input string lengths must be length <= 10000");
			exit(1);
		}
		if ((n = read(sd, &buffer, strlength)) < 0) {
			perror("Error encountered while reading line");
			exit(1);
		}
		for (i = 0, k = strlength - 1; i < strlength/2; i++, k--) {
			temp = buffer[k];
			buffer[k] = buffer[i];
			buffer[i] = temp;
		}
		printf("%s", buffer);
		memset(&buffer, 0, sizeof(buffer));
	}

	printf("\nFinished with connection - closing...\n\n");
	close(sd);

	return(0);
}
