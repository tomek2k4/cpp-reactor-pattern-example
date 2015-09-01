#ifndef COMMDATA_H
#define COMMDATA_H

#ifdef __cplusplus
extern "C" {
#endif

# pragma pack (1)	// Defined to avoid aligning in memory allocation

#define STATUS_UPDATE_LENGTH 690

typedef enum type
{
    STATUS_OK,
    STATUS_ERROR,
    WELCOME_MESSAGE
} OperationType;

typedef struct packet
{
	//First 4 byte can be status if came from server or client's ID
	union
	{
      OperationType	type;		///< Type of operation
	  int           clientId;
	};
    char statusUpdate[STATUS_UPDATE_LENGTH];
} CommData;


__attribute__((always_inline)) inline void initializeCommData( CommData * pData )
{
	bzero( pData, sizeof( CommData ) ) ;
}

__attribute__((always_inline)) inline BOOL validateCommData( const CommData * pData )
{
	BOOL result = FALSE;
	if( sizeof(*pData)  == sizeof(CommData ) )
	{
		result = TRUE;
	}
    return result;
}


# pragma pack ()

#ifdef __cplusplus
}
#endif


#endif	//COMMDATA_H

