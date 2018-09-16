#include "POSIXMutex.h"

namespace CBase {

POSIXMutex::POSIXMutex( void )
	  : m_mutex()
{
	memset( &m_mutex, 0, sizeof(m_mutex) );
}

POSIXMutex::~POSIXMutex( void )
{

}

void POSIXMutex::initialize()
{
	int posix_status = 0;

	posix_status = pthread_mutex_init( &m_mutex, NULL );

	if ( 0 != posix_status ) {
			LOGD(LOG_TAG, "pthread_mutex_init() : return error.");
	}
}

void POSIXMutex::lock()
{
	int posix_status = 0;

	posix_status = pthread_mutex_lock( &m_mutex );

	if ( 0 != posix_status ) {
		switch ( posix_status ) {
		case EINVAL:
			LOGD(LOG_TAG, "pthread_mutex_lock() : return EINVAL.");
			break;
		case EDEADLK:
			LOGD(LOG_TAG, "pthread_mutex_lock() : return EDEADLK.");
			break;
		default:
			LOGD(LOG_TAG, "pthread_mutex_lock() : return error.");
			break;
		}
	}
}

void POSIXMutex::trylock()
{
	int posix_status = 0;

	posix_status = pthread_mutex_trylock( &m_mutex );

	if ( 0 != posix_status ) {
		switch ( posix_status ) {
		case EINVAL:
			// no initialize
			LOGD(LOG_TAG, "pthread_mutex_trylock() : return EINVAL.");
			break;
		case EBUSY:
			// already locked
			LOGD(LOG_TAG, "pthread_mutex_trylock() : return EBUSY.");
			break;
		default:
			LOGD(LOG_TAG, "pthread_mutex_trylock() : return error.");
			break;
		}
	}
}

void POSIXMutex::unlock()
{
	int posix_status = 0;

	posix_status = pthread_mutex_unlock( &m_mutex );

	if ( 0 != posix_status ) {
		switch ( posix_status ) {
		case EINVAL:
			LOGD(LOG_TAG, "pthread_mutex_unlock() : return EINVAL.");
			break;
		case EPERM:
			LOGD(LOG_TAG, "pthread_mutex_unlock() : return EPERM.");
			break;
		default:
			LOGD(LOG_TAG, "pthread_mutex_unlock() : return error.");
			break;
		}
	}
}

void POSIXMutex::destory()
{
	int posix_status = 0;

	posix_status = pthread_mutex_destroy( &m_mutex );

	if ( 0 != posix_status ) {
		switch ( posix_status ) {
		case EBUSY:
			LOGD(LOG_TAG, "pthread_mutex_destroy() : return EBUSY.");
			break;
		default:
			LOGD(LOG_TAG, "pthread_mutex_destroy() : return error.");
			break;
		}
	}
}

}