/*
 * MessageReaderHandler.cpp
 *
 *  Created on: 3 mar 2015
 *      Author: Tomek
 */
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <strings.h>
#include <string>
#include <fstream>
#include <map>
#include "basic_types.h"
#include "RequestDispatcher.h"
#include "MessageAcceptorHandler.h"
#include "MessageReaderHandler.h"
#include "StatusWriter.h"
#include "../common/CommData.h"

using namespace std;

MessageReaderHandler::MessageReaderHandler()
{
}

void MessageReaderHandler::handleEvent(int sd,EventType et)
{
	// If this is accept event switch handler in dispatcher to MessageAcceptorHandler
	if(et == ACCEPT_EVENT)
	{
		cout<<"Accept event in Reader"<<endl;
	}

	if (et == READ_EVENT)
	{
		CommData input;
		CommData output;
		cout<<"Read event in ReaderHandler"<<endl;
		struct sockaddr_in address;
		int valread,addrlen;

		initializeCommData(&input);
		initializeCommData(&output);


        //Check if it was for closing , and also read the incoming message
        if ((valread = read( sd , &input, sizeof(CommData))) == 0)
        {
           //Somebody disconnected , get his details and print
           getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);

           cout<<"Host disconnected , ip "<< inet_ntoa(address.sin_addr)<<
           	  ", port "<<ntohs(address.sin_port)<<endl;

           //remove socket descriptor from socket descriptor array
           m_reqDispatcher->removeClient(sd);

           //Close the socket and mark as 0 in list for reuse
           close( sd );

           //remove client stream
           StatusWriter::getInstance().removeFileStream(6666);
        }
        else
        {
			if(validateCommData(&input))
			{
				cout<<"Successfully validated message from client"<<endl;
				cout<<"Client ID: "<<input.clientId<<endl;
//				cout<<"Status: "<<endl<<input.statusUpdate<<endl;

				//Send acknowledge messgae
		        output.type = STATUS_OK;
		        //send welcome message to client
		        if( send(sd, (char *)&output.type ,sizeof(OperationType), 0) != sizeof(OperationType) )
		        {
		            cout<<"Could not send STATUS_OK"<<endl;
		        }
		        cout<<"STATUS_OK sent successfully"<<endl;

		        StatusWriter::getInstance().
		        		logStatus(input.clientId,string(input.statusUpdate));
			}
			else
			{
				cout<<"Validation messge failed"<<endl;
				//Send not acknowledge messgae
		        output.type = STATUS_ERROR;
		        //send welcome message to client
		        if( send(sd, (char *)&output.type ,sizeof(OperationType), 0) != sizeof(OperationType) )
		        {
		            cout<<"Could not send STATUS_ERROR"<<endl;
		        }
		        cout<<"STATUS_ERROR sent successfully"<<endl;
			}

			cout<<"Logged something from socket: "<<sd<<endl;
        }
	}

}

void MessageReaderHandler::setRequestDispatcher(RequestDispatcher * reqDisp)
{
	m_reqDispatcher = reqDisp;
}


