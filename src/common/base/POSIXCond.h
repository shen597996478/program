#ifndef __POSIXCond_H__
#define __POSIXCond_H__
#include "CCommon.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "POSIXMutex.h"

namespace CBase {

class POSIXCond
{

public:
	POSIXCond();
	virtual ~POSIXCond();

	int initialize();
	int signal();
	int broadcast();
	int wait(POSIXMutex &check_mutex);

	int destory();

private:
	pthread_cond_t  m_cond;

	POSIXCond(const POSIXCond &);
	POSIXCond &operator=(const POSIXCond &);

};

}
#endif