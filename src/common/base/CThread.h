#ifndef __CThread_H__
#define __CThread_H__

#include "POSIXThread.h"
#include "POSIXThreadAttr.h"
#include "POSIXCond.h"
#include "POSIXMutex.h"
#include "CCommon.h"

namespace CBase {

class POSIXThread;
class POSIXThreadAttr;
class POSIXCond;
class POSIXMutex;


class CThread
{

public:


	static const size_t DEFAULT_STACK_SIZE = ( 1 * 1024 * 1024 );

	void createThread(size_t size = DEFAULT_STACK_SIZE);

	void stopThread(void);

	void postNotify(void);

protected:

	CThread(const char * threadName = 0);
	CThread(void * (*threadEntryPoint)(void *), void * threadParam = 0, const char * threadName = 0);
	virtual ~CThread(void);
	
	virtual void onRun(void) = 0;
	bool isActive(void);
	void waitNotify(void);
	void exitThread(void);
	void cancelThread(void);
	void joinThread(void);

private:

	const char *		m_threadName;
	POSIXThread *		m_posixThreadInstance;
	POSIXThreadAttr *	m_posixThreadAttribute;
	void * 				(*m_entryPoint)(void *);
	void * 				m_entryParam;
	bool				m_isThreadStop;
	POSIXCond *			m_wakeCondition;
	bool				m_isPostNotify;
	POSIXMutex *		m_lockPostNotify;
	bool				m_isCreate;
	POSIXMutex			m_lockCreate;

private:
	static void * moduleThreadEntry(void * arg);
	CThread(void);
	CThread(const CThread & other);
	CThread & operator = (const CThread & other);
};

}

#endif
