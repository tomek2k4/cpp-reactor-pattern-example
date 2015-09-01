/*
 * MessageAcceptor.cpp
 *
 *  Created on: 3 mar 2015
 *      Author: Tomek
 */
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <strings.h>
#include "basic_types.h"
#include "RequestDispatcher.h"
#include "MessageAcceptorHandler.h"
#include "MessageReaderHandler.h"
#include "SocketException.h"
#include "../common/CommData.h"

using namespace std;

MessageAcceptorHandler::MessageAcceptorHandler(int sockFileDescriptor,RequestDispatcher *reqDisp)
:m_sockfd(sockFileDescriptor),m_reqDispatcher(reqDisp)
{
	reqDisp->setEventHandler(this);
}

void MessageAcceptorHandler::handleEvent(int sd,EventType et)
{
	int new_sockfd,addrlen;
	struct sockaddr_in address;

	if (et == ACCEPT_EVENT)
	{
		cout<<"Accept event occured"<<endl;
		//Accept new connection
        if ((new_sockfd = accept(this->m_sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            cout<<"Failed to accept new connection"<<endl;
            throw SocketException("Could not accept connection");
        }

        //inform user of socket number - used in send and receive commands
        cout<<"New connection , socket fd is "<< new_sockfd<<
        	  ", ip is : "<<inet_ntoa(address.sin_addr) <<
        	  " port : "<<ntohs(address.sin_port)<<endl;

        OperationType welcome = WELCOME_MESSAGE;
        //send welcome message to client
        if( send(new_sockfd, (char *)&welcome ,sizeof(OperationType), 0) != sizeof(OperationType) )
        {
            cout<<"Could not send";
            close(new_sockfd);
        }
        cout<<"Welcome message sent successfully"<<endl;

        m_reqDispatcher->addNewClient(new_sockfd);

        MessageReaderHandler::getInstance().setRequestDispatcher(m_reqDispatcher);

        m_reqDispatcher->setEventHandler(&MessageReaderHandler::getInstance());
	}
	else
	{
		//Should not be here
		cout<<"Wrong event type called"<<endl;
	}

}


