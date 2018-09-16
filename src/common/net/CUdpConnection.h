#ifndef __CUdpConnection_H__
#define __CUdpConnection_H__

//#include <stddef .h>
#include "CSocket.h"

namespace CNet {

class CUdpConnection : public CSocket
{
public:
	CUdpConnection(const char* ip, const int port);
	virtual ~CUdpConnection();

	int create(int listenNum);
	void destroy();
	size_t recv(void *buf, size_t len);
	size_t send(const void *buf, size_t len);

private:
	CUdpConnection(const CUdpConnection&);
	CUdpConnection& operator =(const CUdpConnection&);
};

}

#endif