/*
 * MessageAcceptorHandler.h
 *
 *  Accespt socket connection from client and sets reader
 *  handler if coonection made successfully
 *
 *  Created on: 3 mar 2015
 *      Author: Tomek
 */

#ifndef MESSAGEACCEPTORHANDLER_H_
#define MESSAGEACCEPTORHANDLER_H_

#include "IEventHandler.h"


class MessageAcceptorHandler : public IEventHandler
{
public:
	MessageAcceptorHandler(const int sockFileDescriptor,RequestDispatcher *);
	virtual void handleEvent(int sd,EventType et);

private:
	int m_sockfd;
	RequestDispatcher *m_reqDispatcher;
};



#endif /* MESSAGEACCEPTORHANDLER_H_ */
