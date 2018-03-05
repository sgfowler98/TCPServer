/*
** Author: sgfowle, Sam Fowler
** Filename: tcpclient.c
** Description: this file provides the implementation of a TCP client that connects
**		to a socket, sends data line-by-line, then terminates.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int sk, n, temp, len;
	struct sockaddr_in skaddr;
	char * input;
	char line[10000];
	FILE *file;

	//check for correct argument count
	if (argc != 4) {
		printf("Usage: ./tcpclient <server IP> <server port> <input file>");
		exit(0);
	}

	//attempt socket creation
	if ((sk = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error encountered while creating socket");
		exit(1);
	}

	//fill in address struct and open input file
	skaddr.sin_family = AF_INET;
	#ifndef SUN
	if (inet_aton(argv[1], &skaddr.sin_addr) == 0) {
		printf("Invalid IP address: %s\n", argv[1]);
		exit(1);
	}
	#else //Solaris doesn't have inet_aton, so inet_addr is used, but its return is -1 on failure so the
	if ((skaddr.sin_addr.s_addr = inet_addr(argv[1])) == -1) { //input must be assumed to be invalid
		printf("Invalid IP address: %s\n", argv[1]);
		exit(1);
	}
	#endif
	skaddr.sin_port = htons(atoi(argv[2]));
	input = argv[3];
	file = fopen(input, "r");

	//attempt to connect
	if (connect(sk, (struct sockaddr *) &skaddr, sizeof(skaddr)) < 0) {
		printf("Error encountered while connecting socket");
		exit(1);
	}


	if (file != NULL) {
		//read file line by line, sending server line length and line data, then close file
		while (fgets(line, sizeof(line), file) != NULL) {
			len = strlen(line);
			temp = htonl(len);
			if ((n = write(sk, (char *) &temp, sizeof(temp))) < 0) {
				perror("Error encountered while writing linesize");
				exit(1);
			}
			if ((n = write(sk, line, len)) < 0) {
				perror("Error encountered while writing message");
				exit(1);
			}
		}
		fclose(file);
	}
	else {
		printf("%s is empty.", input);
		exit(0);
	}
	return(0);
}
