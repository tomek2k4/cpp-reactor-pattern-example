/*
 * INotifier.h
 *
 * Abstract class implements basic tasks of notifier of events
 *
 *  Created on: 2 mar 2015
 *      Author: Tomek
 */

#ifndef EVENTNOTIFIER_H_
#define EVENTNOTIFIER_H_

#include "IEventHandler.h"



class EventNotifier
{
public:
	EventNotifier(); /// Constructor
	virtual ~EventNotifier(); /// Destructor
	void setEventHandler(IEventHandler*); /// Store handler
	virtual void notify(int sd,EventType); /// Notify registered handler
private:
	IEventHandler* m_eventHandler; //< Container for observers
};



#endif /* EVENTNOTIFIER_H_ */
