/*
 * MessageReaderHandler.h
 *
 * Class manages logging messages from clients
 * made as a singleton
 *
 *  Created on: 3 mar 2015
 *      Author: Tomek
 */

#ifndef MESSAGEREADERHANDLER_H_
#define MESSAGEREADERHANDLER_H_

#include "IEventHandler.h"
class RequestDispatcher;
class StatusWriter;

class MessageReaderHandler:public IEventHandler
{
public:
    static MessageReaderHandler& getInstance()
    {
        static MessageReaderHandler  instance; // Guaranteed to be destroyed.
                                               // Instantiated on first use.
        return instance;
    }
   virtual void handleEvent(int sd,EventType);
   void setRequestDispatcher(RequestDispatcher*);

private:
   MessageReaderHandler();
   MessageReaderHandler(MessageReaderHandler const&);
   void operator=(MessageReaderHandler const&);
   int m_sockfd;
   RequestDispatcher * m_reqDispatcher;
};



#endif /* MESSAGEREADERHANDLER_H_ */
