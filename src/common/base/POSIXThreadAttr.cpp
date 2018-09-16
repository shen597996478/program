#include "POSIXThreadAttr.h"

namespace CBase {

POSIXThreadAttr::POSIXThreadAttr( void )
	  : m_threadAttr()
{
	::memset( &m_threadAttr, 0, sizeof(m_threadAttr) );
}

POSIXThreadAttr::~POSIXThreadAttr( void )
{
}


int
POSIXThreadAttr::initialize( void )
{
	int posix_status = 0;


	posix_status = ::pthread_attr_init( &(m_threadAttr) );

	if ( 0 != posix_status ) {
		LOGD(LOG_TAG, "pthread_attr_init() : return error.");
	}


	return posix_status;
}

int
POSIXThreadAttr::setStackSize( size_t size )
{
	int posix_status = 0;

	posix_status = ::pthread_attr_setstacksize( &(m_threadAttr), size );

	return posix_status;
}

int
POSIXThreadAttr::setDetachState( enum DetachState state )
{
	int posix_status = 0;
	int detachstate = PTHREAD_CREATE_JOINABLE;

	switch ( state ) {
	case DETACH_JOINABLE:
		detachstate = PTHREAD_CREATE_JOINABLE;
		break;
	case DETACH_DETACHED:
		detachstate = PTHREAD_CREATE_DETACHED;
		break;
	default:
		detachstate = PTHREAD_CREATE_JOINABLE;
		break;
	}

	posix_status = ::pthread_attr_setdetachstate( &(m_threadAttr), detachstate ); 

	return posix_status;
}

int
POSIXThreadAttr::setSchedPolicy( enum SchedPolicy policy )
{
	int posix_status = 0;
	int sched_policy = SCHED_OTHER;

	switch ( policy ) {
	case SCHEDULE_NORMAL:
		sched_policy = SCHED_OTHER;
		break;
	case SCHEDULE_RR:
		sched_policy = SCHED_RR;
		break;
	case SCHEDULE_FIFO:
		sched_policy = SCHED_FIFO;
		break;
	default:
		sched_policy = SCHED_OTHER;
		break;
	}

	posix_status = ::pthread_attr_setschedpolicy( &(m_threadAttr), sched_policy );

	return posix_status;
}

int
POSIXThreadAttr::setSchedPriority( int priority )
{
	int posix_status = 0;
	struct sched_param param;

	posix_status = ::pthread_attr_getschedparam( &(m_threadAttr), &param );
	if ( 0 == posix_status ) {
		param.sched_priority = priority;
		
		posix_status = ::pthread_attr_setschedparam( &(m_threadAttr), &param );
	}

	return posix_status;

}

}