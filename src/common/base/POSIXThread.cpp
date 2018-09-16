#include "POSIXThread.h"

#include "POSIXThreadAttr.h"

namespace CBase {

POSIXThread::POSIXThread( void )
	  : m_thread()
{
	::memset( &m_thread, 0, sizeof(m_thread) );
}

/* ------------------------------------------------------------------------ */
/* デストラクタ */
/* ------------------------------------------------------------------------ */
POSIXThread::~POSIXThread( void )
{
}

/* ------------------------------------------------------------------------ */
/* POSIXスレッド操作 作成*/
/* ------------------------------------------------------------------------ */
int
POSIXThread::create( POSIXThreadAttr *attribute, void *(*entryPoint)(void *), void *param )
{
	int posix_status = 0;

	if ( NULL != attribute ) {
		posix_status = ::pthread_create( &(m_thread), &(attribute->m_threadAttr), entryPoint, param );

		int		state	= 0;
		int		polycy	= 0;
		size_t	size	= 0;
		::pthread_attr_getdetachstate( &(attribute->m_threadAttr), &(state) );
		::pthread_attr_getschedpolicy( &(attribute->m_threadAttr), &(polycy) );
		::pthread_attr_getstacksize( &(attribute->m_threadAttr), &(size) );
		LOGD(LOG_TAG, "POSIXThread::create() param -> %d / %d / %d", state, polycy, size);
	}
	else {
		posix_status = ::pthread_create( &(m_thread), NULL, entryPoint, param );
	}


	if ( 0 != posix_status ) {
		if ( EAGAIN == posix_status ) {
			LOGD(LOG_TAG, "pthread_create() : return EAGAIN.");
		}
		else {
			LOGD(LOG_TAG, "pthread_create() : return error.");
		}
	}
	return posix_status;
}

/* ------------------------------------------------------------------------ */
/* POSIXスレッド操作 作成*/
/* ------------------------------------------------------------------------ */
int
POSIXThread::create( void *(*entryPoint)(void *), void *param )
{
	return POSIXThread::create( NULL, entryPoint, param );
}

/* ------------------------------------------------------------------------ */
/* POSIXスレッド操作 キャンセル指示 */
/* ------------------------------------------------------------------------ */
int
POSIXThread::cancel( void )
{
	int posix_status = 0;

	if ( ESRCH == posix_status ) {
		posix_status = 0;
	}
	else if ( 0 != posix_status ) {
		LOGD(LOG_TAG, "pthread_cancel() : return error.");
	}

	return posix_status;
}

/* ------------------------------------------------------------------------ */
/* POSIXスレッド操作 停止待ち */
/* ------------------------------------------------------------------------ */
int
POSIXThread::join( void **returnParam )
{
	int posix_status = 0;
	void *returnPosixParam    = NULL;

	posix_status = ::pthread_join( m_thread, &returnPosixParam );


	if ( 0 != posix_status ) {
		if ( EINVAL == posix_status ) {
			returnPosixParam = NULL;
		}
		else if ( ESRCH == posix_status ) {
			posix_status = 0;
			returnPosixParam = NULL;
		}
		else if ( EDEADLK  == posix_status ) {
			LOGD(LOG_TAG, "pthread_cancel() : return EDEADLK.");
		}
		else {
			LOGD(LOG_TAG, "pthread_cancel() : return error.");
		}

	}
	if ( NULL != returnParam ) {
		*returnParam = returnPosixParam;
	}

	return posix_status;
}

void
POSIXThread::exit( void *returnParam )
{
	::pthread_exit( returnParam );
}

}
