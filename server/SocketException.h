/*
 * SocketException.h
 *
 *  Created on: 28 lut 2015
 *      Author: Tomek
 */

#ifndef SOCKETEXCEPTION_H_
#define SOCKETEXCEPTION_H_

/*---------------------------- CLASS DESCRIPTION ----------------------------*/
/**
 This class defines exception type used in server.
*/

class SocketException : public std::exception
{

public:
	/// Configuration constructor
	SocketException(const std::string p_exceptionMessage);

    ~SocketException() throw();

	/// Returns description of a message
	virtual const char* what() const throw();
private:
	std::string m_exceptionMessage;	///< Exception message

};



#endif /* SOCKETEXCEPTION_H_ */
