#include "CNetEvent.h"
#include "NetCommon.h"
#include "unistd.h"

using namespace CBase;

namespace CNet {

CNetEvent::CNetEvent(int size, const char* name)
: CThread(name)
, m_epoll()
, m_timeout(-1)
, m_isStop(false)
, m_isExit(false)
{
	m_epoll.create(size);
}

CNetEvent::~CNetEvent()
{
	m_isStop = true;
	stopThread();
	int cnt = 5;
	while(cnt--) {
		if(!m_isExit) {
			// sleep 100ms
			usleep(100000);
		}
	}
}

void CNetEvent::initialize()
{

	memset(m_waitEvents, 0, sizeof(struct epoll_event) * MAX_EVENT);
	m_timeout = -1;
	m_isStop = false;
	m_isExit = false;
	createThread();
}

int CNetEvent::addEvent(int fd, int opt, void* data)
{
	struct epoll_event optEv;
	optEv.data.ptr = data;
	optEv.events = opt;
	return m_epoll.addListener(fd, optEv);
}

int CNetEvent::modifyEvent(int fd, int opt, void* data)
{
	struct epoll_event optEv;
	optEv.data.ptr = data;
	optEv.events = opt;
	return m_epoll.modifyListenEvent(fd, optEv);
}

int CNetEvent::delEvent(int fd)
{
	return m_epoll.removeListener(fd);
}

void CNetEvent::onRun()
{
	int nfds;
	while(!m_isStop) {
		nfds = m_epoll.listen(m_waitEvents, MAX_EVENT, m_timeout);
		CChainList dataList;
		for (int i = 0; i < nfds; ++i) {
			m_freeNode[i].data = m_waitEvents[i].data.ptr;
			NET_LOGD("%s:%s:%d:events:%d", __FILE__, __func__, __LINE__,m_waitEvents[i].events);
			if(m_waitEvents[i].events & EPOLLIN) 
				dataList.addToEnd(&m_freeNode[i]);
		}
		NET_LOGD("%s:%s:%d", __FILE__, __func__, __LINE__);
		depatch(&dataList);
		NET_LOGD("%s:%s:%d", __FILE__, __func__, __LINE__);

	}

	m_isExit = true;
}

}