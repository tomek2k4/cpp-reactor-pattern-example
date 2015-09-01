/*
 * RequestDispatcher.h
 *
 *  Created on: 27 lut 2015
 *      Author: Tomek
 */

#ifndef REQUESTDISPATCHER_H_
#define REQUESTDISPATCHER_H_

#include "EventNotifier.h"

class RequestDispatcher : public EventNotifier
{
public:
	///Constructor
	RequestDispatcher();

	/// Initialize main functionality
	void initialize();

	///Deinitialize tasks
	void deinitialize();

    /// Thread method listen on socket for connections from clients
    static void dispatcherRoutine(void* param);

    ///Starts listenning on created socket
    void listenOnSocket();

    /// Adds new client socket to connected sockets list
    void addNewClient(int sockfd);

    /// Adds new client socket to connected sockets list
    void removeClient(int sockfd);

    ///Getter for master socket descriptor
    int getMasterSocket();

    const static WORD PORT = 5000;

private:
    //Helper method tasks that needs to be handled in loop
    void handleEvents();

    void createSocket();

    int m_sockfd;

    //set of socket descriptors
    fd_set m_readfds;

    static const int MAX_CONN_LISTEN = 5;
    static const int MAX_CONNECTED_CLIENTS = 10;
    int client_socket[MAX_CONNECTED_CLIENTS];
};



#endif /* REQUESTDISPATCHER_H_ */
