#ifndef __POSIXThreadAttr_H__
#define __POSIXThreadAttr_H__

#include <pthread.h>
#include "CCommon.h"
#include "string.h"

namespace CBase {

class POSIXThreadAttr
{
	friend class POSIXThread;

public:
	enum DetachState {
		DETACH_JOINABLE,	///< 合流可能状態
		DETACH_DETACHED		///< デタッチ状態
	};

	enum SchedPolicy {
		SCHEDULE_NORMAL,	///< 通常値(実装で定義)
		SCHEDULE_RR,		///< ラウンドロビン方式
		SCHEDULE_FIFO		///< 先入れ先出し方式
	};

	POSIXThreadAttr( void );
	virtual ~POSIXThreadAttr( void );

	int initialize( void );
	int setStackSize( size_t size );
	int setDetachState( enum DetachState state );
	int setSchedPolicy( enum SchedPolicy policy );
	int setSchedPriority( int priority );

private:
	pthread_attr_t m_threadAttr;

	POSIXThreadAttr(const POSIXThreadAttr &);
	POSIXThreadAttr &operator=(const POSIXThreadAttr &);
};

}

#endif

