/*
 * SocketException.cpp
 *
 *  Created on: 28 lut 2015
 *      Author: Tomek
 */

#include <string>
#include "SocketException.h"

/**
	Configuration constructor
*/
SocketException :: SocketException(const std::string p_exceptionMessage)
{
	this->m_exceptionMessage = p_exceptionMessage;
}

SocketException :: ~SocketException() throw()
{

}

/**
	Returns description of a message

	@return exception description
*/
const char* SocketException :: what() const throw()
{
	return this->m_exceptionMessage.c_str();
}


