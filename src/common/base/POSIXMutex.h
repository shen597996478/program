#ifndef __POSIXMutex_H__
#define __POSIXMutex_H__

#include "CCommon.h"
#include <pthread.h>
#include <stdio.h>
#include "string.h"

namespace CBase {

class POSIXMutex
{
public:
	POSIXMutex();
	virtual ~POSIXMutex();

	void initialize();
	void lock();
	void trylock();
	void unlock();
	void destory();
	pthread_mutex_t m_mutex;

private:
	POSIXMutex(const POSIXMutex &);
	POSIXMutex &operator=(const POSIXMutex &);
};

}
#endif