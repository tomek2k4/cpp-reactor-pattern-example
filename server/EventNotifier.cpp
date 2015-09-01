/*
 * EventNotifier.cpp
 *
 * Implementation of abstract EventNotifier class
 *
 *  Created on: 2 mar 2015
 *      Author: Tomek
 */

#include "EventNotifier.h"
#include "IEventHandler.h"

/**
 * Constructor
 */
EventNotifier::EventNotifier()
{
}

/**
 * Destructor
 */
EventNotifier::~EventNotifier()
{
}

/**
 * Method sets handler object
 *
 */
void EventNotifier::setEventHandler(IEventHandler* eventHandler)
{
	m_eventHandler = eventHandler;
}

/**
 * Methods notifies registered handler
 */
void EventNotifier::notify(int sd,EventType et)
{
	m_eventHandler->handleEvent(sd,et);
}




