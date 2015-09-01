/*
 * StatusWriter.h
 *
 *  Class responsible for managing log files and writing status updates to files
 *
 *  Created on: 7 mar 2015
 *      Author: Tomek
 */

#ifndef STATUSWRITER_H_
#define STATUSWRITER_H_

#include <fstream>
#include <map>

class StatusWriter
{
public:
    static StatusWriter& getInstance()
    {
        static StatusWriter  instance; // Guaranteed to be destroyed.
                                               // Instantiated on first use.
        return instance;
    }

	// Logs status to apropriate file;
	void logStatus(int clientID, std::string status);

	/// remove object from monitoring
	void removeFileStream(int clientId);

	void cleanUp();
private:
   StatusWriter();
   StatusWriter(StatusWriter const&);
   void operator=(StatusWriter const&);
	///< file streams map (clientIds are keys, file streams are values)
   std::map<int, std::ofstream *> m_monitoredClients;

	/// add object to monitoring
	void addFileStream(int clientId, std::ofstream*);

	//helper method to open filestream
	std::ofstream * openFile(int clientId);

	void flushStatus(std::string status,std::ofstream * ofstr);

	/// Logger destructor
	virtual ~StatusWriter();

};




#endif /* STATUSWRITER_H_ */
