#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>

#include "NetCommon.h"
#include "CEpoll.h"
//#include "base/MacroDefine.h"
//#include "base/ErrorCode.h"

namespace CNet {

CEpoll::CEpoll(void)
: m_epollFd(-1)
{
}

CEpoll::~CEpoll()
{
	destroy();
}

int CEpoll::create(int listenCount)
{
	// 创建 epoll，Since Linux 2.6.8, the listenCount argument is ignored, but must be greater than zero
	if (listenCount < 1)
	{
		return InvalidParam;
	}
	
    int epfd = ::epoll_create(listenCount);
    if (epfd == -1)
    {
		NET_LOGE("create epoll error = %d, info = %s", errno, strerror(errno));
        return CreateEPollFailed;
    }
	
	m_epollFd = epfd;
	return Success;
}

void CEpoll::destroy()
{
	if (m_epollFd != -1)
	{
		if (::close(m_epollFd) != 0)
		{
			NET_LOGE("close epoll error = %d, info = %s", errno, strerror(errno));
		}
		m_epollFd = -1;
	}
}

int CEpoll::listen(struct epoll_event* waitEvents, const int maxEvents, const int waitTimeout)
{	
	// 等待epoll事件的发生，一次无法输出全部事件，下次会继续输出，因此事件不会丢失
	int num = epoll_wait(m_epollFd, waitEvents, maxEvents, waitTimeout);
	if (num == -1)
	{
		NET_LOGE("epoll wait listen error = %d, info = %s", errno, strerror(errno));
	}
	return num;
}

int CEpoll::addListener(int fd, struct epoll_event& optEv)
{
    if (::epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &optEv) == -1)
	{
		NET_LOGE("add listener to epoll error = %d, info = %s", errno, strerror(errno));
        return AddEPollListenerFailed;
	}
	return Success;
}

int CEpoll::modifyListenEvent(int fd, struct epoll_event& optEv)
{
	if (::epoll_ctl(m_epollFd, EPOLL_CTL_MOD, fd, &optEv) == -1)
	{
		NET_LOGE("modify listen event in epoll error = %d, info = %s", errno, strerror(errno));
        return ModifyEPollListenerFailed;
	}
	return Success;
}

int CEpoll::removeListener(int fd)
{
	struct epoll_event optEv;
	optEv.data.ptr = NULL;
    optEv.events = 0;
	if (::epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, &optEv) == -1)  // 删除监听
	{
		NET_LOGE("remove listener from epoll error = %d, info = %s", errno, strerror(errno));
        return RemoveEPollListenerFailed;
	}
	return Success;
}

}