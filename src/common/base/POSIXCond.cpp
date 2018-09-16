#include "POSIXCond.h"

namespace CBase {

POSIXCond::POSIXCond()
	  : m_cond()
{
	memset( &m_cond, 0, sizeof(m_cond) );
}

POSIXCond::~POSIXCond()
{
}

int
POSIXCond::initialize()
{
	int posix_status = 0;

	posix_status = pthread_cond_init( &m_cond, NULL );

	if ( 0 != posix_status ) {
		LOGE(LOG_TAG, "POSIXCond::POSIXCond() : failed pthread_cond_init().");
	}

	return posix_status;
}

int
POSIXCond::signal()
{
	int posix_status = 0;

	posix_status = pthread_cond_signal( &m_cond );

	if ( 0 != posix_status ) {
		LOGE(LOG_TAG, "POSIXCond::POSIXCond() : failed pthread_cond_signal()." );
	}

	return posix_status;
}

int
POSIXCond::broadcast()
{
	int posix_status = 0;

	posix_status = pthread_cond_broadcast( &m_cond );

	if ( 0 != posix_status ) {

		LOGE(LOG_TAG, "POSIXCond::POSIXCond() : failed pthread_cond_broadcast()." );
	}

	return posix_status;
}

int
POSIXCond::wait( POSIXMutex &check_mutex )
{
	int posix_status = 0;

	posix_status = ::pthread_cond_wait( &m_cond, &(check_mutex.m_mutex) );

	if ( 0 != posix_status ) {
		LOGE(LOG_TAG, "POSIXCond::POSIXCond() : failed pthread_cond_wait().");
	}

	return posix_status;
}

int
POSIXCond::destory()
{
	int posix_status = 0;

	posix_status = pthread_cond_destroy( &m_cond ); 
	if ( 0 != posix_status ) {
		switch ( posix_status ){
		case EBUSY:
			// Either thread is currently waiting for cond.
			break;
		default:
			LOGE(LOG_TAG, "POSIXCond::POSIXCond() : failed pthread_cond_destroy().");
			break;
		}
	}
	return posix_status;
}

}