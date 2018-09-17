#ifndef __CEpoll_H__
#define __CEpoll_H__

#include <sys/epoll.h>

namespace CNet {

class CEpoll
{
public:
	CEpoll(void);
	virtual ~CEpoll();

	int create(int size);
	void destroy();

	int listen(struct epoll_event* waitEvents, int maxevents, int timeout);
	int addListener(int fd, struct epoll_event& optEv);
	int modifyListenEvent(int fd, struct epoll_event& optEv);
	int removeListener(int fd);

private:
	int 	m_epollFd;

	CEpoll(const CEpoll &);
	CEpoll &operator=(const CEpoll &);
};

}

#endif