#ifndef __CNetEvent_H__
#define __CNetEvent_H__

#include "CEpoll.h"
#include "base/CThread.h"
#include "base/CChain.h"

#include <sys/epoll.h>

namespace CNet {

#define	MAX_EVENT	1024
class CNetEvent : public CBase::CThread
{

public:
	// event opt
	CNetEvent(int size, const char* name);
	virtual ~CNetEvent();

	void initialize();

	int addEvent(int fd, int opt, void* data);
	int modifyEvent(int fd, int opt, void* data);
	int delEvent(int fd);


protected:
	virtual void onRun(void);
	virtual void depatch(CBase::CChainList *connList) = 0;

private:
	CEpoll m_epoll;
	struct epoll_event m_waitEvents[MAX_EVENT];
	int m_timeout;
	CBase::Node m_freeNode[MAX_EVENT];
	bool m_isStop;
	bool m_isExit;

	CNetEvent(const CNetEvent &);
	CNetEvent &operator=(const CNetEvent &);
};

}

#endif