/*
 * IEventHandler.h
 *
 * Interface for functional handlers
 *
 *  Created on: 2 mar 2015
 *      Author: Tomek
 */

#ifndef IEVENTHANDLER_H_
#define IEVENTHANDLER_H_

enum EventType
{
  ACCEPT_EVENT = 1,
  READ_EVENT = 2
};


class IEventHandler
{
public:
	//Handle event
	virtual void handleEvent(int sd,EventType et) = 0;
};


#endif /* IEVENTHANDLER_H_ */
