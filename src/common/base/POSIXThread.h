
#ifndef __POSIXThread_H__
#define __POSIXThread_H__

#include <pthread.h>
#include "CCommon.h"
#include "POSIXThreadAttr.h"
#include "string.h"

class POSIXThreadAttr;

namespace CBase{

class POSIXThread
{
public:
	POSIXThread();
	virtual ~POSIXThread();

	int create( void *(*entryPoint)(void *), void *param );
	int create( POSIXThreadAttr *attribute, void *(*entryPoint)(void *), void *param );
	int cancel( void );
	int join( void **returnParam = NULL );
	void exit( void *returnParam );

private:
	pthread_t m_thread;

	POSIXThread(const POSIXThread &);
	POSIXThread &operator=(const POSIXThread &);

};

}
#endif