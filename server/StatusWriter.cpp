/*
 * StatusWriter.cpp
 *
 *  Created on: 7 mar 2015
 *      Author: Tomek
 */

#include <iostream>
#include <sstream>
#include "StatusWriter.h"

using namespace std;

StatusWriter::StatusWriter()
{
}

void StatusWriter::logStatus(int clientId,string status)
{
	//Find if we already have a stream for that client
	std::map<int, std::ofstream *>::const_iterator it;
	it = m_monitoredClients.find(clientId);

	if(it != m_monitoredClients.end())
	{
		cout<<"Found stream for that client"<<endl;
		if(it->second->is_open())
		{
			cout<<"File is opened"<<endl;
			if(it->second->good())
			{
				std::stringstream os;
				os<<status<<endl;
				(*it->second)<< os.str() << std::flush;
				it->second->flush();
			}
		}
		else
		{
			cout<<"File is not opened"<<endl;
			ofstream * ofstr = this->openFile(clientId);
			if( ofstr->is_open())
			{
				this->flushStatus(status,ofstr);
			}
		}
	}
	else
	{
		//open new filestream
		cout<<"Openning new filestream"<<endl;
		ofstream * ofstr = this->openFile(clientId);
		if( ofstr->is_open())
		{
			cout<<"New filestream was opened"<<endl;
			this->flushStatus(status,ofstr);
			cout<<"Adding filestream to hashmap"<<endl;
			this->addFileStream(clientId,ofstr);
		}
	}
}

void StatusWriter::addFileStream(int clientId, std::ofstream *fileStream)
{
	m_monitoredClients.insert(pair<int,std::ofstream*>(clientId,fileStream));
}

void StatusWriter::removeFileStream(int clientId)
{
	std::map<int,std::ofstream*>::const_iterator it;
	it = m_monitoredClients.find(clientId);

	if(it != m_monitoredClients.end())
	{
		it->second->close();
		delete it->second;
		m_monitoredClients.erase(clientId);
	}
}

std::ofstream* StatusWriter::openFile(int clientId)
{
	std::stringstream fileNameStr;
	fileNameStr<<"clientId_"<<clientId<<".log";
	string file = fileNameStr.str();
	std::ofstream *fileOfStream = new std::ofstream();
	fileOfStream->open( file.c_str(), ios_base::out | ios_base::app );
	return fileOfStream;
}

void StatusWriter::flushStatus(string status,ofstream * ofstr)
{
	std::stringstream stOs;
	stOs<<status<<endl;
	*ofstr << stOs.str() << std::flush;
	ofstr->flush();
}


void StatusWriter::cleanUp()
{
	std::map<int, std::ofstream*>::const_iterator it;
	for (it = m_monitoredClients.begin(); it != m_monitoredClients.end(); ++it)
	{
		// unregister
		it->second->close();
		// delete the file on the disc
		delete it->second;
	}
}

StatusWriter::~StatusWriter()
{
	cleanUp();
}


