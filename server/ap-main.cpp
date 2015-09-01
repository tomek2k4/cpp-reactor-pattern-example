 
//************************************************************
//************************************************************
//**														**
//**	Title:		    Reactor Pattern example				**
//** 	Device / Size:	                					**
//**	Programmed by:										**
//**	Copyright (c):	Tomasz Maslon 						**
//**					All rights reserved					**
//**														**
//**														**
//************************************************************
//************************************************************

#include <time.h>

#include "basic_types.h"
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "RequestDispatcher.h"
#include "SocketException.h"


//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************

using namespace std;

void start_message_dispoatcher()
{
	RequestDispatcher *reqDispatcher = new RequestDispatcher();
	try {
		reqDispatcher->initialize();
	} catch (SocketException &e) {
		cout<<"Exception appeared during dispatcher initialization: "<< e.what()<<endl;
		cout<<"No need to continue terminate program"<<endl;
		reqDispatcher->deinitialize();
		exit(1);
	} catch(...)
	{
		cout << "Cought exception"<<endl;
		exit(1);
	}

	pthread_t server_thread;
	int  iret;

	iret = pthread_create(
			&server_thread,
			NULL,
			(void *(*)(void *))&RequestDispatcher::dispatcherRoutine,
			(void*) reqDispatcher);
    if(iret)
    {
    	cout<< "Error - pthread_create() return code: "<<iret<<endl;
    	exit(1);
    }
    cout<<"pthread_create() for thread 1 returns: "<<dec<<iret<<endl;
    pthread_join( server_thread, NULL);

    reqDispatcher->deinitialize();
    delete reqDispatcher;

}
//***********************************
//***********************************
//********** MAIN FUNCTION **********
//***********************************
//***********************************
int main(int argc, char **argv)
{
	cout << "This is server main function" << endl;

	start_message_dispoatcher();

	return 0;
}







