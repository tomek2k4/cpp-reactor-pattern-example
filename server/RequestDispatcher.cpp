/*
 * RequestDispatcher.cpp
 *
 *  Created on: 27 lut 2015
 *      Author: Tomek
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <strings.h>
#include "basic_types.h"
#include "RequestDispatcher.h"
#include "SocketException.h"
#include "MessageAcceptorHandler.h"


using namespace std;

RequestDispatcher::RequestDispatcher():m_sockfd(0)
{
}

void RequestDispatcher::dispatcherRoutine(void* param)
{

   cout<<"Enter RequestDispatcher::dispatcherRoutine"<<endl;

   RequestDispatcher* reqDispatcher = (RequestDispatcher*)param;

   cout<<"Start listening on created socket"<<endl;
   try
   {
       reqDispatcher->listenOnSocket();
       cout<<"Waiting for connection.."<<endl;
       while(1)
       {
    	   reqDispatcher->handleEvents();
       }
   }
   catch(SocketException &e1)
   {
		cout<<"Exception occurred: "<< e1.what()<<endl;
		reqDispatcher->deinitialize();
   }

}

void RequestDispatcher::handleEvents()
{
	int max_sd,sd;

    //clear the socket set
    FD_ZERO(&m_readfds);

    //add master socket to set
    FD_SET(this->m_sockfd, &m_readfds);

    max_sd = this->m_sockfd;

    //add child sockets to set
    for (int i = 0 ; i < MAX_CONNECTED_CLIENTS ; i++)
    {
        //socket descriptor
        sd = client_socket[i];
        //if valid socket descriptor then add to read list
        if(sd > 0)
            FD_SET( sd , &m_readfds);
        //highest file descriptor number, need it for the select function
        if(sd > max_sd)
            max_sd = sd;
    }

    //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
    int activity = select( max_sd + 1 , &m_readfds , NULL , NULL , NULL);
    if( (activity<0)  && (errno!=EINTR) )
    {
    	throw SocketException("Error on select");
    }

    //If something happened on the master socket , then its an incoming connection
    if (FD_ISSET(this->m_sockfd, &m_readfds))
    {
    	//Initialize Acceptor handler on the stack
    	MessageAcceptorHandler messageAcceptor = MessageAcceptorHandler(this->m_sockfd,this);

    	//pass 0 as socket file descriptor-assigned in constructor
        this->notify(0,ACCEPT_EVENT);
    }
    else
    {
    	//Data arrived on one of clients
         for (int i = 0; i < MAX_CONNECTED_CLIENTS; i++)
         {
             sd = client_socket[i];
             if (FD_ISSET( sd , &m_readfds))
             {
            	 this->notify(sd,READ_EVENT);
             }
         }
    }
}

void RequestDispatcher::initialize()
{
    //initialise all client_socket[] to 0 so not checked
    for (int i = 0; i < MAX_CONNECTED_CLIENTS; i++)
    {
       client_socket[i] = 0;
    }

    //clear the socket set
    FD_ZERO(&m_readfds);

    createSocket();

    //add master socket to set
    FD_SET(this->m_sockfd, &m_readfds);

}

void RequestDispatcher::deinitialize()
{
    //initialise all client_socket[] to 0 so not checked
    for (int i = 0; i < MAX_CONNECTED_CLIENTS; i++)
    {
       if( client_socket[i] != 0 )
       {
    	   close(client_socket[i]);
    	   client_socket[i] = 0;
       }
    }

    if(m_sockfd!=0)
    {
      close(m_sockfd);
    }
}

void RequestDispatcher::createSocket()
{
	int sockfd,res;
	struct sockaddr_in serv_addr;
	int opt = TRUE;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
    	throw SocketException("Could not create socket");
    }

    m_sockfd = sockfd;

    bzero((char *) &serv_addr, sizeof(serv_addr));

    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
    	throw SocketException("setsockopt failed");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    res=bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr));
    if ( res < 0)
    {
    	stringstream s;
       s<<"Could not bind socket error: "<<res;
       throw SocketException(s.str());
    }

    cout<<"Socket created and bound on port: "<<ntohs(serv_addr.sin_port)<<endl;

}

void RequestDispatcher::listenOnSocket()
{
	//Listen on bound socket, for max 5 connections
    if (listen(this->m_sockfd, MAX_CONN_LISTEN) == -1)
    {
       throw SocketException("Could not listen for connection on defined port");
    }
}

void RequestDispatcher::addNewClient(int sockfd)
{
    for (int i = 0; i < MAX_CONNECTED_CLIENTS; i++)
    {
        //if position is empty
        if( client_socket[i] == 0 )
        {
            client_socket[i] = sockfd;
            break;
        }
    }
}

void RequestDispatcher::removeClient(int sockfd)
{
    for (int i = 0; i < MAX_CONNECTED_CLIENTS; i++)
    {
        //if position is empty
        if( client_socket[i] == sockfd )
        {
            client_socket[i] = 0;
            break;
        }
    }
}


int RequestDispatcher::getMasterSocket()
{
	return m_sockfd;
}
