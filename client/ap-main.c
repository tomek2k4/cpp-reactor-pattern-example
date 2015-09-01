 
//************************************************************
//************************************************************
//**														**
//**	Title:		    ReactorPattern example				**
//** 	Device / Size:	                					**
//**	Programmed by:										**
//**	Copyright (c):	Tomasz Maslon 						**
//**					All rights reserved					**
//**														**
//**														**
//************************************************************
//************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> //read(),write(),close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "type_def.h"
#include "../common/CommData.h"
#include "utils.h"
#define	MAIN_C
#include "ap-main.h"

#define BUFF_SIZE 256
#define SERV_PORT 5000

//***********************************
//***********************************
//********** MAIN FUNCTION **********
//***********************************
//***********************************
int main(int argc, char **argv)
{
    int sockfd, portno, n,repeat=1;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    CommData input;
    CommData output;
    int clientId;
    //add terminating string char
    char printBuffer[STATUS_UPDATE_LENGTH + 1];
    char buffer[BUFF_SIZE];

    //initialize input data structure
    initializeCommData(&input);

    //initialize output data structure
    initializeCommData(&output);

    //terminate if to less arguments
    if (argc < 2) {
       fprintf(stderr,"usage %s clientId [optional]hostname\n", argv[0]);
       exit(0);
    }

    clientId = atoi(argv[1]);
    //set client id in message output
    output.clientId = clientId;
    printf("Client ID: %d \n",clientId);

    // set port number to 5000
    portno = SERV_PORT;

    //create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // get host from argument if provided
    if(argc >= 3)
    {
    	server = gethostbyname(argv[2]);
        bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        printf("Server address provided,sending to %s \n",inet_ntoa(serv_addr.sin_addr));
    }
    else
    {
    	printf("Server address not provided,sending to localhost\n");
    	//set localhost if host ip/name not provided
    	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    }
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    bzero(buffer,BUFF_SIZE);

    //expect welcome message from server
    n = read(sockfd,&input.type,sizeof(input.type));
    if (n < 0)
         error("ERROR reading from socket");

    if(input.type == WELCOME_MESSAGE)
    {
    	printf("Successfuly received welcome message %d\n",input.type);
    }
    else
    {
    	printf("Welcome message did not arrive %d\n",input.type);
    }



send_messages:
    do
    {
    	randstring( output.statusUpdate,STATUS_UPDATE_LENGTH);
    	memcpy(printBuffer,output.statusUpdate,STATUS_UPDATE_LENGTH);
//    	printBuffer[STATUS_UPDATE_LENGTH] = '\0';
//
//      printf("Generated string: %s \n",printBuffer);
		//send data
		n = write(sockfd,&output,sizeof(CommData));
		if (n < 0)
		  error("ERROR writing to socket");
		//expect acknowlege message
		n = read(sockfd,&input.type,sizeof(input.type));
		if (n < 0)
			 error("ERROR reading from socket");

		if(input.type == STATUS_OK)
		{
		  printf("Server acknowledged status update: %d\n",input.type);
		}
		else if(STATUS_ERROR)
		{
		  printf("Server refused status updaete %d\n",input.type);
		}
		repeat--;
    }while(repeat);

	printf("Would you like to send more messages? (write number of messages:[0-255]):");
	scanf("%d", &repeat);
	if(repeat>0)
	{
		if(repeat>255) repeat = 255;
		goto send_messages;
	}

    close(sockfd);
    return 0;
}





