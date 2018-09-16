#include "CThread.h"

namespace CBase
{

CThread::CThread(const char * threadName)
:	m_threadName(threadName)
,	m_posixThreadInstance(0)
,	m_posixThreadAttribute(0)
,	m_entryPoint(CThread::moduleThreadEntry)
,	m_entryParam(this)
,	m_isThreadStop(false)
,	m_wakeCondition(0)
,	m_isPostNotify(false)
,	m_lockPostNotify(0)
,	m_isCreate(false)
,	m_lockCreate()
{
}

CThread::CThread(void * (*threadEntryPoint)(void *), void * threadParam, const char * threadName)
:	m_threadName(threadName)
,	m_posixThreadInstance(0)
,	m_posixThreadAttribute(0)
,	m_entryPoint(threadEntryPoint)
,	m_entryParam(threadParam)
,	m_isThreadStop(false)
,	m_wakeCondition(0)
,	m_isPostNotify(false)
,	m_lockPostNotify(0)
,	m_isCreate(false)
,	m_lockCreate()
{
}

CThread::~CThread(void)
{
	if (0 != m_wakeCondition) {
		int posixStatus = m_wakeCondition->destory();
		if (EBUSY == posixStatus) {
			LOGE(LOG_TAG, "CThread::~CThread() failed -> m_wakeCondition->destroy()");
		}
		delete m_wakeCondition;
		m_wakeCondition = 0;
	}
	if (0 != m_lockPostNotify) {
		m_lockPostNotify->destory();
		delete m_lockPostNotify;
		m_lockPostNotify = 0;
	}

	if (0 != m_posixThreadAttribute) {
		delete m_posixThreadAttribute;
		m_posixThreadAttribute = 0;
	}
	if (0 != m_posixThreadInstance) {
		delete m_posixThreadInstance;
		m_posixThreadInstance = 0;
	}

	m_isCreate = false;
}

void
CThread::createThread(size_t size)
{
	m_lockCreate.lock();

	// スレッド作成済みなら何もしない
	if (m_isCreate) {
		m_lockCreate.unlock();
		return;
	}

	if (0 == m_wakeCondition) {
		m_wakeCondition = new POSIXCond();
		if (0 == m_wakeCondition) {
			LOGD(LOG_TAG, "CThread::createThread() failed -> memory allocation of m_wakeCondition");
		}
	}
	if (0 != m_wakeCondition) {
		m_wakeCondition->initialize();
	}

	if (0 == m_lockPostNotify) {
		m_lockPostNotify = new POSIXMutex();
		if (0 == m_lockPostNotify) {
			LOGD(LOG_TAG, "CThread::createThread() failed -> memory allocation of m_lockPostNotify");
		}
	}
	if (0 != m_lockPostNotify) {
		m_lockPostNotify->initialize();
	}

#ifndef CThread_UNUSE_POSIX_ATTR
	if (0 == m_posixThreadAttribute) {
		m_posixThreadAttribute = new POSIXThreadAttr();
		if (0 == m_posixThreadAttribute) {
			LOGD(LOG_TAG, "CThread::createThread() failed -> memory allocation of m_posixThreadAttribute");
		}
	}
	if (0 != m_posixThreadAttribute) {
		m_posixThreadAttribute->initialize();
		// スレッドのスタックサイズ
		m_posixThreadAttribute->setStackSize(size);
#if 0
		// スレッドの生成・消滅方法：親と合流可能状態で生成
		m_posixThreadAttribute->setDetachState(POSIXThreadAttr::DETACH_JOINABLE);
#ifdef CThread_DEBUG_TRACE_ON
		// スレッドのスケジューリング方法：通常
		m_posixThreadAttribute->setSchedPolicy(POSIXThreadAttr::SCHEDULE_NORMAL);
#else
		// スレッドのスケジューリング方法：ラウンドロビン
		m_posixThreadAttribute->setSchedPolicy(POSIXThreadAttr::SCHEDULE_RR);
#endif /* CThread_DEBUG_TRACE_ON */
		// スレッドの優先 ( 全て固定 ) ( 優先度設定の方針が決定するまでの仮設定 )
		m_posixThreadAttribute->setSchedPriority(60);
#endif

	}
#endif /* CThread_UNUSE_POSIX_ATTR */

	m_isThreadStop = false;

	// POSIX スレッドのオブジェクトの生成
	if (0 == m_posixThreadInstance) {
		m_posixThreadInstance = new POSIXThread();
		if (0 == m_posixThreadInstance) {
			LOGD(LOG_TAG, "CThread::createThread() failed -> memory allocation of m_posixThreadInstance");
		}
	}
	if (0 != m_posixThreadInstance) {
		int posixStatus = m_posixThreadInstance->create(m_posixThreadAttribute, m_entryPoint, m_entryParam);
		if (0 != posixStatus) {
			LOGE(LOG_TAG, "CThread::createThread() failed -> m_posixThreadInstance->create()");
		}
		else {
			m_isCreate = true;
		}
	}

	m_lockCreate.unlock();
}

// スレッドの停止
void
CThread::stopThread(void)
{
	m_lockCreate.lock();

	if (!(m_isCreate)) {
		m_lockCreate.unlock();
		return;
	}

	m_lockCreate.unlock();

	m_lockPostNotify->lock();
	m_isThreadStop = true;
	m_lockPostNotify->unlock();

	postNotify();
}

void
CThread::postNotify(void)
{
	__AUTO_LOG__
	if (!(m_isCreate)) {
		return;
	}
	__AUTO_LOG__
	m_lockPostNotify->lock();
	if (!(m_isPostNotify)) {
		m_isPostNotify = true;
		m_wakeCondition->signal();
	}
	m_lockPostNotify->unlock();
}

bool
CThread::isActive(void)
{
	if (!(m_isCreate)) {
		return false;
	}

	bool nowActiveState = false;

	m_lockPostNotify->lock();
	nowActiveState = !(m_isThreadStop);
	m_lockPostNotify->unlock();

	return nowActiveState;
}

void
CThread::waitNotify(void)
{
	if (!(m_isCreate)) {
		return;
	}
	__AUTO_LOG__
	m_lockPostNotify->lock();
	if (!(m_isThreadStop)) {
		if (!(m_isPostNotify)) {
			__AUTO_LOG__
			m_wakeCondition->wait(*(m_lockPostNotify));
		}
		m_isPostNotify = false;
	}
	m_lockPostNotify->unlock();
}

void
CThread::cancelThread(void)
{
	m_lockCreate.lock();

	if (!(m_isCreate)) {
		m_lockCreate.unlock();
		return;
	}

	m_lockCreate.unlock();


	int posixStatus = m_posixThreadInstance->cancel();
	if (0 != posixStatus) {
		LOGE(LOG_TAG, "CThread::joinThread() failed -> m_posixThreadInstance->cancel()");
	}
}

void
CThread::joinThread(void)
{
	if (!(m_isCreate)) {
		return;
	}

	int posixStatus = m_posixThreadInstance->join();
	if (0 != posixStatus) {
		LOGE(LOG_TAG, "CThread::joinThread() failed -> m_posixThreadInstance->join()");
	}

	return;
}

void *
CThread::moduleThreadEntry(void * arg)
{
	LOGD(LOG_TAG, "CThread::moduleThreadEntry() enter");

	CThread * moduleArgument = static_cast<CThread *>(arg);

	moduleArgument->m_lockCreate.lock();
	moduleArgument->m_lockCreate.unlock();

	moduleArgument->onRun();

	LOGD(LOG_TAG, "CThread::moduleThreadEntry() exit");

	return static_cast<void *>(moduleArgument);
}

}